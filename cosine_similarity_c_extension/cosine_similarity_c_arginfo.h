/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: ee4d16f0f88e2159752790a6ca5e38edfd5e7168 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_cosine_similarity_c, 0, 2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_cosine_similarity_c_direct arginfo_cosine_similarity_c

#define arginfo_cosine_similarity_c_sse arginfo_cosine_similarity_c

#define arginfo_cosine_similarity_c_opti arginfo_cosine_similarity_c


ZEND_FUNCTION(cosine_similarity_c);
ZEND_FUNCTION(cosine_similarity_c_direct);
ZEND_FUNCTION(cosine_similarity_c_sse);
ZEND_FUNCTION(cosine_similarity_c_opti);


static const zend_function_entry ext_functions[] = {
	ZEND_FE(cosine_similarity_c, arginfo_cosine_similarity_c)
	ZEND_FE(cosine_similarity_c_direct, arginfo_cosine_similarity_c_direct)
	ZEND_FE(cosine_similarity_c_sse, arginfo_cosine_similarity_c_sse)
	ZEND_FE(cosine_similarity_c_opti, arginfo_cosine_similarity_c_opti)
	ZEND_FE_END
};
