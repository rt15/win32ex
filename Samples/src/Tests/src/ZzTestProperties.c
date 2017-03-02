#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey)
{
  RT_CHAR* lpValue;

  lpValue = RtGetStringProperty(lpProperties, lpKey, RT_NULL);
  if (lpValue == RT_NULL)
  {
    lpValue = _R("NULL");
  }
  RtWriteStringsToConsole(4, lpKey, _R(" = \""), lpValue, _R("\"\n"));

  return RT_SUCCESS;
}

RT_B RT_CALL ZzTestIntegerProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_N nDefaultValue, RT_N nExpected)
{
  RT_N nValue;

  nValue = RtGetIntegerProperty(lpProperties, lpKey, nDefaultValue);

  return (nValue == nExpected);
}

RT_B RT_CALL ZzTestBooleanProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_B bDefaultValue, RT_B bExpected)
{
  RT_B bValue;
  RT_B bResult;

  bValue = RtGetBooleanProperty(lpProperties, lpKey, bDefaultValue);
  bResult = ((bValue && bExpected) || ((!bValue) && (!bExpected)));

  return bResult;
}

RT_B RT_CALL ZzTestProperties(RT_HEAP** lpHeap)
{
  RT_B bPropertiesCreated;
  RT_PROPERTIES rtProperties;
  RT_B bResult;

  bPropertiesCreated = RT_FALSE;

  if (!RtCreateProperties(&rtProperties, _R("data/properties.txt"), RT_ENCODING_ISO_8859_1, lpHeap)) goto handle_error;
  bPropertiesCreated = RT_TRUE;

  if (!ZzTestProperty(&rtProperties, _R("my_key"))) goto handle_error;
  if (!ZzTestProperty(&rtProperties, _R("my_other_key"))) goto handle_error;
  if (!ZzTestProperty(&rtProperties, _R("my_wrong_key"))) goto handle_error;
  if (!ZzTestProperty(&rtProperties, _R(""))) goto handle_error;
  if (!ZzTestProperty(&rtProperties, _R("key_with_empty_value"))) goto handle_error;
  if (!ZzTestProperty(&rtProperties, _R("key_without_value"))) goto handle_error;

  if (!ZzTestIntegerProperty(&rtProperties, _R("key_with_number"), 5, 12)) goto handle_error;
  if (!ZzTestIntegerProperty(&rtProperties, _R("key_with_zero"), 5, 0)) goto handle_error;
  if (!ZzTestIntegerProperty(&rtProperties, _R("wrong_number_key"), 5, 5)) goto handle_error;

  if (!ZzTestBooleanProperty(&rtProperties, _R("key_with_true"), RT_FALSE, RT_TRUE)) goto handle_error;
  if (!ZzTestBooleanProperty(&rtProperties, _R("key_with_false"), RT_TRUE, RT_FALSE)) goto handle_error;
  if (!ZzTestBooleanProperty(&rtProperties, _R("wrong_boolean_key"), RT_FALSE, RT_FALSE)) goto handle_error;
  if (!ZzTestBooleanProperty(&rtProperties, _R("wrong_boolean_key"), RT_TRUE, RT_TRUE)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bPropertiesCreated)
  {
    bPropertiesCreated = RT_FALSE;
    if (!RtFreeProperties(&rtProperties) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
