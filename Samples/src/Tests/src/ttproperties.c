#include <RtWin32Ex.h>

RT_UN16 RT_CALL TtTestProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey)
{
  RT_CHAR* lpValue;

  lpValue = RtGetStringProperty(lpProperties, lpKey, RT_NULL);
  if (lpValue == RT_NULL)
  {
    lpValue = _R("NULL");
  }
  RtWriteStringsToConsole(4, lpKey, _R(" = \""), lpValue, _R("\"\n"));
  return 0;
}

RT_UN16 RT_CALL TtTestNumberProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_N nDefaultValue, RT_N nExpected)
{
  RT_N nValue;
  RT_UN16 unResult;

  nValue = RtGetNumberProperty(lpProperties, lpKey, nDefaultValue);
  unResult = (nValue == nExpected) ? 0 : 1;

  return unResult;
}

RT_UN16 RT_CALL TtTestBooleanProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_B bDefaultValue, RT_B bExpected)
{
  RT_B bValue;
  RT_UN16 unResult;

  bValue = RtGetBooleanProperty(lpProperties, lpKey, bDefaultValue);
  unResult = ((bValue && bExpected) || ((!bValue) && (!bExpected))) ? 0 : 1;

  return unResult;
}

RT_UN16 RT_CALL TtTestProperties(RT_HEAP** lpHeap)
{
  RT_PROPERTIES properties;
  RT_UN16 unResult;

  unResult = 1;

  if (!RtCreateProperties(&properties, _R("data/properties.txt"), RT_ENCODING_ISO_8859_1, lpHeap))
  {
    goto the_end;
  }

  if (TtTestProperty(&properties, _R("my_key"))) goto the_end;
  if (TtTestProperty(&properties, _R("my_other_key"))) goto the_end;
  if (TtTestProperty(&properties, _R("my_wrong_key"))) goto the_end;
  if (TtTestProperty(&properties, _R(""))) goto the_end;
  if (TtTestProperty(&properties, _R("key_with_empty_value"))) goto the_end;
  if (TtTestProperty(&properties, _R("key_without_value"))) goto the_end;

  if (TtTestNumberProperty(&properties, _R("key_with_number"), 5, 12)) goto the_end;
  if (TtTestNumberProperty(&properties, _R("key_with_zero"), 5, 0)) goto the_end;
  if (TtTestNumberProperty(&properties, _R("wrong_number_key"), 5, 5)) goto the_end;

  if (TtTestBooleanProperty(&properties, _R("key_with_true"), RT_FALSE, RT_TRUE)) goto the_end;
  if (TtTestBooleanProperty(&properties, _R("key_with_false"), RT_TRUE, RT_FALSE)) goto the_end;
  if (TtTestBooleanProperty(&properties, _R("wrong_boolean_key"), RT_FALSE, RT_FALSE)) goto the_end;
  if (TtTestBooleanProperty(&properties, _R("wrong_boolean_key"), RT_TRUE, RT_TRUE)) goto the_end;

  if (!RtFreeProperties(&properties)) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}
