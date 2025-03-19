#include "php.h"

static void (*original_zend_execute_ex)(zend_execute_data *execute_data);

// Our custom execute function that wraps zend_execute_ex
static void invariant_execute_ex(zend_execute_data *execute_data) {
    const zend_op_array *op_array = &execute_data->func->op_array;
    if (op_array && op_array->function_name) {
        php_printf("Executing: %s\n", ZSTR_VAL(op_array->function_name));
    }

    // Call the original zend_execute_ex
    original_zend_execute_ex(execute_data);
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
