/* cosine_similarity_c extension for PHP */

// https://ashvardanian.com/posts/python-c-assembly-comparison/#c

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_cosine_similarity_c.h"
#include "cosine_similarity_c_arginfo.h"


#include <xmmintrin.h>  // SSE extensions
#include <emmintrin.h>  // SSE2 extensions
#include <smmintrin.h>  // SSE4.1 extensions
#include <immintrin.h>  // Include for AVX

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif


__attribute__((optimize("O3"))) static double cosine_similarity_impl_opti(const double *array1, const double *array2, int size) {
   double dot_product = 0.0;
   double magnitude_a = 0.0;
   double magnitude_b = 0.0;
   int i;

   for (i = 0; i < size; ++i) {
       dot_product += array1[i] * array2[i];
       magnitude_a += array1[i] * array1[i];
       magnitude_b += array2[i] * array2[i];
   }

   return dot_product / (sqrt(magnitude_a) * sqrt(magnitude_b));
}

static double cosine_similarity_impl(const double *array1, const double *array2, int size) {
   double dot_product = 0.0;
   double magnitude_a = 0.0;
   double magnitude_b = 0.0;
   int i;

   for (i = 0; i < size; ++i) {
       dot_product += array1[i] * array2[i];
       magnitude_a += array1[i] * array1[i];
       magnitude_b += array2[i] * array2[i];
   }

   return dot_product / (sqrt(magnitude_a) * sqrt(magnitude_b));
}

static double cosine_similarity_impl_sse2(const double *array1, const double *array2, int size) {
    __m128d sum_dot = _mm_setzero_pd();
    __m128d sum_mag_a = _mm_setzero_pd();
    __m128d sum_mag_b = _mm_setzero_pd();

    for (int i = 0; i < size; i += 2) {
        __m128d a = _mm_loadu_pd(&array1[i]);
        __m128d b = _mm_loadu_pd(&array2[i]);

        sum_dot = _mm_add_pd(sum_dot, _mm_mul_pd(a, b));
        sum_mag_a = _mm_add_pd(sum_mag_a, _mm_mul_pd(a, a));
        sum_mag_b = _mm_add_pd(sum_mag_b, _mm_mul_pd(b, b));
    }

    // Horizontal add for final reduction
    sum_dot = _mm_hadd_pd(sum_dot, sum_dot);
    sum_mag_a = _mm_hadd_pd(sum_mag_a, sum_mag_a);
    sum_mag_b = _mm_hadd_pd(sum_mag_b, sum_mag_b);

    double dot_product, magnitude_a, magnitude_b;
    _mm_store_sd(&dot_product, sum_dot);
    _mm_store_sd(&magnitude_a, sum_mag_a);
    _mm_store_sd(&magnitude_b, sum_mag_b);

    return dot_product / (sqrt(magnitude_a) * sqrt(magnitude_b));
}

static double cosine_similarity_impl_sse(const double *array1, const double *array2, int size) {
    __m128d sum_dot = _mm_setzero_pd();
    __m128d sum_mag_a = _mm_setzero_pd();
    __m128d sum_mag_b = _mm_setzero_pd();

    int i;
    for (i = 0; i <= size - 2; i += 2) {  // Process pairs of elements
        __m128d a = _mm_loadu_pd(&array1[i]);
        __m128d b = _mm_loadu_pd(&array2[i]);

        sum_dot = _mm_add_pd(sum_dot, _mm_mul_pd(a, b));
        sum_mag_a = _mm_add_pd(sum_mag_a, _mm_mul_pd(a, a));
        sum_mag_b = _mm_add_pd(sum_mag_b, _mm_mul_pd(b, b));
    }

    // Handle the last element if size is odd
    if (size % 2 != 0) {
        __m128d a = _mm_set_sd(array1[i]);
        __m128d b = _mm_set_sd(array2[i]);

        sum_dot = _mm_add_sd(sum_dot, _mm_mul_sd(a, b));
        sum_mag_a = _mm_add_sd(sum_mag_a, _mm_mul_sd(a, a));
        sum_mag_b = _mm_add_sd(sum_mag_b, _mm_mul_sd(b, b));
    }

    // Final reduction
    double dot_product[2], magnitude_a[2], magnitude_b[2];
    _mm_storeu_pd(dot_product, sum_dot);
    _mm_storeu_pd(magnitude_a, sum_mag_a);
    _mm_storeu_pd(magnitude_b, sum_mag_b);

    double final_dot = dot_product[0] + dot_product[1];
    double final_mag_a = magnitude_a[0] + magnitude_a[1];
    double final_mag_b = magnitude_b[0] + magnitude_b[1];

    return final_dot / (sqrt(final_mag_a) * sqrt(final_mag_b));
}
/*
static double cosine_similarity_impl_avx(const double *array1, const double *array2, int size) {
    __m256d sum_dot = _mm256_setzero_pd();
    __m256d sum_mag_a = _mm256_setzero_pd();
    __m256d sum_mag_b = _mm256_setzero_pd();

    int i;
    for (i = 0; i <= size - 4; i += 4) {  // Process quads of elements
        __m256d a = _mm256_loadu_pd(&array1[i]);
        __m256d b = _mm256_loadu_pd(&array2[i]);

        sum_dot = _mm256_add_pd(sum_dot, _mm256_mul_pd(a, b));
        sum_mag_a = _mm256_add_pd(sum_mag_a, _mm256_mul_pd(a, a));
        sum_mag_b = _mm256_add_pd(sum_mag_b, _mm256_mul_pd(b, b));
    }

    // After processing blocks of 4 elements with AVX
    double tail_dot = 0.0, tail_mag_a = 0.0, tail_mag_b = 0.0;
    for (; i < size; ++i) {
        double a = array1[i];
        double b = array2[i];
        tail_dot += a * b;
        tail_mag_a += a * a;
        tail_mag_b += b * b;
    }

    // Integrate scalar results with AVX results
    // Convert scalar results to __m256d before adding to ensure type compatibility
    __m256d tail_dot_avx = _mm256_set1_pd(tail_dot);
    __m256d tail_mag_a_avx = _mm256_set1_pd(tail_mag_a);
    __m256d tail_mag_b_avx = _mm256_set1_pd(tail_mag_b);

    sum_dot = _mm256_add_pd(sum_dot, tail_dot_avx);
    sum_mag_a = _mm256_add_pd(sum_mag_a, tail_mag_a_avx);
    sum_mag_b = _mm256_add_pd(sum_mag_b, tail_mag_b_avx);

    // Final reduction
    double dot_product[4], magnitude_a[4], magnitude_b[4];
    _mm256_storeu_pd(dot_product, sum_dot);
    _mm256_storeu_pd(magnitude_a, sum_mag_a);
    _mm256_storeu_pd(magnitude_b, sum_mag_b);

    double final_dot = dot_product[0] + dot_product[1] + dot_product[2] + dot_product[3];
    double final_mag_a = magnitude_a[0] + magnitude_a[1] + magnitude_a[2] + magnitude_a[3];
    double final_mag_b = magnitude_b[0] + magnitude_b[1] + magnitude_b[2] + magnitude_b[3];

    return final_dot / (sqrt(final_mag_a) * sqrt(final_mag_b));
}
*/
PHP_FUNCTION(cosine_similarity_c) {
    zval *array1, *array2;
    double *arr1, *arr2;
    int size1, size2, i;
    double result;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "aa", &array1, &array2) == FAILURE) {
        return;
    }

    size1 = zend_array_count(Z_ARRVAL_P(array1));
    size2 = zend_array_count(Z_ARRVAL_P(array2));

    // Ensure arrays are of the same size
    if (size1 != size2) {
        php_error_docref(NULL, E_WARNING, "Arrays must be of the same size");
        RETURN_FALSE;
    }

    arr1 = (double *)emalloc(size1 * sizeof(double));
    arr2 = (double *)emalloc(size2 * sizeof(double));

    // Convert PHP arrays to C arrays
    HashTable *arr_hash1 = Z_ARRVAL_P(array1);
    HashTable *arr_hash2 = Z_ARRVAL_P(array2);
    zval *val1, *val2;

    for (i = 0, zend_hash_internal_pointer_reset(arr_hash1), zend_hash_internal_pointer_reset(arr_hash2);
         (val1 = zend_hash_get_current_data(arr_hash1)) != NULL && (val2 = zend_hash_get_current_data(arr_hash2)) != NULL;
         zend_hash_move_forward(arr_hash1), zend_hash_move_forward(arr_hash2), i++) {
        arr1[i] = zval_get_double(val1);
        arr2[i] = zval_get_double(val2);
    }

    result = cosine_similarity_impl(arr1, arr2, size1);

    efree(arr1);
    efree(arr2);

    RETURN_DOUBLE(result);
}

PHP_FUNCTION(cosine_similarity_c_direct) {
    zval *array1, *array2;
    int size1, size2, i;
    double result;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "aa", &array1, &array2) == FAILURE) {
        return;
    }

    size1 = zend_array_count(Z_ARRVAL_P(array1));
    size2 = zend_array_count(Z_ARRVAL_P(array2));

    // Ensure arrays are of the same size
    if (size1 != size2) {
        php_error_docref(NULL, E_WARNING, "Arrays must be of the same size");
        RETURN_FALSE;
    }

    HashTable *arr_hash1 = Z_ARRVAL_P(array1);
    HashTable *arr_hash2 = Z_ARRVAL_P(array2);
    zval *val1, *val2;

    double dot_product = 0.0;
    double magnitude_a = 0.0;
    double magnitude_b = 0.0;

    for (i = 0; i < size1; i++) {
        val1 = zend_hash_index_find(arr_hash1, i);
        val2 = zend_hash_index_find(arr_hash2, i);
        if (val1 && val2) {
            double elem1 = zval_get_double(val1);
            double elem2 = zval_get_double(val2);
            dot_product += elem1 * elem2;
            magnitude_a += elem1 * elem1;
            magnitude_b += elem2 * elem2;
        } else {
            php_error_docref(NULL, E_WARNING, "Arrays must be of the same size");
            RETURN_FALSE;
        }
    }

    result = dot_product / (sqrt(magnitude_a) * sqrt(magnitude_b));

    RETURN_DOUBLE(result);
}



PHP_FUNCTION(cosine_similarity_c_sse) {
    zval *array1, *array2;
    double *arr1, *arr2;
    int size1, size2, i;
    double result;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "aa", &array1, &array2) == FAILURE) {
        return;
    }

    size1 = zend_array_count(Z_ARRVAL_P(array1));
    size2 = zend_array_count(Z_ARRVAL_P(array2));

    // Ensure arrays are of the same size
    if (size1 != size2) {
        php_error_docref(NULL, E_WARNING, "Arrays must be of the same size");
        RETURN_FALSE;
    }

    arr1 = (double *)emalloc(size1 * sizeof(double));
    arr2 = (double *)emalloc(size2 * sizeof(double));

    // Convert PHP arrays to C arrays
    HashTable *arr_hash1 = Z_ARRVAL_P(array1);
    HashTable *arr_hash2 = Z_ARRVAL_P(array2);
    zval *val1, *val2;

    for (i = 0, zend_hash_internal_pointer_reset(arr_hash1), zend_hash_internal_pointer_reset(arr_hash2);
         (val1 = zend_hash_get_current_data(arr_hash1)) != NULL && (val2 = zend_hash_get_current_data(arr_hash2)) != NULL;
         zend_hash_move_forward(arr_hash1), zend_hash_move_forward(arr_hash2), i++) {
        arr1[i] = zval_get_double(val1);
        arr2[i] = zval_get_double(val2);
    }

    result = cosine_similarity_impl_sse(arr1, arr2, size1);

    efree(arr1);
    efree(arr2);

    RETURN_DOUBLE(result);
}

/*
PHP_FUNCTION(cosine_similarity_c_avx) {
    zval *array1, *array2;
    double *arr1, *arr2;
    int size1, size2, i;
    double result;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "aa", &array1, &array2) == FAILURE) {
        return;
    }

    size1 = zend_array_count(Z_ARRVAL_P(array1));
    size2 = zend_array_count(Z_ARRVAL_P(array2));

    // Ensure arrays are of the same size
    if (size1 != size2) {
        php_error_docref(NULL, E_WARNING, "Arrays must be of the same size");
        RETURN_FALSE;
    }

    arr1 = (double *)emalloc(size1 * sizeof(double));
    arr2 = (double *)emalloc(size2 * sizeof(double));

    // Convert PHP arrays to C arrays
    HashTable *arr_hash1 = Z_ARRVAL_P(array1);
    HashTable *arr_hash2 = Z_ARRVAL_P(array2);
    zval *val1, *val2;

    for (i = 0, zend_hash_internal_pointer_reset(arr_hash1), zend_hash_internal_pointer_reset(arr_hash2);
         (val1 = zend_hash_get_current_data(arr_hash1)) != NULL && (val2 = zend_hash_get_current_data(arr_hash2)) != NULL;
         zend_hash_move_forward(arr_hash1), zend_hash_move_forward(arr_hash2), i++) {
        arr1[i] = zval_get_double(val1);
        arr2[i] = zval_get_double(val2);
    }

    result = cosine_similarity_impl_avx(arr1, arr2, size1);

    efree(arr1);
    efree(arr2);

    RETURN_DOUBLE(result);
}
*/
PHP_FUNCTION(cosine_similarity_c_opti) {
    zval *array1, *array2;
    double *arr1, *arr2;
    int size1, size2, i;
    double result;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "aa", &array1, &array2) == FAILURE) {
        return;
    }

    size1 = zend_array_count(Z_ARRVAL_P(array1));
    size2 = zend_array_count(Z_ARRVAL_P(array2));

    // Ensure arrays are of the same size
    if (size1 != size2) {
        php_error_docref(NULL, E_WARNING, "Arrays must be of the same size");
        RETURN_FALSE;
    }

    arr1 = (double *)emalloc(size1 * sizeof(double));
    arr2 = (double *)emalloc(size2 * sizeof(double));

    // Convert PHP arrays to C arrays
    HashTable *arr_hash1 = Z_ARRVAL_P(array1);
    HashTable *arr_hash2 = Z_ARRVAL_P(array2);
    zval *val1, *val2;

    for (i = 0, zend_hash_internal_pointer_reset(arr_hash1), zend_hash_internal_pointer_reset(arr_hash2);
         (val1 = zend_hash_get_current_data(arr_hash1)) != NULL && (val2 = zend_hash_get_current_data(arr_hash2)) != NULL;
         zend_hash_move_forward(arr_hash1), zend_hash_move_forward(arr_hash2), i++) {
        arr1[i] = zval_get_double(val1);
        arr2[i] = zval_get_double(val2);
    }

    result = cosine_similarity_impl_opti(arr1, arr2, size1);

    efree(arr1);
    efree(arr2);

    RETURN_DOUBLE(result);
}

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(cosine_similarity_c)
{
#if defined(ZTS) && defined(COMPILE_DL_COSINE_SIMILARITY_C)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(cosine_similarity_c)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "cosine_similarity_c support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ cosine_similarity_c_module_entry */
zend_module_entry cosine_similarity_c_module_entry = {
	STANDARD_MODULE_HEADER,
	"cosine_similarity_c",					/* Extension name */
	ext_functions,					/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(cosine_similarity_c),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(cosine_similarity_c),			/* PHP_MINFO - Module info */
	PHP_COSINE_SIMILARITY_C_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_COSINE_SIMILARITY_C
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(cosine_similarity_c)
#endif
