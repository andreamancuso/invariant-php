#include "php.h"
#include "zend_object_handlers.h" // for OBJ_PROP()
#include "zend_compile.h"  // for zend_unmangle_property_name()
#include "ext/standard/info.h"
#include "ext/standard/php_var.h"

#define PHP_INVARIANT_PHP_VERSION "0.1.1"

static void (*original_zend_execute_ex)(zend_execute_data *execute_data);

// Forward declarations
static void invariant_execute_ex(zend_execute_data *execute_data);
static void call_invariant_if_exists(zend_object *obj);
static zend_bool is_invariant_boundary_call(zend_execute_data *execute_data);
static zend_bool has_active_boundary_call_for_object(zend_execute_data *execute_data, zend_object *obj);

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

    if (invariant_fn
        && (invariant_fn->common.fn_flags & ZEND_ACC_PUBLIC)
        && !(invariant_fn->common.fn_flags & ZEND_ACC_STATIC)) {
        zval retval;
        zval obj_zval, function_name;
        ZVAL_UNDEF(&retval);
        ZVAL_OBJ(&obj_zval, obj);
        ZVAL_STRING(&function_name, "__invariant");

        // Temporarily restore original zend_execute_ex to prevent infinite recursion
        zend_execute_ex = original_zend_execute_ex;

        if (call_user_function(CG(function_table),
                               &obj_zval,
                               &function_name,
                               &retval,
                               0, NULL) == SUCCESS
            && !Z_ISUNDEF(retval)) {
            zval_ptr_dtor(&retval);
        }

        // Restore our custom execution hook
        zend_execute_ex = invariant_execute_ex;
        zval_ptr_dtor(&function_name);
    }
}

///////////////////////////////////////////////////////////////////
// boundary call detection
//
// Detects public method boundaries and skips nested same-object public
// calls by inspecting earlier frames on the Zend call stack.
///////////////////////////////////////////////////////////////////
static zend_bool is_invariant_boundary_call(zend_execute_data *execute_data)
{
    if (!execute_data || !execute_data->func) {
        return 0;
    }

    zend_function *func = execute_data->func;

    if (func->type != ZEND_USER_FUNCTION) {
        return 0;
    }

    if (!func->op_array.scope || Z_TYPE(execute_data->This) != IS_OBJECT) {
        return 0;
    }

    if (!(func->common.fn_flags & ZEND_ACC_PUBLIC)) {
        return 0;
    }

    if (!func->common.function_name) {
        return 0;
    }

    // Undefined method calls also expose the missing-method frame; skip
    // the __call() implementation frame to avoid duplicate checks.
    if (zend_string_equals_literal_ci(func->common.function_name, "__call")
        || zend_string_equals_literal_ci(func->common.function_name, "__invariant")
        || zend_string_equals_literal_ci(func->common.function_name, "__destruct")) {
        return 0;
    }

    return 1;
}

static zend_bool has_active_boundary_call_for_object(zend_execute_data *execute_data, zend_object *obj)
{
    if (!execute_data || !obj) {
        return 0;
    }

    zend_execute_data *frame = execute_data->prev_execute_data;
    while (frame) {
        if (is_invariant_boundary_call(frame) && Z_OBJ(frame->This) == obj) {
            return 1;
        }

        frame = frame->prev_execute_data;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////
// invariant_execute_ex
//
// Hooks zend_execute_ex. After the outermost successful public method
// call for an object, we check invariants.
///////////////////////////////////////////////////////////////////
static void invariant_execute_ex(zend_execute_data *execute_data)
{
    zend_object *obj = NULL;
    zend_bool should_check_after_call = 0;

    if (is_invariant_boundary_call(execute_data)) {
        obj = Z_OBJ(execute_data->This);
        should_check_after_call = !has_active_boundary_call_for_object(execute_data, obj);
    }

    // Execute the real function
    original_zend_execute_ex(execute_data);

    if (should_check_after_call && !EG(exception)) {
        call_invariant_if_exists(obj);
    }
}

///////////////////////////////////////////////////////////////////
// MINIT / MSHUTDOWN
///////////////////////////////////////////////////////////////////
PHP_MINIT_FUNCTION(invariant_php)
{
    REGISTER_STRING_CONSTANT("Invariant\\VERSION", PHP_INVARIANT_PHP_VERSION, CONST_PERSISTENT);

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

PHP_MINFO_FUNCTION(invariant_php)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "invariant_php support", "enabled");
    php_info_print_table_row(2, "Version", PHP_INVARIANT_PHP_VERSION);
    php_info_print_table_row(2, "Invariant checks", "after successful outermost public object method calls");
    php_info_print_table_row(2, "Typed property initialization checks", "enabled");
    php_info_print_table_end();
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
    PHP_MINFO(invariant_php),   // MINFO
    PHP_INVARIANT_PHP_VERSION,  // Version
    STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(invariant_php)
