/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: ecb26c4ae91eb4af13864e9441c90a2a2f62298d */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_cosine_similarity_c, 0, 2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_cosine_similarity_c_sse arginfo_cosine_similarity_c


ZEND_FUNCTION(cosine_similarity_c);
ZEND_FUNCTION(cosine_similarity_c_sse);


static const zend_function_entry ext_functions[] = {
	ZEND_FE(cosine_similarity_c, arginfo_cosine_similarity_c)
	ZEND_FE(cosine_similarity_c_sse, arginfo_cosine_similarity_c_sse)
	ZEND_FE_END
};
