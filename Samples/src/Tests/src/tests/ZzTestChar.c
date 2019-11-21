#include <RtWin32Ex.h>

#define ZZ_BIG_STRING_SIZE 7000
#define ZZ_STRING_SIZE_COUNT 2
RT_CHAR* zz_StringSizeStrings[] = { _R("Foo Bar."), _R("") };
RT_N zz_StringSizeSizes[] =       { 8        ,      0      };

RT_B RT_CALL ZzTestGetStringSize()
{
  RT_ARRAY zzString;
  RT_CHAR lpString[256];
  RT_ARRAY zzMessage;
  RT_CHAR* lpBigString;
  RT_UN unI;
  RT_B bResult;

  lpBigString = RT_NULL;

  /* Testing various strings sizes. */
  for (unI = 0; unI < ZZ_STRING_SIZE_COUNT; unI++)
  {
     if (RtChar_GetCStringSize(zz_StringSizeStrings[unI]) != zz_StringSizeSizes[unI]) goto handle_error;
     RtChar_CreateString(&zzString, zz_StringSizeStrings[unI]);
     if (RtChar_GetStringSize(&zzString) != zz_StringSizeSizes[unI]) goto handle_error;
  }

  /* Checking conversion to CString appending a zero. */
  RtArray_Create(&zzString, lpString, sizeof(RT_CHAR), 256);
  for (unI = 0; unI < 256; unI++)
  {
    lpString[unI] = _R('b');
  }
  if (!RtChar_AppendCString(&zzString, _R("aaaaaaaa"))) goto handle_error;
  if (!RtChar_Append(&zzString, 0)) goto handle_error;
  if (RtChar_GetStringSize(&zzString) != 9) goto handle_error;
  if (RtChar_GetCStringSize(lpString) != 8) goto handle_error;

  RtConsole_WriteString(RtChar_CreateString(&zzMessage, _R("Testing big string size...\n")));

  /* Allocate some memory. */
  if (!RtStaticHeap_Alloc((void**)&lpBigString, ZZ_BIG_STRING_SIZE * sizeof(RT_CHAR)))
  {
    goto handle_error;
  }

  /* Fill the allocated memory with some 'a'. */
  for (unI = 0; unI < ZZ_BIG_STRING_SIZE - 1; unI++)
  {
    lpBigString[unI] = _R('a');
  }
  lpBigString[ZZ_BIG_STRING_SIZE - 1] = 0;

  /* Check the string size. */
  if (RtChar_GetCStringSize(lpBigString) != ZZ_BIG_STRING_SIZE - 1) goto handle_error;
  RtChar_CreateString(&zzString, lpBigString);
  if (RtChar_GetStringSize(&zzString) != ZZ_BIG_STRING_SIZE - 1) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (lpBigString)
  {
    if (!RtStaticHeap_Free((void**)&lpBigString) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_N RT_CALL ZzPerformComparison(RT_CHAR* lpString1, RT_CHAR* lpString2)
{
  RT_ARRAY zzString1;
  RT_ARRAY zzString2;

  RtChar_CreateString(&zzString1, lpString1);
  RtChar_CreateString(&zzString2, lpString2);

  return RtChar_CompareStrings(&zzString1, &zzString2);
}

RT_B RT_CALL ZzTestCompareStrings()
{
  RT_B bResult;

  RtConsole_WriteCString(_R("Testing RtChar_CompareStrings...\n"));

  if (ZzPerformComparison(_R("Foo"), _R("Foo")) != 0) goto handle_error;
  if (ZzPerformComparison(_R(" "), _R(" ")) != 0) goto handle_error;
  if (ZzPerformComparison(_R("Foo"), _R("Bar")) <= 0) goto handle_error;
  if (ZzPerformComparison(_R("Bar"), _R("Foo")) >= 0) goto handle_error;
  if (ZzPerformComparison(_R(" "), _R("a")) >= 0) goto handle_error;
  if (ZzPerformComparison(_R("a"), _R(" ")) <= 0) goto handle_error;
  if (ZzPerformComparison(_R("e"), _R("é")) >= 0) goto handle_error;
  if (ZzPerformComparison(_R("é"), _R("e")) <= 0) goto handle_error;
  if (ZzPerformComparison(_R("a"), _R("ab")) >= 0) goto handle_error;
  if (ZzPerformComparison(_R("ab"), _R("a")) <= 0) goto handle_error;
  if (ZzPerformComparison(_R(""), _R("a")) >= 0) goto handle_error;
  if (ZzPerformComparison(_R("a"), _R("")) <= 0) goto handle_error;

  if (RtChar_CompareStringsWithSize(_R("Fooa"), _R("Foob"), 1) != 0) goto handle_error;
  if (RtChar_CompareStringsWithSize(_R("Fooa"), _R("Foob"), 2) != 0) goto handle_error;
  if (RtChar_CompareStringsWithSize(_R("Fooa"), _R("Foob"), 3) != 0) goto handle_error;
  if (RtChar_CompareStringsWithSize(_R("Fooa"), _R("Foob"), 4) >= 0) goto handle_error;
  if (RtChar_CompareStringsWithSize(_R("Foob"), _R("Fooa"), 4) <= 0) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestLeftPadString(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_CHAR* lpInput, RT_CHAR nChar, RT_UN unSize, RT_UN unReference, RT_CHAR* lpReference)
{
  RT_ARRAY zzBuffer;
  RT_ARRAY rtMessage;
  RT_CHAR lpMessageBuffer[256];
  RT_ARRAY rtConversionBuffer;
  RT_CHAR lpConversionBuffer[64];
  RT_UN unOutputSize;
  RT_B bResult;

  RtArray_Create(&rtMessage, lpMessageBuffer, sizeof(RT_CHAR), 256);
  RtArray_Create(&rtConversionBuffer, lpConversionBuffer, sizeof(RT_CHAR), 64);

  /* Input. */
  if (!RtChar_AppendCString(&rtMessage, _R("Testing RtChar_LeftPadString with input string = \""))) goto handle_error;
  if (!RtChar_AppendCString(&rtMessage, lpInput)) goto handle_error;

  /* Buffer size. */
  if (!RtChar_AppendCString(&rtMessage, _R("\", buffer size = "))) goto handle_error;
  if (!RtChar_ConvertUnsignedIntegerToString(unBufferSize, &rtConversionBuffer)) goto handle_error;
  if (!RtArray_Append(&rtMessage, &rtConversionBuffer)) goto handle_error;

  /* Size. */
  if (!RtChar_AppendCString(&rtMessage, _R(" and size = "))) goto handle_error;
  if (!RtChar_ConvertUnsignedIntegerToString(unSize, &rtConversionBuffer)) goto handle_error;
  if (!RtArray_Append(&rtMessage, &rtConversionBuffer)) goto handle_error;

  if (!RtChar_AppendCString(&rtMessage, _R("...\n"))) goto handle_error;
  if (!RtConsole_WriteString(&rtMessage)) goto handle_error;

  RtChar_LeftPadString(lpInput, RtChar_GetCStringSize(lpInput), nChar, unSize, lpBuffer, unBufferSize, &unOutputSize);

 /* Check function result. */
  if (unOutputSize != unReference) goto handle_error;

  /* If something has been written to the buffer. */
  if (unReference > 0)
  {
    RtChar_CreateString(&zzBuffer, lpBuffer);
    if (!RtChar_StringEqualsCString(&zzBuffer, lpReference))
    {
      RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Resulting string = \""), lpBuffer, _R("\" is different from reference = \""), lpReference, _R("\".\n"), (RT_CHAR*)RT_NULL);
      goto handle_error;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestLeftPadString()
{
  RT_CHAR lpBuffer[200];
  RT_B bResult;

  if (!ZzDoTestLeftPadString(lpBuffer, 200, _R("Foo"), _R('O'), 6, 6, _R("OOOFoo"))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestRightTrimString(RT_CHAR* lpInput, RT_UN unReference, RT_CHAR* lpReference)
{
  RT_ARRAY zzLocalBuffer;
  RT_CHAR lpLocalBuffer[200];
  RT_UN unInputSize;
  RT_UN unOutputSize;
  RT_B bResult;

  RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Testing RtChar_RightTrimString with input string = \""), lpInput, _R("\"...\n"), (RT_CHAR*)RT_NULL);

  unInputSize = 0;
  RtChar_CopyString(lpInput, lpLocalBuffer, 200, &unInputSize);

  RtChar_RightTrimString(lpLocalBuffer, unInputSize, &unOutputSize);

  if (unOutputSize != unReference) goto handle_error;

  RtChar_CreateString(&zzLocalBuffer, lpLocalBuffer);
  if (!RtChar_StringEqualsCString(&zzLocalBuffer, lpReference))
  {
    RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Resulting string = \""), lpLocalBuffer, _R("\" is different from reference = \""), lpReference, _R("\".\n"), (RT_CHAR*)RT_NULL);
    goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestRightTrimString()
{
  RT_B bResult;

  if (!ZzDoTestRightTrimString(_R("foo"), 3, _R("foo"))) goto handle_error;
  if (!ZzDoTestRightTrimString(_R("foo "), 3, _R("foo"))) goto handle_error;
  if (!ZzDoTestRightTrimString(_R("foo  "), 3, _R("foo"))) goto handle_error;
  if (!ZzDoTestRightTrimString(_R(""), 0, _R(""))) goto handle_error;
  if (!ZzDoTestRightTrimString(_R(" "), 0, _R(""))) goto handle_error;
  if (!ZzDoTestRightTrimString(_R("  "), 0, _R(""))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestCountStringOccurrences(RT_CHAR* lpString, RT_CHAR* lpSearched, RT_UN unExpected)
{
  RT_UN unFound;
  RT_B bResult;

  RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Testing RtChar_CountStringOccurrences, occurrences count of \""), lpSearched, _R("\" in \""), lpString, _R("\"...\n"), (RT_CHAR*)RT_NULL);
  unFound = RtChar_CountStringOccurrences(lpString, lpSearched);
  if (unFound != unExpected) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestCountStringOccurrences()
{
  RT_B bResult;

  if (!ZzDoTestCountStringOccurrences(_R("foo"), _R("foo"), 1)) goto handle_error;
  if (!ZzDoTestCountStringOccurrences(_R("foofoo"), _R("foo"), 2)) goto handle_error;
  if (!ZzDoTestCountStringOccurrences(_R("ofoofoo"), _R("o"), 5)) goto handle_error;
  if (!ZzDoTestCountStringOccurrences(_R("foof"), _R("o"), 2)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestReplaceString(RT_CHAR* lpString, RT_CHAR* lpSearched, RT_CHAR* lpReplacement, RT_CHAR* lpReference)
{
  RT_ARRAY zzBuffer;
  RT_CHAR lpBuffer[64];
  RT_UN unStringSize;
  RT_UN unSearchedSize;
  RT_UN unReplacementSize;
  RT_UN unOutputSize;
  RT_B bResult;

  unStringSize = RtChar_GetCStringSize(lpString);
  unSearchedSize = RtChar_GetCStringSize(lpSearched);
  unReplacementSize = RtChar_GetCStringSize(lpReplacement);

  RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Replacing \""), lpSearched, _R("\" by \""), lpReplacement, _R("\" in \""), lpString, _R("\".\n"), (RT_CHAR*)RT_NULL);
  if (!RtChar_ReplaceString(lpString, unStringSize,
                            lpSearched, unSearchedSize,
                            lpReplacement, unReplacementSize,
                            lpBuffer, 64, &unOutputSize)) goto handle_error;

  if (unOutputSize != RtChar_GetCStringSize(lpBuffer)) goto handle_error;

  RtChar_CreateString(&zzBuffer, lpBuffer);
  if (!RtChar_StringEqualsCString(&zzBuffer, lpReference))
  {
    RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Wrong value \""), lpBuffer, _R("\" expected \""), lpReference, _R("\".\n"), (RT_CHAR*)RT_NULL);
    goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestReplaceString()
{
  RT_B bResult;

  if (!ZzDoTestReplaceString(_R("foo"), _R("foo"), _R("bar"), _R("bar"))) goto handle_error;
  if (!ZzDoTestReplaceString(_R("foo"), _R("o"), _R("a"), _R("faa"))) goto handle_error;
  if (!ZzDoTestReplaceString(_R("foobarfoo"), _R("foo"), _R("a"), _R("abara"))) goto handle_error;
  if (!ZzDoTestReplaceString(_R("foobarfoo"), _R("foo"), _R("long_string"), _R("long_stringbarlong_string"))) goto handle_error;
  if (!ZzDoTestReplaceString(_R("foobarfoo"), _R("foo"), _R(""), _R("bar"))) goto handle_error;
  if (!ZzDoTestReplaceString(_R("foofoo"), _R("foo"), _R(""), _R(""))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestSearchStringInStrings()
{
  RT_CHAR* lpStrings[4];
  RT_B bResult;

  lpStrings[0] = _R("foo");
  lpStrings[1] = _R("bar");
  lpStrings[2] = _R("team");
  lpStrings[3] = RT_NULL;

  if (RtChar_SearchStringInStrings(lpStrings, _R("TEAM")) != RT_TYPE_MAX_UN) goto handle_error;
  if (RtChar_SearchStringInStrings(lpStrings, _R("team")) != 2) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestAppend(RT_CHAR* lpString1, RT_CHAR* lpString2, RT_CHAR* lpExpected)
{
  RT_ARRAY rtString;
  RT_ARRAY rtString1;
  RT_ARRAY rtString2;
  RT_ARRAY rtExpected;
  RT_CHAR lpBuffer[RT_CHAR_QUARTER_BIG_STRING_SIZE];
  RT_B bResult;

  /* Test RtArray_Append. */
  RtArray_Create(&rtString, lpBuffer, sizeof(RT_CHAR), RT_CHAR_QUARTER_BIG_STRING_SIZE);
  if (!RtArray_Append(&rtString, RtChar_CreateString(&rtString1, lpString1))) goto handle_error;
  if (!RtArray_Append(&rtString, RtChar_CreateString(&rtString2, lpString2))) goto handle_error;
  if (!RtChar_StringEqualsString(&rtString, RtChar_CreateString(&rtExpected, lpExpected))) goto handle_error;

  /* Test RtChar_AppendCString. */
  RtArray_Create(&rtString, lpBuffer, sizeof(RT_CHAR), RT_CHAR_QUARTER_BIG_STRING_SIZE);
  if (!RtChar_AppendCString(&rtString, lpString1)) goto handle_error;
  if (!RtChar_AppendCString(&rtString, lpString2)) goto handle_error;
  if (!RtChar_StringEqualsString(&rtString, RtChar_CreateString(&rtExpected, lpExpected))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestAppend()
{
  RT_CHAR lpBuffer[12];
  RT_ARRAY rtBuffer;
  RT_B bResult;

  if (!ZzDoTestAppend(_R("Hello"), _R(", World!"), _R("Hello, World!"))) goto handle_error;
  if (!ZzDoTestAppend(_R(""),      _R("Foo"),      _R("Foo")))           goto handle_error;
  if (!ZzDoTestAppend(_R("Bar"),   _R(""),         _R("Bar")))           goto handle_error;
  if (!ZzDoTestAppend(_R(""),      _R(""),         _R("")))              goto handle_error;

  RtArray_Create(&rtBuffer, lpBuffer, sizeof(RT_CHAR), 12);
  if (!RtChar_AppendCString(&rtBuffer, _R("Hello,"))) goto handle_error;
  if (!RtChar_AppendCString(&rtBuffer, _R(" world"))) goto handle_error;

  /* Capacity should be to small for another character. */
  if (RtChar_AppendCString(&rtBuffer, _R("!"))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestEquals(RT_CHAR* lpString, RT_CHAR* lpOtherString, RT_B bExpected)
{
  RT_ARRAY rtString;
  RT_ARRAY rtOtherString;
  RT_B bResult;

  if (!RtChar_StringEqualsString(RtChar_CreateString(&rtString, lpString),
                                 RtChar_CreateString(&rtOtherString, lpOtherString)) != !bExpected) goto handle_error;

  if (!RtChar_StringEqualsCString(RtChar_CreateString(&rtString, lpString),
                                  lpOtherString) != !bExpected) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestEquals()
{
  RT_B bResult;

  if (!ZzDoTestEquals(_R("Foo"),     _R("Foo"),      RT_TRUE))  goto handle_error;
  if (!ZzDoTestEquals(_R("Foo"),     _R("Bar"),      RT_FALSE)) goto handle_error;
  if (!ZzDoTestEquals(_R(""),        _R(""),         RT_TRUE))  goto handle_error;
  if (!ZzDoTestEquals(_R("a"),       _R(""),         RT_FALSE)) goto handle_error;
  if (!ZzDoTestEquals(_R(""),        _R("a"),        RT_FALSE)) goto handle_error;
  if (!ZzDoTestEquals(_R("notsame"), _R("notsama"),  RT_FALSE)) goto handle_error;
  if (!ZzDoTestEquals(_R("notsame"), _R("notsamez"), RT_FALSE)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestSearchString(RT_CHAR* lpString, RT_CHAR* lpSearched, RT_UN unExpected)
{
  RT_ARRAY zzString;
  RT_ARRAY zzSearched;
  RT_B bResult;

  RtChar_CreateString(&zzString, lpString);
  RtChar_CreateString(&zzSearched, lpSearched);
  if (RtChar_SearchString(&zzString, &zzSearched) != unExpected) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestSearchString()
{
  RT_B bResult;

  if (!ZzDoTestSearchString(_R("Foo"),     _R("oo"), 1)) goto handle_error;
  if (!ZzDoTestSearchString(_R("Foo"),     _R("F"),  0)) goto handle_error;
  if (!ZzDoTestSearchString(_R("Foo"),     _R("Fo"), 0)) goto handle_error;
  if (!ZzDoTestSearchString(_R("abcdefg"), _R("g"),  6)) goto handle_error;
  if (!ZzDoTestSearchString(_R("abcdefg"), _R("fg"), 5)) goto handle_error;
  if (!ZzDoTestSearchString(_R("fffaffg"), _R("fg"), 5)) goto handle_error;

  if (!ZzDoTestSearchString(_R("Foo"),  _R(""),      RT_TYPE_MAX_UN)) goto handle_error;
  if (!ZzDoTestSearchString(_R(""),     _R("Foo"),   RT_TYPE_MAX_UN)) goto handle_error;
  if (!ZzDoTestSearchString(_R("Foo"),  _R("f"),     RT_TYPE_MAX_UN)) goto handle_error;
  if (!ZzDoTestSearchString(_R("Foo"),  _R("Foz"),   RT_TYPE_MAX_UN)) goto handle_error;
  if (!ZzDoTestSearchString(_R("Foo"),  _R("Fooo"),  RT_TYPE_MAX_UN)) goto handle_error;
  if (!ZzDoTestSearchString(_R("aFoo"), _R("Foooo"), RT_TYPE_MAX_UN)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestSearch(RT_CHAR* lpString, RT_CHAR nSearched, RT_UN unExpected)
{
  RT_ARRAY zzString;
  RT_B bResult;

  RtChar_CreateString(&zzString, lpString);
  if (RtChar_Search(&zzString, nSearched) != unExpected) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestSearch()
{
  RT_B bResult;

  if (!ZzDoTestSearch(_R("Foo"),     _R('o'), 1)) goto handle_error;
  if (!ZzDoTestSearch(_R("Foo"),     _R('F'), 0)) goto handle_error;
  if (!ZzDoTestSearch(_R("abcdefg"), _R('g'), 6)) goto handle_error;
  if (!ZzDoTestSearch(_R("abcdefg"), _R('f'), 5)) goto handle_error;

  if (!ZzDoTestSearch(_R(""),    _R('a'), RT_TYPE_MAX_UN)) goto handle_error;
  if (!ZzDoTestSearch(_R("Foo"), _R('f'), RT_TYPE_MAX_UN)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestConvertIntegerToString(RT_N nInput, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[200];
  RT_ARRAY rtBuffer;
  RT_ARRAY rtExpected;
  RT_B bResult;

  RtArray_Create(&rtBuffer, lpBuffer, sizeof(RT_CHAR), 200);

  if (!RtChar_ConvertIntegerToString(nInput, &rtBuffer)) goto handle_error;
  if (!RtChar_StringEqualsString(&rtBuffer, RtChar_CreateString(&rtExpected, lpExpected))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestConvertIntegerToString()
{
  RT_B bResult;

  if (!ZzDoTestConvertIntegerToString(-2000000000, _R("-2000000000"))) goto handle_error;
  if (!ZzDoTestConvertIntegerToString(-1, _R("-1"))) goto handle_error;
  if (!ZzDoTestConvertIntegerToString(0, _R("0"))) goto handle_error;
  if (!ZzDoTestConvertIntegerToString(2, _R("2"))) goto handle_error;
  if (!ZzDoTestConvertIntegerToString(123, _R("123"))) goto handle_error;
  if (!ZzDoTestConvertIntegerToString(2000000000, _R("2000000000"))) goto handle_error;
  /* TODO: Test bigger numbers in case of 64 bits. */

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestConvertStringToUnsignedInteger(RT_CHAR* lpString, RT_UN unExpected)
{
  RT_ARRAY rtString;
  RT_UN unValue;
  RT_B bResult;

  if (!RtChar_ConvertStringToUnsignedInteger(RtChar_CreateString(&rtString, lpString), &unValue)) goto handle_error;
  if (unValue != unExpected) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestConvertStringToUnsignedInteger()
{
  RT_ARRAY rtString;
  RT_UN unValue;
  RT_B bResult;

  if (RtChar_ConvertStringToUnsignedInteger(RtChar_CreateString(&rtString, _R("")), &unValue)) goto handle_error;
  if (RtChar_ConvertStringToUnsignedInteger(RtChar_CreateString(&rtString, _R(" ")), &unValue)) goto handle_error;
  if (RtChar_ConvertStringToUnsignedInteger(RtChar_CreateString(&rtString, _R("a")), &unValue)) goto handle_error;
  if (RtChar_ConvertStringToUnsignedInteger(RtChar_CreateString(&rtString, _R("2a")), &unValue)) goto handle_error;
  if (RtChar_ConvertStringToUnsignedInteger(RtChar_CreateString(&rtString, _R("a2")), &unValue)) goto handle_error;
  if (RtChar_ConvertStringToUnsignedInteger(RtChar_CreateString(&rtString, _R("-2")), &unValue)) goto handle_error;

  if (!ZzDoTestConvertStringToUnsignedInteger(_R("0"), 0)) goto handle_error;
  if (!ZzDoTestConvertStringToUnsignedInteger(_R("1"), 1)) goto handle_error;
  if (!ZzDoTestConvertStringToUnsignedInteger(_R("123"), 123)) goto handle_error;
  if (!ZzDoTestConvertStringToUnsignedInteger(_R("2111222333"), 2111222333)) goto handle_error;
  if (!ZzDoTestConvertStringToUnsignedInteger(_R("4111222333"), 4111222333)) goto handle_error;
  /* TODO: Test larger numbers under 64 bits. */

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestConvertStringToInteger(RT_CHAR* lpString, RT_N nExpected)
{
  RT_ARRAY rtString;
  RT_N nValue;
  RT_B bResult;

  if (!RtChar_ConvertStringToInteger(RtChar_CreateString(&rtString, lpString), &nValue)) goto handle_error;
  if (nValue != nExpected) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestConvertStringToInteger()
{
  RT_ARRAY rtString;
  RT_N nResult;
  RT_B bResult;

  if (RtChar_ConvertStringToInteger(RtChar_CreateString(&rtString, _R("")), &nResult)) goto handle_error;
  if (RtChar_ConvertStringToInteger(RtChar_CreateString(&rtString, _R(" ")), &nResult)) goto handle_error;
  if (RtChar_ConvertStringToInteger(RtChar_CreateString(&rtString, _R("-")), &nResult)) goto handle_error;
  if (RtChar_ConvertStringToInteger(RtChar_CreateString(&rtString, _R("a")), &nResult)) goto handle_error;
  if (RtChar_ConvertStringToInteger(RtChar_CreateString(&rtString, _R("2a")), &nResult)) goto handle_error;
  if (RtChar_ConvertStringToInteger(RtChar_CreateString(&rtString, _R("a2")), &nResult)) goto handle_error;

  if (!ZzDoTestConvertStringToInteger(_R("0"), 0)) goto handle_error;
  if (!ZzDoTestConvertStringToInteger(_R("1"), 1)) goto handle_error;
  if (!ZzDoTestConvertStringToInteger(_R("123"), 123)) goto handle_error;
  if (!ZzDoTestConvertStringToInteger(_R("-123"), -123)) goto handle_error;
  if (!ZzDoTestConvertStringToInteger(_R("2111222333"), 2111222333)) goto handle_error;
  if (!ZzDoTestConvertStringToInteger(_R("-2111222333"), -2111222333)) goto handle_error;
  /* TODO: Test larger numbers under 64 bits. */

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestChar()
{
  RT_B bResult;

  if (!ZzTestEquals()) goto handle_error;
  if (!ZzTestAppend()) goto handle_error;
  if (!ZzTestGetStringSize()) goto handle_error;
  if (!ZzTestSearchString()) goto handle_error;
  if (!ZzTestSearch()) goto handle_error;
  if (!ZzTestConvertIntegerToString()) goto handle_error;
  if (!ZzTestConvertStringToUnsignedInteger()) goto handle_error;
  if (!ZzTestConvertStringToInteger()) goto handle_error;


  /* 1337 */

  if (!ZzTestLeftPadString()) goto handle_error;
  if (!ZzTestRightTrimString()) goto handle_error;
  if (!ZzTestCountStringOccurrences()) goto handle_error;
  if (!ZzTestReplaceString()) goto handle_error;
  if (!ZzTestSearchStringInStrings()) goto handle_error;

  if (!ZzTestCompareStrings()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
