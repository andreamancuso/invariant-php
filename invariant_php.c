#include "php.h"

static void (*original_zend_execute_ex)(zend_execute_data *execute_data);

// Forward declaration
static void invariant_execute_ex(zend_execute_data *execute_data);

// Function to check if a class has __invariant and call it
static void call_invariant_if_exists(zend_object *obj) {
    if (!obj) return;
    
    zend_class_entry *ce = obj->ce;
    if (!ce) return;
    
    zend_function *invariant_fn = zend_hash_str_find_ptr(&ce->function_table, "__invariant", sizeof("__invariant") - 1);
    if (invariant_fn) {
        zval retval;
        zval obj_zval, function_name;
        ZVAL_OBJ(&obj_zval, obj);
        
        // Initialize function name correctly
        ZVAL_STRING(&function_name, "__invariant");
        
        // Temporarily restore original zend_execute_ex to prevent infinite recursion
        zend_execute_ex = original_zend_execute_ex;
        
        if (call_user_function(CG(function_table), &obj_zval, &function_name, &retval, 0, NULL) == SUCCESS) {
            zval_ptr_dtor(&retval);
        }
        
        // Restore our custom execution hook
        zend_execute_ex = invariant_execute_ex;
        
        zval_ptr_dtor(&function_name); // Free memory
    }
}

// Our custom execute function that wraps zend_execute_ex
static void invariant_execute_ex(zend_execute_data *execute_data) {
    const zend_op_array *op_array = &execute_data->func->op_array;
    
    if (execute_data->func->op_array.scope && Z_TYPE(execute_data->This) == IS_OBJECT) { // Check if it's a class method and an object
        zend_object *obj = Z_OBJ(execute_data->This);
        call_invariant_if_exists(obj); // Call __invariant() before execution
    }

    // Call the original zend_execute_ex
    original_zend_execute_ex(execute_data);
    
    if (execute_data->func->op_array.scope && Z_TYPE(execute_data->This) == IS_OBJECT) { // Call __invariant() after execution
        zend_object *obj = Z_OBJ(execute_data->This);
        call_invariant_if_exists(obj);
    }
}

// MINIT function (called when the module is loaded)
PHP_MINIT_FUNCTION(invariant_php) {
    original_zend_execute_ex = zend_execute_ex;
    zend_execute_ex = invariant_execute_ex;
    return SUCCESS;
}

// MSHUTDOWN function (called when the module is unloaded)
PHP_MSHUTDOWN_FUNCTION(invariant_php) {
    zend_execute_ex = original_zend_execute_ex; // Restore original function
    return SUCCESS;
}

// Module entry definition
zend_module_entry invariant_php_module_entry = {
    STANDARD_MODULE_HEADER,
    "invariant_php", // Extension name
    NULL, // No functions
    PHP_MINIT(invariant_php),
    PHP_MSHUTDOWN(invariant_php),
    NULL, // RINIT
    NULL, // RSHUTDOWN
    NULL, // MINFO
    "0.1", // Version
    STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(invariant_php)