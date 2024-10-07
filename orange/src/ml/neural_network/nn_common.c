#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<orange.h>
#include<orange_error.h> // ToDo - Line to be removed in the end , after we shift the framework code to separate source

#include<matrix.h>


// Code for training for Multiclass Classification using neural network

// Amit - The ML Framework writer

extern __thread int _orange_error_code;
extern __thread char _orange_error_string[1024];
extern __thread char _orange_debug_string[1024];

enum ORANGE_GRADIENT_DESCENT_TYPE {ORANGE_BATCH_GRADIENT_DESCENT, ORANGE_MINI_BATCH_GRADIENT_DESCENT, ORANGE_STOCHASTIC_GRADIENT_DESSCENT};


typedef struct __orange_neural_network_model {
    mat_double **parameters;
    dimension_t size;
} orange_neural_network_model;


typedef vec_double* (*orange_neural_network_activation_function_t) (const vec_double*);
typedef orange_neural_network_activation_function_t orange_neural_network_activation_function_derivative_t;

typedef struct __orange_neural_network {
    dimension_t number_of_input_features;
    dimension_t number_of_output_units;
    dimension_t number_of_hidden_layers;
    dimension_t* number_of_units_in_each_hidden_layer;

    //vec_double* (*output_layer_activation_function) (vec_double*);
    //vec_double* (*output_layer_activation_function_der) (vec_double*);
    orange_neural_network_activation_function_t output_layer_activation_function;
    orange_neural_network_activation_function_derivative_t output_layer_activation_function_derivative;

    orange_neural_network_activation_function_t* hidden_layer_activation_functions;
    orange_neural_network_activation_function_derivative_t* hidden_layer_activation_function_derivatives;
    
} orange_neural_network;

// Parameters - iteration_number , y , predicted_y , model , regularization_parameter
typedef int (*orange_gradient_descent_progress_callback_t)(uint64_t,void*,void*,void*,double);

// Parameters - address of feature pointer (training example(s)) , address of target pointer , from row index  , number of rows
typedef void (*orange_gradient_descent_data_provider_t)(void*,void*,uint64_t,uint32_t);

// code for gd options start here

typedef struct __orange_gd_options {
    double learning_rate;
    uint64_t number_of_iterations;
    int gradient_descent_type;
    uint32_t mini_batch_size;
    orange_gradient_descent_progress_callback_t progress_callback;
    orange_gradient_descent_data_provider_t data_provider;
} orange_gd_options;

// orange_gd_options setters

void orange_gd_options_set_progress_callback(orange_gd_options* options, orange_gradient_descent_progress_callback_t progress_callback)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return;
    }
    options->progress_callback = progress_callback;
}

void orange_gd_options_set_data_provider(orange_gd_options* options, orange_gradient_descent_data_provider_t data_provider)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return;
    }
    options->data_provider = data_provider;
}

void orange_gd_options_set_gradient_descent_type(orange_gd_options* options, int gradient_descent_type)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return;
    }
    options->gradient_descent_type = gradient_descent_type;
}

void orange_gd_options_set_learning_rate(orange_gd_options* options, double learning_rate)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return;
    }
    options->learning_rate = learning_rate;
}

void orange_gd_options_set_number_of_iterations(orange_gd_options* options, uint64_t number_of_iterations)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return;
    }
    options->number_of_iterations = number_of_iterations;
}

void orange_gd_options_set_mini_batch_size(orange_gd_options* options, uint32_t mini_batch_size)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return;
    }
    options->mini_batch_size = mini_batch_size;
}

// orange_gd_options getters

orange_gradient_descent_progress_callback_t orange_gd_options_get_progress_callback(orange_gd_options* options)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return NULL;
    }
    return options->progress_callback;
}

orange_gradient_descent_data_provider_t orange_gd_options_get_data_provider(orange_gd_options* options)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return NULL;
    }
    return options->data_provider;
}

int orange_gd_options_get_gradient_descent_type(orange_gd_options* options)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return 0;
    }
    return options->gradient_descent_type;
}

double orange_gd_options_get_learning_rate(orange_gd_options* options)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return 0.0;
    }
    return options->learning_rate;
}

uint64_t orange_gd_options_get_number_of_iterations(orange_gd_options* options)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return 0;
    }
    return options->number_of_iterations;
}

uint32_t orange_gd_options_get_mini_batch_size(orange_gd_options* options)
{
    orange_reset_error();
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return 0;
    }
    return options->mini_batch_size;
}

// code for gd options end here

orange_neural_network* orange_neural_network_create_new(orange_neural_network_model* model)
{
    orange_neural_network* neural_network;
    orange_reset_error();
    neural_network = (orange_neural_network*) calloc(1 , sizeof(orange_neural_network));
    if(!neural_network)
    {
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, sizeof(orange_neural_network));
        return NULL;
    }
    return neural_network;
}

orange_gd_options* orange_gd_options_create_new()
{
    orange_gd_options* options;
    orange_reset_error();
    options = (orange_gd_options*) calloc(1, sizeof(orange_gd_options));
    if(!options)
    {
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, sizeof(orange_gd_options));
        return NULL;
    }
    return options;
}

orange_neural_network_model* orange_neural_network_model_create_new()
{
    orange_neural_network_model* model;
    orange_reset_error();
    model = (orange_neural_network_model*) calloc(1, sizeof(orange_neural_network_model));
    if(!model)
    {
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, sizeof(orange_neural_network_model));
        return NULL;
    }
    return model;
}

void orange_neural_network_model_destroy(orange_neural_network_model* model)
{
    int i;
    if(!model) return;
    for(i=0;i<model->size;++i)
    {
        mat_double_destroy(model->parameters[i]);
        model->parameters[i] = NULL;
    }
    if(model->parameters) free(model->parameters);
    model->parameters = NULL;
    free(model);
}

void orange_neural_network_model_set_parameters(orange_neural_network_model* model, mat_double** parameters, dimension_t number_of_parameters)
{
    orange_reset_error();
    if(!model)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "model");
        return;
    }
    if(!parameters)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "parameters");
        return;
    }
    if(number_of_parameters == 0)
    {
        _orange_set_error(ORANGE_INVALID_COLLECTION_SIZE_CODE, ORANGE_INVALID_COLLECTION_SIZE, number_of_parameters, "number_of_parameters");
        return;
    }
    model->parameters = parameters;
    model->size = number_of_parameters;
}

// Number of features does not contain Bias unit
void orange_neural_network_set_input_layer(orange_neural_network *neural_network, dimension_t number_of_features)
{
    orange_reset_error();
    if(!neural_network)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "neural_network");
        return;
    }
    if(number_of_features == 0)
    {
        _orange_set_error(ORANGE_INVALID_NUMBER_OF_INPUT_FEATURES_CODE, ORANGE_INVALID_NUMBER_OF_INPUT_FEATURES, number_of_features);
        return;
    }
    neural_network->number_of_input_features = number_of_features;
}

void orange_neural_network_add_hidden_layer(orange_neural_network *neural_network, dimension_t number_of_units, orange_neural_network_activation_function_t activation_function, orange_neural_network_activation_function_derivative_t activation_function_derivative)
{
    long unsigned int allocation_size;
    dimension_t v_number_of_hidden_layers;
    dimension_t* v_number_of_units_in_each_hidden_layer;
    orange_neural_network_activation_function_t* v_hidden_layer_activation_functions;
    orange_neural_network_activation_function_derivative_t* v_hidden_layer_activation_function_derivatives;

    orange_reset_error();
    if(!neural_network)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "neural_network");
        return;
    }
    if(number_of_units == 0)
    {
        _orange_set_error(ORANGE_INVALID_NUMBER_OF_UNITS_CODE, ORANGE_INVALID_NUMBER_OF_UNITS, number_of_units);
        return;
    }
    if(!activation_function)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "activation_function");
        return;
    }
    if(!activation_function_derivative)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "activation_function_derivative");
        return;
    }

    v_number_of_hidden_layers = neural_network->number_of_hidden_layers + 1;

    allocation_size = sizeof(dimension_t)*v_number_of_hidden_layers;
    v_number_of_units_in_each_hidden_layer = (dimension_t*) malloc(allocation_size);
    if(!v_number_of_units_in_each_hidden_layer)
    {
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, allocation_size);
        return;
    }

    allocation_size = sizeof(orange_neural_network_activation_function_t)*v_number_of_hidden_layers;
    v_hidden_layer_activation_functions = (orange_neural_network_activation_function_t*) malloc(allocation_size);
    if(!v_hidden_layer_activation_functions)
    {
        free(v_number_of_units_in_each_hidden_layer);
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, allocation_size);
        return;
    }

    allocation_size = sizeof(orange_neural_network_activation_function_derivative_t)*v_number_of_hidden_layers;
    v_hidden_layer_activation_function_derivatives = (orange_neural_network_activation_function_derivative_t*) malloc(allocation_size);
    if(!v_hidden_layer_activation_function_derivatives)
    {
        free(v_number_of_units_in_each_hidden_layer);
        free(v_hidden_layer_activation_functions);
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, allocation_size);
        return;
    }
    if(neural_network->number_of_hidden_layers == 0)
    {
        memcpy(v_number_of_units_in_each_hidden_layer, neural_network->number_of_units_in_each_hidden_layer, sizeof(dimension_t) * neural_network->number_of_hidden_layers);
        memcpy(v_hidden_layer_activation_functions, neural_network->hidden_layer_activation_functions, sizeof(orange_neural_network_activation_function_t) * neural_network->number_of_hidden_layers);
        memcpy(v_hidden_layer_activation_function_derivatives, neural_network->hidden_layer_activation_function_derivatives, sizeof(orange_neural_network_activation_function_derivative_t) * neural_network->number_of_hidden_layers);

        free(neural_network->number_of_units_in_each_hidden_layer);
        free(neural_network->hidden_layer_activation_functions);
        free(neural_network->hidden_layer_activation_function_derivatives);
    }

    v_number_of_units_in_each_hidden_layer[neural_network->number_of_hidden_layers] = number_of_units + 1;
    v_hidden_layer_activation_functions[neural_network->number_of_hidden_layers] = activation_function;
    v_hidden_layer_activation_function_derivatives[neural_network->number_of_hidden_layers] = activation_function_derivative;

    neural_network->number_of_hidden_layers = v_number_of_hidden_layers;
    neural_network->number_of_units_in_each_hidden_layer = v_number_of_units_in_each_hidden_layer;
    neural_network->hidden_layer_activation_functions = v_hidden_layer_activation_functions;
    neural_network->hidden_layer_activation_function_derivatives = v_hidden_layer_activation_function_derivatives;
}

void orange_neural_network_set_output_layer(orange_neural_network* neural_network, dimension_t number_of_units, orange_neural_network_activation_function_t activation_function, orange_neural_network_activation_function_derivative_t activation_function_derivative)
{
    orange_reset_error();
    if(!neural_network)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "neural_network");
        return;
    }
    if(number_of_units == 0)
    {
        _orange_set_error(ORANGE_INVALID_NUMBER_OF_UNITS_CODE, ORANGE_INVALID_NUMBER_OF_UNITS, number_of_units);
        return;
    }
    if(!activation_function)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "activation_function");
        return;
    }
    if(!activation_function_derivative)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "activation_function_derivative");
        return;
    }
    neural_network->number_of_output_units = number_of_units;
    neural_network->output_layer_activation_function = activation_function;
    neural_network->output_layer_activation_function_derivative = activation_function_derivative;
}

// should y be of type mat_double
orange_neural_network_model* orange_neural_network_logistic_batch_gd_fit(orange_neural_network* neural_network, mat_double* X, mat_double* Y, orange_gd_options* options, double regularization_parameter)
{
    uint64_t iteration_number;
    index_t i, j;
    dimension_t r, c;
    orange_neural_network_model* model;
    vec_double** activation_vectors;
    vec_double** product_vectors;
    vec_double** delta_vectors;
    mat_double** theta_matrices;
    mat_double** tmp_theta_matrices;

    unsigned long int allocation_size;
    double learning_rate;
    uint64_t number_of_iterations;
    orange_gradient_descent_progress_callback_t progress_callback;
    dimension_t x_rows, x_columns;
    dimension_t y_rows, y_columns;

    dimension_t number_of_input_features;
    dimension_t number_of_output_units;
    dimension_t number_of_hidden_layers;
    dimension_t* number_of_units_in_each_hidden_layer;


    vec_double* training_example;
    vec_double* tmp_activation_vector = NULL;
    dimension_t tmp_activation_vector_length;



    orange_reset_error();

    // initial validations

    if(!neural_network)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "neural_network");
        return NULL;
    }
    if(!options)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "options");
        return NULL;
    }
    if(!X)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "X");
        return NULL;
    }
    if(!Y)
    {
        _orange_set_error(ORANGE_NULL_ARGUMENT_CODE, ORANGE_NULL_ARGUMENT, "Y");
        return NULL;
    }
    mat_double_get_dimensions(X, &x_rows, &x_columns);
    mat_double_get_dimensions(Y, &y_rows, &y_columns);

    if(x_rows != y_rows)
    {
        _orange_set_error(ORANGE_FEATURE_MATRIX_ROW_COUNT_NOT_EQUAL_TO_TARGET_ROWS_COUNT_CODE, ORANGE_FEATURE_MATRIX_ROW_COUNT_NOT_EQUAL_TO_TARGET_ROWS_COUNT, "X", x_rows, "Y", y_rows);
        return NULL;
    }

    learning_rate = orange_gd_options_get_learning_rate(options);
    number_of_iterations = orange_gd_options_get_number_of_iterations(options);
    progress_callback = orange_gd_options_get_progress_callback(options);

    if(learning_rate<=0.0)
    {
        _orange_set_error(ORANGE_INVALID_LEARNING_RATE_CODE, ORANGE_INVALID_LEARNING_RATE, learning_rate);
        return NULL;
    }
    if(number_of_iterations == 0)
    {
        _orange_set_error(ORANGE_INVALID_NUMBER_OF_ITERATIONS_CODE, ORANGE_INVALID_NUMBER_OF_ITERATIONS, number_of_iterations);
        return NULL;
    }

    // initial validations done

    // (pre) creating structures Neural Network - starts here

    number_of_input_features = neural_network->number_of_input_features;
    number_of_output_units = neural_network->number_of_output_units;
    number_of_hidden_layers = neural_network->number_of_hidden_layers;
    number_of_units_in_each_hidden_layer = neural_network->number_of_units_in_each_hidden_layer;

    // (pre.1) creating <a> vectors - activation vectors
    allocation_size = sizeof(vec_double*) * (number_of_hidden_layers + 2);
    activation_vectors = (vec_double**) calloc(1, allocation_size);
    if(!activation_vectors)
    {
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, allocation_size);
        goto handle_err;
    }
    activation_vectors[0] = vec_double_create_new(number_of_input_features + 1); // +1 to accommodate bias
    if(orange_error()) goto handle_err; // ToDo - change the definition of vec_double functions to set error
    vec_double_set(activation_vectors[0], 0, 1); // setting bias
    for(i=0;i<number_of_hidden_layers;++i)
    {
        j = number_of_units_in_each_hidden_layer[i];
        activation_vectors[i+1] =  vec_double_create_new(j);
        if(orange_error()) goto handle_err;
        vec_double_set(activation_vectors[i+1], 0, 1);
    }
    activation_vectors[i] = vec_double_create_new(number_of_output_units);
    if(orange_error()) goto handle_err;

    // (pre.2) creating <z> vectors (aka product vectors) and  delta vectors (same dimensions required)

    allocation_size = sizeof(vec_double*) * (number_of_hidden_layers + 1);

    product_vectors = (vec_double**) calloc(1, allocation_size);
    if(!product_vectors)
    {
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, allocation_size);
        goto handle_err;
    }
    delta_vectors = (vec_double**) calloc(1, allocation_size);
    if(!delta_vectors)
    {
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, allocation_size);
        goto handle_err;
    }
    for(i=0;i<number_of_hidden_layers;++i)
    {
        product_vectors[i] = vec_double_create_new(number_of_units_in_each_hidden_layer[i] - 1);
        if(orange_error()) goto handle_err;
        product_vectors[i] = vec_double_create_new(number_of_units_in_each_hidden_layer[i] - 1);
        if(orange_error()) goto handle_err;
    }
    product_vectors[i] = vec_double_create_new(number_of_output_units);
    if(orange_error()) goto handle_err;
    product_vectors[i] = vec_double_create_new(number_of_output_units);
    if(orange_error()) goto handle_err;

    // (pre.3) creating theta and temp_theta matrices

    allocation_size = sizeof(mat_double*) * (number_of_hidden_layers + 1);
    theta_matrices = (mat_double**) calloc(1, allocation_size);
    if(!theta_matrices)
    {
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, allocation_size);
        goto handle_err;
    }
    tmp_theta_matrices = (mat_double**) calloc(1, allocation_size);
    if(!tmp_theta_matrices)
    {
        _orange_set_error(ORANGE_LOW_MEMORY_CODE, ORANGE_LOW_MEMORY, allocation_size);
        goto handle_err;
    }
    for(i=0;i<number_of_hidden_layers + 1;++i)
    {
        vec_double_get_length(product_vectors[i], &r);
        vec_double_get_length(activation_vectors[i], &c);
        theta_matrices[i] = mat_double_create_new_random_filled(r, c, 0, 1);
        if(orange_error()) goto handle_err;
        tmp_theta_matrices[i] = mat_double_create_new_filled(r, c, 0.0);
        if(orange_error()) goto handle_err;
    }

    // (pre) creating structures Neural Network - ends here

    // Code to train the parameters starts here
    for(iteration_number = 1;iteration_number <= number_of_iterations;++iteration_number)
    {
        for(i=0;i<x_rows;++i)
        {
            // load ith row in 1st layer

            training_example = mat_double_row_to_vector(X, i, product_vectors[0]);
            if(!training_example) printf("Cant load a training example!!!\n");

            // forward propagation starts
            for(j = 0;j < number_of_hidden_layers;++j)
            {
                vec_double_matrix_vector_multiplication(theta_matrices[j], activation_vectors[j], product_vectors[j]);
                tmp_activation_vector = neural_network->hidden_layer_activation_functions[j](product_vectors[j]);
                if(!tmp_activation_vector)
                {
                    _orange_set_error(ORANGE_ACTIVATION_FUNCTION_RETURNED_NULL_CODE, ORANGE_ACTIVATION_FUNCTION_RETURNED_NULL, j+1);
                    goto: handle_err;
                }
                vec_double_get_length(tmp_activation_vector,&tmp_activation_vector_length);
                if(tmp_activation_vector_length != number_of_units_in_each_hidden_layer[j] - 1)
                {
                    _orange_set_error(ORANGE_ACTIVATION_FUNCTION_RETURNED_INVALID_VECTOR_CODE, ORANGE_ACTIVATION_FUNCTION_RETURNED_INVALID_VECTOR_CODE, j+1);
                    goto: handle_err;
                }
                vec_double_copy(tmp_activation_vector, activation_vectors[j+1],0,1,tmp_activation_vector_length);
                vec_double_destroy(tmp_activation_vector);
            }
            vec_double_matrix_vector_multiplication(theta_matrices[j], activation_vectors[j], product_vectors[j]);
            tmp_activation_vector = neural_network->output_layer_activation_function(product_vectors[j]);
            if(!tmp_activation_vector)
            {
                _orange_set_error(ORANGE_ACTIVATION_FUNCTION_RETURNED_NULL_CODE, ORANGE_ACTIVATION_FUNCTION_RETURNED_NULL, j+1);
                goto: handle_err;
            }
            vec_double_get_length(tmp_activation_vector,&tmp_activation_vector_length);
            if(tmp_activation_vector_length != number_of_units_in_each_hidden_layer[j] - 1)
            {
                _orange_set_error(ORANGE_ACTIVATION_FUNCTION_RETURNED_INVALID_VECTOR_CODE, ORANGE_ACTIVATION_FUNCTION_RETURNED_INVALID_VECTOR_CODE, j+1);
                goto: handle_err;
            }
            vec_double_copy(tmp_activation_vector, activation_vectors[j+1],0,1,tmp_activation_vector_length);
            vec_double_destroy(tmp_activation_vector);

            // forward propagation ends

            // backward propagation starts



            // backward propagation ends
        }
        // update theta matrices
         if(progress_callback)
         {
            // progress_callback();
         }
    }
    // Code to train the parameters ends here

    // release allocations starts
    for(i=0;i<number_of_hidden_layers+2;++i)
    {
        vec_double_destroy(activation_vectors[i]);
    }
    free(activation_vectors);
    activation_vectors = NULL;

    for(i=0;i<number_of_hidden_layers+1;++i)
    {
        vec_double_destroy(product_vectors[i]);
    }
    free(product_vectors);
    product_vectors = NULL;

    for(i=0;i<number_of_hidden_layers+1;++i)
    {
        vec_double_destroy(delta_vectors[i]);
    }
    free(delta_vectors);
    delta_vectors = NULL;

    for(i=0;i<number_of_hidden_layers+1;++i)
    {
        mat_double_destroy(theta_matrices[i]);
    }
    free(theta_matrices);
    theta_matrices = NULL;

    for(i=0;i<number_of_hidden_layers+1;++i)
    {
        mat_double_destroy(tmp_theta_matrices[i]);
    }
    free(tmp_theta_matrices);
    tmp_theta_matrices = NULL;
    // release allocations ends

    model = orange_neural_network_model_create_new();
    if(orange_error()) goto handle_err;
    orange_neural_network_model_set_parameters(model, theta_matrices, number_of_hidden_layers + 1);
    return model;

    handle_err:

        if(activation_vectors)
        {
            for(i=0;i<number_of_hidden_layers+2;++i)
            {
                if(activation_vectors[i])
                {
                    vec_double_destroy(activation_vectors[i]);
                }
            }
            free(activation_vectors);
            activation_vectors = NULL;
        }

        if(product_vectors)
        {
            for(i=0;i<number_of_hidden_layers+1;++i)
            {
                if(product_vectors[i])
                {
                    vec_double_destroy(product_vectors[i]);
                }
            }
            free(product_vectors);
            product_vectors = NULL;
        }

        if(delta_vectors)
        {
            for(i=0;i<number_of_hidden_layers+1;++i)
            {
                if(delta_vectors[i])
                {
                    vec_double_destroy(delta_vectors[i]);
                }
            }
            free(delta_vectors);
            delta_vectors = NULL;
        }

        if(theta_matrices)
        {
            for(i=0;i<number_of_hidden_layers+1;++i)
            {
                if(theta_matrices[i])
                {
                    mat_double_destroy(theta_matrices[i]);
                }
            }
            free(theta_matrices);
            theta_matrices = NULL;
        }

        if(tmp_theta_matrices)
        {
            for(i=0;i<number_of_hidden_layers+1;++i)
            {
                if(tmp_theta_matrices[i])
                {
                    mat_double_destroy(tmp_theta_matrices[i]);
                }
            }
            free(tmp_theta_matrices);
            tmp_theta_matrices = NULL;
        }
        return NULL;
}

void orange_neural_network_model_save(orange_neural_network_model* model, const char* filename)
{
}

void orange_neural_network_destroy(orange_neural_network* neural_network)
{
}

void orange_gd_options_destroy(orange_gd_options* options)
{
}


// Bobby - The ML Framework user

int after_iteration(uint64_t iteration_number,void* y,void* predicted_y,void* model,double regularization_parameter)
{
    printf("Progress Callback - i = %ld\n", iteration_number);
    return 0;
}

void load_training_examples(mat_double** X, mat_double** Y)
{

}

void set_gd_options(orange_gd_options* options)
{
    orange_gd_options_set_learning_rate(options, 0.000001);
    orange_gd_options_set_number_of_iterations(options, 500000);
    orange_gd_options_set_progress_callback(options, after_iteration);
    orange_gd_options_set_gradient_descent_type(options, ORANGE_BATCH_GRADIENT_DESCENT);
}

vec_double* sigmoid(vec_double* v)
{
return NULL;
}

vec_double* sigmoid_derivative(vec_double* v)
{
return NULL;
}

int main()
{
    orange_neural_network_model* model = NULL;
    dimension_t number_of_training_examples, number_of_features;
    dimension_t number_of_target_classes;

    mat_double *X, *Y;

    orange_neural_network* nn;
    nn = orange_neural_network_create_new(model); // model is NULL

    orange_gd_options* options = orange_gd_options_create_new();
    set_gd_options(options);

    orange_neural_network_set_input_layer(nn, number_of_features);
    orange_neural_network_add_hidden_layer(nn, 6, sigmoid, sigmoid_derivative);
    orange_neural_network_add_hidden_layer(nn, 7, sigmoid, sigmoid_derivative);
    orange_neural_network_add_hidden_layer(nn, 16, sigmoid, sigmoid_derivative);
    orange_neural_network_add_hidden_layer(nn, 32, sigmoid, sigmoid_derivative);
    orange_neural_network_add_hidden_layer(nn, 102, sigmoid, sigmoid_derivative);
    orange_neural_network_set_output_layer(nn, number_of_target_classes, sigmoid, sigmoid_derivative);

    model = orange_neural_network_logistic_batch_gd_fit(nn, X, Y, options, 0.235);

    orange_neural_network_model_save(model, "nn-model-1.dat");
    orange_neural_network_model_destroy(model);
    orange_neural_network_destroy(nn);
    orange_gd_options_destroy(options);
    mat_double_destroy(X);
    mat_double_destroy(Y);
    return 0;
}