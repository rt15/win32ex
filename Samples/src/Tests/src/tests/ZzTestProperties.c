#include <rpr.h>

rt_s RT_CALL ZzTestProperty(RT_PROPERTIES *lpProperties, rt_char *lpKey)
{
	rt_char *value;

	value = rt_properties_GetString(lpProperties, lpKey, RT_NULL);
	if (value == RT_NULL) {
		value = _R("NULL");
	}
	rt_console_WriteStringsOrErrors(RT_TRUE, lpKey, _R(" = \""), value, _R("\"\n"), (rt_char*)RT_NULL);

	return RT_OK;
}

rt_s RT_CALL ZzTestIntegerProperty(RT_PROPERTIES *lpProperties, rt_char *lpKey, rt_un unDefaultValue, rt_un expected)
{
	rt_n value;

	value = rt_properties_GetUnsignedInteger(lpProperties, lpKey, unDefaultValue);

	return (value == expected);
}

rt_s RT_CALL ZzTestBooleanProperty(RT_PROPERTIES *lpProperties, rt_char *lpKey, rt_b bDefaultValue, rt_b bExpected)
{
	rt_b value;
	rt_s ret;

	value = rt_properties_GetBoolean(lpProperties, lpKey, bDefaultValue);
	ret = ((value && bExpected) || ((!value) && (!bExpected)));

	return ret;
}

rt_s RT_CALL ZzTestProperties(struct rt_heap **heap)
{
	rt_b bPropertiesCreated;
	RT_PROPERTIES zzProperties;
	rt_s ret;

	bPropertiesCreated = RT_FALSE;

	if (!rt_properties_Create(&zzProperties, _R("data/properties.txt"), RT_ENCODING_ISO_8859_1, heap)) goto error;
	bPropertiesCreated = RT_TRUE;

	if (!ZzTestProperty(&zzProperties, _R("my_key"))) goto error;
	if (!ZzTestProperty(&zzProperties, _R("my_other_key"))) goto error;
	if (!ZzTestProperty(&zzProperties, _R("my_wrong_key"))) goto error;
	if (!ZzTestProperty(&zzProperties, _R(""))) goto error;
	if (!ZzTestProperty(&zzProperties, _R("key_with_empty_value"))) goto error;
	if (!ZzTestProperty(&zzProperties, _R("key_without_value"))) goto error;

	if (!ZzTestIntegerProperty(&zzProperties, _R("key_with_number"), 5, 12)) goto error;
	if (!ZzTestIntegerProperty(&zzProperties, _R("key_with_zero"), 5, 0)) goto error;
	if (!ZzTestIntegerProperty(&zzProperties, _R("wrong_number_key"), 5, 5)) goto error;

	if (!ZzTestBooleanProperty(&zzProperties, _R("key_with_true"), RT_FALSE, RT_TRUE)) goto error;
	if (!ZzTestBooleanProperty(&zzProperties, _R("key_with_false"), RT_TRUE, RT_FALSE)) goto error;
	if (!ZzTestBooleanProperty(&zzProperties, _R("wrong_boolean_key"), RT_FALSE, RT_FALSE)) goto error;
	if (!ZzTestBooleanProperty(&zzProperties, _R("wrong_boolean_key"), RT_TRUE, RT_TRUE)) goto error;

	ret = RT_OK;
free:
	if (bPropertiesCreated) {
		bPropertiesCreated = RT_FALSE;
		if (!rt_properties_Free(&zzProperties) && ret)
			goto error;
	}
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}
