#include "ZzTests.h"

#define ZZ_BIG_STRING_SIZE 7000
#define ZZ_STRING_SIZE_COUNT 2
RT_CHAR* zz_StringSizeStrings[] = { _R("Foo Bar."), _R("") };
RT_N zz_StringSizeSizes[] =       { 8        ,      0      };

RT_B RT_CALL ZzTestGetStringSize()
{
  RT_CHAR* lpBigString;
  RT_UN unI;
  RT_B bResult;

  lpBigString = RT_NULL;

  /* Testing various strings sizes. */
  for (unI = 0; unI < ZZ_STRING_SIZE_COUNT; unI++)
  {
    RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Testing \""), zz_StringSizeStrings[unI], _R("\" size...\n"), (RT_CHAR*)RT_NULL);
    if (RtChar_GetStringSize(zz_StringSizeStrings[unI]) != zz_StringSizeSizes[unI]) goto handle_error;
  }

  RtConsole_WriteString(_R("Testing big string size...\n"));

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
  if (RtChar_GetStringSize(lpBigString) != ZZ_BIG_STRING_SIZE - 1) goto handle_error;


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

RT_B RT_CALL ZzTestCompareStrings()
{
  RT_B bResult;

  RtConsole_WriteString(_R("Testing RtChar_CompareStrings...\n"));

  if (RtChar_CompareStrings(_R("Foo"), _R("Foo")) != 0) goto handle_error;
  if (RtChar_CompareStrings(_R(" "), _R(" ")) != 0) goto handle_error;
  if (RtChar_CompareStrings(_R("Foo"), _R("Bar")) <= 0) goto handle_error;
  if (RtChar_CompareStrings(_R("Bar"), _R("Foo")) >= 0) goto handle_error;
  if (RtChar_CompareStrings(_R(" "), _R("a")) >= 0) goto handle_error;
  if (RtChar_CompareStrings(_R("a"), _R(" ")) <= 0) goto handle_error;
  if (RtChar_CompareStrings(_R("e"), _R("�")) >= 0) goto handle_error;
  if (RtChar_CompareStrings(_R("�"), _R("e")) <= 0) goto handle_error;
  if (RtChar_CompareStrings(_R("a"), _R("ab")) >= 0) goto handle_error;
  if (RtChar_CompareStrings(_R("ab"), _R("a")) <= 0) goto handle_error;

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

/**
 * @param lpReference If RT_NULL, then lpSource is assumed as the reference. Not used if unBufferSize is negative or zero.
 */
RT_B RT_CALL ZzDoTestCopyString(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_CHAR* lpSource, RT_UN unReference, RT_CHAR* lpReference)
{
  RT_CHAR lpLocalBuffer[200];
  RT_CHAR* lpLocalReference;
  RT_UN unWritten;
  RT_B bResult;

  RtChar_ConvertIntegerToString(unBufferSize, lpLocalBuffer, 200, &unWritten);
  RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Testing RtChar_CopyString with input string = \""), lpSource, _R("\" and buffer size = "), lpLocalBuffer, _R("...\n"), (RT_CHAR*)RT_NULL);

  unWritten = 0;
  RtChar_CopyString(lpSource, lpBuffer, unBufferSize, &unWritten);

  /* Check function result. */
  if (unWritten != unReference)
  {
    RtChar_ConvertIntegerToString(unWritten, lpLocalBuffer, 200, &unWritten);
    RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Wrong result = "), lpLocalBuffer, _R(".\n"), (RT_CHAR*)RT_NULL);
    goto handle_error;
  }

  /* If something has been written to the buffer. */
  if (unReference > 0)
  {
    /* Use lpSource as reference if lpReference is RT_NULL. */
    if (lpReference)
    {
      lpLocalReference = lpReference;
    }
    else
    {
      lpLocalReference = lpSource;
    }

    if (RtChar_CompareStrings(lpLocalReference, lpBuffer))
    {
      RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Resulting string = \""), lpBuffer, _R("\" is different from reference = \""), lpLocalReference, _R("\".\n"), (RT_CHAR*)RT_NULL);
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

RT_B RT_CALL ZzTestCopyString()
{
  RT_CHAR lpBuffer[200];
  RT_B bResult;

  if (!ZzDoTestCopyString(lpBuffer, 0, _R("FOO"), 0, RT_NULL)) goto handle_error;
  if (!ZzDoTestCopyString(lpBuffer, 1, _R("FOO"), 0, _R(""))) goto handle_error;
  if (!ZzDoTestCopyString(lpBuffer, 2, _R("FOO"), 1, _R("F"))) goto handle_error;
  if (!ZzDoTestCopyString(lpBuffer, 3, _R("FOO"), 2, _R("FO"))) goto handle_error;
  if (!ZzDoTestCopyString(lpBuffer, 4, _R("FOO"), 3, RT_NULL)) goto handle_error;
  if (!ZzDoTestCopyString(lpBuffer, 200, _R("FOO"), 3, RT_NULL)) goto handle_error;
  if (!ZzDoTestCopyString(lpBuffer, 200, _R(""), 0, RT_NULL)) goto handle_error;
  if (!ZzDoTestCopyString(lpBuffer, 1, _R(""), 0, RT_NULL)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestCopyStringWithSize(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_CHAR* lpSource, RT_UN unSize, RT_UN unReference, RT_CHAR* lpReference)
{
  RT_CHAR lpLocalBuffer1[200];
  RT_CHAR lpLocalBuffer2[200];
  RT_UN unWritten;
  RT_B bResult;

  RtChar_ConvertIntegerToString(unBufferSize, lpLocalBuffer1, 200, &unWritten);
  RtChar_ConvertIntegerToString(unSize, lpLocalBuffer2, 200, &unWritten);
  RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Testing RtChar_CopyStringWithSize with input string = \""), lpSource, _R("\", buffer size = "), lpLocalBuffer1, _R(" and size = "), lpLocalBuffer2, _R("...\n"), (RT_CHAR*)RT_NULL);

  unWritten = 0;
  RtChar_CopyStringWithSize(lpSource, unSize, lpBuffer, unBufferSize, &unWritten);

 /* Check function result. */
  if (unWritten != unReference)
  {
    RtChar_ConvertIntegerToString(unWritten, lpLocalBuffer1, 200, &unWritten);
    RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Wrong result = "), lpLocalBuffer1, _R(".\n"), (RT_CHAR*)RT_NULL);
    goto handle_error;
  }

  /* If something has been written to the buffer. */
  if (unReference > 0)
  {
    if (RtChar_CompareStrings(lpReference, lpBuffer))
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

RT_B RT_CALL ZzTestCopyStringWithSize()
{
  RT_CHAR lpBuffer[200];
  RT_B bResult;

  if (!ZzDoTestCopyStringWithSize(lpBuffer, 1, _R("FOO"), 3, 0, _R(""))) goto handle_error;
  if (!ZzDoTestCopyStringWithSize(lpBuffer, 2, _R("FOO"), 3, 1, _R("F"))) goto handle_error;
  if (!ZzDoTestCopyStringWithSize(lpBuffer, 200, _R("FOO"), 3, 3, _R("FOO"))) goto handle_error;
  if (!ZzDoTestCopyStringWithSize(lpBuffer, 200, _R("FOO"), 0, 0, _R(""))) goto handle_error;
  if (!ZzDoTestCopyStringWithSize(lpBuffer, 200, _R("FOO"), 1, 1, _R("F"))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}


RT_B RT_CALL ZzDoTestLeftPadString(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_CHAR* lpInput, RT_CHAR nChar, RT_UN unSize, RT_UN unReference, RT_CHAR* lpReference)
{
  RT_CHAR lpLocalBuffer1[200];
  RT_CHAR lpLocalBuffer2[200];
  RT_UN unWritten;
  RT_B bResult;

  RtChar_ConvertIntegerToString(unBufferSize, lpLocalBuffer1, 200, &unWritten);
  RtChar_ConvertIntegerToString(unSize, lpLocalBuffer2, 200, &unWritten);
  RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Testing RtChar_LeftPadString with input string = \""), lpInput, _R("\", buffer size = "), lpLocalBuffer1, _R(" and size = "), lpLocalBuffer2, _R("...\n"), (RT_CHAR*)RT_NULL);

  unWritten = 0;
  RtChar_LeftPadString(lpInput, nChar, unSize, lpBuffer, unBufferSize, &unWritten);

 /* Check function result. */
  if (unWritten != unReference)
  {
    RtChar_ConvertIntegerToString(unWritten, lpLocalBuffer1, 200, &unWritten);
    RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Wrong result = "), lpLocalBuffer1, _R(".\n"), (RT_CHAR*)RT_NULL);
    goto handle_error;
  }

  /* If something has been written to the buffer. */
  if (unReference > 0)
  {
    if (RtChar_CompareStrings(lpReference, lpBuffer))
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
  RT_CHAR lpLocalBuffer[200];
  RT_UN unWritten;
  RT_B bResult;

  RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Testing RtChar_RightTrimString with input string = \""), lpInput, _R("\"...\n"), (RT_CHAR*)RT_NULL);

  RtChar_CopyString(lpInput, lpLocalBuffer, 200, &unWritten);

  RtChar_RightTrimString(lpLocalBuffer, &unWritten);

  if (unWritten != unReference)
  {
    RtChar_ConvertIntegerToString(unWritten, lpLocalBuffer, 200, &unWritten);
    RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Wrong result = "), lpLocalBuffer, _R(".\n"), (RT_CHAR*)RT_NULL);
    goto handle_error;
  }

  if (RtChar_CompareStrings(lpReference, lpLocalBuffer))
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
  RT_CHAR lpFound[64];
  RT_CHAR lpExpected[64];
  RT_UN unFound;
  RT_UN unWritten;
  RT_B bResult;

  RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Testing RtChar_CountStringOccurrences, occurences count of \""), lpSearched, _R("\" in \""), lpString, _R("\"...\n"), (RT_CHAR*)RT_NULL);
  unFound = RtChar_CountStringOccurrences(lpString, lpSearched);
  if (unFound != unExpected)
  {
    RtChar_ConvertUIntegerToString(unExpected, lpExpected, 64, &unWritten);
    RtChar_ConvertUIntegerToString(unFound, lpFound, 64, &unWritten);
    RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Wrong result, expected "), lpExpected, _R(" but found "), lpFound, _R(".\n"), (RT_CHAR*)RT_NULL);
    goto handle_error;
  }

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
  RT_CHAR lpBuffer[64];
  RT_UN unWritten;
  RT_B bResult;

  RtConsole_WriteStringsOrErrors(RT_TRUE, _R("Replacing \""), lpSearched, _R("\" by \""), lpReplacement, _R("\" in \""), lpString, _R("\".\n"), (RT_CHAR*)RT_NULL);
  if (!RtChar_ReplaceString(lpString, lpSearched, lpReplacement, lpBuffer, 64, &unWritten)) goto handle_error;
  if (RtChar_CompareStrings(lpBuffer, lpReference))
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

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestConcatStrings()
{
  RT_CHAR lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  if (!RtChar_ConcatStrings(&lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten, _R("foo"), _R("bar"), _R("team"), (RT_CHAR*)RT_NULL)) goto handle_error;
  if (unWritten != 10) goto handle_error;
  if (RtChar_CompareStrings(_R("foobarteam"), lpBuffer)) goto handle_error;

  if (!RtChar_ConcatStrings(&lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten, _R("Hello, world!"), (RT_CHAR*)RT_NULL)) goto handle_error;
  if (unWritten != 23) goto handle_error;
  if (RtChar_CompareStrings(_R("foobarteamHello, world!"), lpBuffer)) goto handle_error;

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

RT_B RT_CALL ZzTestSearchChar()
{
  RT_B bResult;

  if (RtChar_Search(_R("foo"), _R('f')) != 0) goto handle_error;
  if (RtChar_Search(_R("foo"), _R('o')) != 1) goto handle_error;
  if (RtChar_Search(_R("foo"), _R('b')) != RT_TYPE_MAX_UN) goto handle_error;

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

  if (!ZzTestGetStringSize()) goto handle_error;
  if (!ZzTestCompareStrings()) goto handle_error;
  if (!ZzTestCopyString()) goto handle_error;
  if (!ZzTestCopyStringWithSize()) goto handle_error;
  if (!ZzTestLeftPadString()) goto handle_error;
  if (!ZzTestRightTrimString()) goto handle_error;
  if (!ZzTestCountStringOccurrences()) goto handle_error;
  if (!ZzTestReplaceString()) goto handle_error;
  if (!ZzTestConcatStrings()) goto handle_error;
  if (!ZzTestSearchStringInStrings()) goto handle_error;
  if (!ZzTestSearchChar()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
