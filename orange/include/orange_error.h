#ifndef ____ORANGE_ERROR_H
#define ____ORANGE_ERROR_H 1

__thread int _orange_error_code;
__thread char _orange_error_string[1024];
__thread char _orange_debug_string[1024];

#define _orange_set_error(error_code,error_string,...) \
    _orange_error_code = error_code; \
    snprintf(_orange_error_string,1023,error_string, ## __VA_ARGS__); \
    snprintf(_orange_debug_string,1023,"File %s\nFunction: %s\nLine: %d\n", __FILE__ , __FUNCTION__ , __LINE__);

void orange_reset_error(void);
int orange_error(void);

enum _ORANGE_ERROR_CODES {
    ORANGE_NO_ERROR = 0,
    ORANGE_LOW_MEMORY_CODE = 1001,
    ORANGE_INVALID_NUMBER_OF_INPUT_FEATURES_CODE = 301,
    ORANGE_INVALID_NUMBER_OF_UNITS_CODE = 302,
    ORANGE_ACTIVATION_FUNCTION_RETURNED_NULL_CODE = 391;
    ORANGE_ACTIVATION_FUNCTION_RETURNED_INVALID_VECTOR_CODE = 392,
    ORANGE_FEATURE_MATRIX_ROW_COUNT_NOT_EQUAL_TO_TARGET_ROWS_COUNT_CODE = 303,
    ORANGE_INVALID_LEARNING_RATE_CODE = 304,
    ORANGE_INVALID_NUMBER_OF_ITERATIONS_CODE = 305,
    ORANGE_NULL_ARGUMENT_CODE = 90001,
    ORANGE_INVALID_COLLECTION_SIZE_CODE = 90002
};

#define ORANGE_NULL_ARGUMENT "NULL Argument passed for mandatory parameter '%s'"
#define ORANGE_LOW_MEMORY "LOW MEMORY! while trying to allocate %lu bytes"
#define ORANGE_INVALID_COLLECTION_SIZE "Invalid value '%d' passed for '%s'"
#define ORANGE_INVALID_NUMBER_OF_INPUT_FEATURES "Invalid number of input features - %u"
#define ORANGE_INVALID_NUMBER_OF_UNITS "Invalid number of units passed/set - %u"
#define ORANGE_FEATURE_MATRIX_ROW_COUNT_NOT_EQUAL_TO_TARGET_ROWS_COUNT "Incompatible Feature Matrix and Target, Length mismatch - %s(%u) and %s(%u)"
#define ORANGE_INVALID_LEARNING_RATE "Invalid learning rate passed/set - %f"
#define ORANGE_INVALID_NUMBER_OF_ITERATIONS "Invalid number of iterations passed/set - %lu"
#define ORANGE_ACTIVATION_FUNCTION_RETURNED_NULL "Activation function associated with layer[%u] returned NULL. Note: Layer index start from 0, for eg. layer 0 is the input layer."
#define ORANGE_ACTIVATION_FUNCTION_RETURNED_INVALID_VECTOR "Activation function associated with layer[%u] returned invalid vector. Note: Layer index start from 0, for eg. layer 0 is the input layer."

void orange_reset_error(void)
{
    _orange_error_code = ORANGE_NO_ERROR;
    _orange_error_string[0] = '\0';
    _orange_debug_string[0] = '\0';
}


int orange_error(void)
{
    return _orange_error_code;
}


#endif