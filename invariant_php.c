#include "php.h"
#include "zend_object_handlers.h" // for OBJ_PROP()
#include "zend_compile.h"  // for zend_unmangle_property_name()
#include "ext/standard/php_var.h"

static void (*original_zend_execute_ex)(zend_execute_data *execute_data);

// Forward declarations
static void invariant_execute_ex(zend_execute_data *execute_data);
static void call_invariant_if_exists(zend_object *obj);

///////////////////////////////////////////////////////////////////
// call_invariant_if_exists
//
// Enumerates ce->properties_info to detect typed properties.
// If any typed property is IS_UNDEF, we throw a fatal error.
// If all props are valid, we call __invariant().
//
// Unmangles private/protected property names so we don't get "".
///////////////////////////////////////////////////////////////////
static void call_invariant_if_exists(zend_object *obj)
{
    if (!obj) {
        return;
    }

    zend_class_entry *ce = obj->ce;
    if (!ce) {
        return;
    }

    // For each declared property in ce->properties_info
    zend_property_info *prop_info;
    ZEND_HASH_FOREACH_PTR(&ce->properties_info, prop_info) {
        // 1) Skip static properties
        if (prop_info->flags & ZEND_ACC_STATIC) {
            continue;
        }

        // Unmangle private/protected property names
        const char *class_name = NULL;
        const char *unmangled_name = NULL;
        zend_unmangle_property_name(prop_info->name, &class_name, &unmangled_name);

        // If the unmangled name is empty, skip it
        if (!unmangled_name || strlen(unmangled_name) == 0) {
            continue;
        }

        // 2) Access the actual property from the object
        zval *z_prop = OBJ_PROP(obj, prop_info->offset);
        // If typed property is uninitialized, z_prop->type == IS_UNDEF

        if (!z_prop || Z_TYPE_P(z_prop) == IS_UNDEF) {
            zend_error(E_ERROR,
                       "Invariant check failed: Uninitialized property '%s' in class '%s'",
                       unmangled_name,
                       ZSTR_VAL(ce->name));
            return; // E_ERROR kills the script
        }
    } ZEND_HASH_FOREACH_END();

    // If no uninitialized properties found, call __invariant if present
    zend_function *invariant_fn = zend_hash_str_find_ptr(
        &ce->function_table, "__invariant", sizeof("__invariant") - 1);

    if (invariant_fn) {
        zval retval;
        zval obj_zval, function_name;
        ZVAL_OBJ(&obj_zval, obj);
        ZVAL_STRING(&function_name, "__invariant");

        // Temporarily restore original zend_execute_ex to prevent infinite recursion
        zend_execute_ex = original_zend_execute_ex;

        if (call_user_function(CG(function_table),
                               &obj_zval,
                               &function_name,
                               &retval,
                               0, NULL) == SUCCESS)
        {
            zval_ptr_dtor(&retval);
        }

        // Restore our custom execution hook
        zend_execute_ex = invariant_execute_ex;
        zval_ptr_dtor(&function_name);
    }
}

///////////////////////////////////////////////////////////////////
// invariant_execute_ex
//
// Hooks zend_execute_ex. Before/after calling a method, we check invariants.
///////////////////////////////////////////////////////////////////
static void invariant_execute_ex(zend_execute_data *execute_data)
{
    // Before execution: check invariants if method call
    if (execute_data->func->op_array.scope && Z_TYPE(execute_data->This) == IS_OBJECT) {
        zend_object *obj = Z_OBJ(execute_data->This);
        zend_class_entry *ce = obj->ce;

        call_invariant_if_exists(obj);
    }

    // Execute the real function
    original_zend_execute_ex(execute_data);

    // After execution: check invariants again if method call
    if (execute_data->func->op_array.scope && Z_TYPE(execute_data->This) == IS_OBJECT) {
        zend_object *obj = Z_OBJ(execute_data->This);

        call_invariant_if_exists(obj);
    }
}

///////////////////////////////////////////////////////////////////
// MINIT / MSHUTDOWN
///////////////////////////////////////////////////////////////////
PHP_MINIT_FUNCTION(invariant_php)
{
    // Hook our custom zend_execute_ex
    original_zend_execute_ex = zend_execute_ex;
    zend_execute_ex = invariant_execute_ex;

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(invariant_php)
{
    // Restore the original zend_execute_ex
    zend_execute_ex = original_zend_execute_ex;
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////
// Module entry
///////////////////////////////////////////////////////////////////
zend_module_entry invariant_php_module_entry = {
    STANDARD_MODULE_HEADER,
    "invariant_php",            // Extension name
    NULL,                       // Functions (none exposed to userland)
    PHP_MINIT(invariant_php),   // MINIT
    PHP_MSHUTDOWN(invariant_php),// MSHUTDOWN
    NULL,                       // RINIT
    NULL,                       // RSHUTDOWN
    NULL,                       // MINFO
    "0.1",                     // Version
    STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(invariant_php)
