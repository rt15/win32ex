#include <RtWin32Ex.h>

#define TT_BIG_STRING_SIZE 7000
#define TT_STRING_SIZE_COUNT 2
RT_CHAR* tt_StringSizeStrings[] = { _R("Foo Bar."), _R("") };
RT_N tt_StringSizeSizes[] =       { 8        ,      0      };

RT_UN16 RT_CALL TtGetStringSize()
{
  RT_CHAR* lpBigString;
  RT_N nI;
  RT_UN16 unResult;

  unResult = 1;

  /* Testing various strings sizes. */
  for (nI = 0; nI < TT_STRING_SIZE_COUNT; nI++)
  {
    RtWriteStringsToConsole(3, _R("Testing \""), tt_StringSizeStrings[nI], _R("\" size...\n"));
    if (RtGetStringSize(tt_StringSizeStrings[nI]) != tt_StringSizeSizes[nI]) goto the_end;
  }

  RtWriteStringToConsole(_R("Testing big string size...\n"));

  /* Allocate some memory. */
  if (!RtAlloc((void**)&lpBigString, TT_BIG_STRING_SIZE * sizeof(RT_CHAR)))
  {
    goto the_end;
  }

  /* Fill the allocated memory with some 'a'. */
  for (nI = 0; nI < TT_BIG_STRING_SIZE - 1; nI++)
  {
    lpBigString[nI] = _R('a');
  }
  lpBigString[TT_BIG_STRING_SIZE - 1] = 0;

  /* Check the string size. */
  if (RtGetStringSize(lpBigString) != TT_BIG_STRING_SIZE - 1) goto free_big_string;

  unResult = 0;
free_big_string:
  if (!RtFree((void**)&lpBigString))
  {
    unResult = 1;
  }
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtCompareStrings()
{
  RT_UN16 unResult;

  unResult = 1;

  RtWriteStringToConsole(_R("Testing RtCompareStrings...\n"));

  if (RtCompareStrings(_R("Foo"), _R("Foo")) != 0) goto the_end;
  if (RtCompareStrings(_R(" "), _R(" ")) != 0) goto the_end;
  if (RtCompareStrings(_R("Foo"), _R("Bar")) <= 0) goto the_end;
  if (RtCompareStrings(_R("Bar"), _R("Foo")) >= 0) goto the_end;
  if (RtCompareStrings(_R(" "), _R("a")) >= 0) goto the_end;
  if (RtCompareStrings(_R("a"), _R(" ")) <= 0) goto the_end;
  if (RtCompareStrings(_R("e"), _R("é")) >= 0) goto the_end;
  if (RtCompareStrings(_R("é"), _R("e")) <= 0) goto the_end;
  if (RtCompareStrings(_R("a"), _R("ab")) >= 0) goto the_end;
  if (RtCompareStrings(_R("ab"), _R("a")) <= 0) goto the_end;
  unResult = 0;

the_end:
  return unResult;
}

/**
 * @param lpReference If RT_NULL, then lpSource is assumed as the reference. Not used if nBufferSize is negative or zero.
 */
RT_UN16 RT_CALL TtTestCopyString(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_CHAR* lpSource, RT_N nReference, RT_CHAR* lpReference)
{
  RT_CHAR lpLocalBuffer[200];
  RT_CHAR* lpLocalReference;
  RT_N nWritten;
  RT_UN16 unResult;

  RtConvertNumberToString(nBufferSize, lpLocalBuffer, 200, &nWritten);
  RtWriteStringsToConsole(5, _R("Testing RtCopyString with input string = \""), lpSource, _R("\" and buffer size = "), lpLocalBuffer, _R("...\n"));

  nWritten = 0;
  RtCopyString(lpSource, lpBuffer, nBufferSize, &nWritten);

  /* Check function result. */
  if (nWritten != nReference)
  {
    RtConvertNumberToString(nWritten, lpLocalBuffer, 200, &nWritten);
    RtWriteStringsToConsole(3, _R("Wrong result = "), lpLocalBuffer, _R(".\n"));
    unResult = 1;
    goto the_end;
  }

  /* If something has been written to the buffer. */
  if (nReference > 0)
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

    if (RtCompareStrings(lpLocalReference, lpBuffer))
    {
      RtWriteStringsToConsole(5, _R("Resulting string = \""), lpBuffer, _R("\" is different from reference = \""), lpLocalReference, _R("\".\n"));
      unResult = 1;
      goto the_end;
    }
  }

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtCopyString()
{
  RT_CHAR lpBuffer[200];
  RT_UN16 unResult;

  unResult = 1;
  if (TtTestCopyString(lpBuffer, -1, _R("FOO"), 0, RT_NULL)) goto the_end;
  if (TtTestCopyString(lpBuffer, 0, _R("FOO"), 0, RT_NULL)) goto the_end;
  if (TtTestCopyString(lpBuffer, 1, _R("FOO"), 0, _R(""))) goto the_end;
  if (TtTestCopyString(lpBuffer, 2, _R("FOO"), 1, _R("F"))) goto the_end;
  if (TtTestCopyString(lpBuffer, 3, _R("FOO"), 2, _R("FO"))) goto the_end;
  if (TtTestCopyString(lpBuffer, 4, _R("FOO"), 3, RT_NULL)) goto the_end;
  if (TtTestCopyString(lpBuffer, 200, _R("FOO"), 3, RT_NULL)) goto the_end;
  if (TtTestCopyString(lpBuffer, 200, _R(""), 0, RT_NULL)) goto the_end;
  if (TtTestCopyString(lpBuffer, 1, _R(""), 0, RT_NULL)) goto the_end;
  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtTestCopyStringWithSize(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_CHAR* lpSource, RT_N nSize, RT_N nReference, RT_CHAR* lpReference)
{
  RT_CHAR lpLocalBuffer1[200];
  RT_CHAR lpLocalBuffer2[200];
  RT_N nWritten;
  RT_UN16 unResult;

  RtConvertNumberToString(nBufferSize, lpLocalBuffer1, 200, &nWritten);
  RtConvertNumberToString(nSize, lpLocalBuffer2, 200, &nWritten);
  RtWriteStringsToConsole(7, _R("Testing RtCopyStringWithSize with input string = \""), lpSource, _R("\", buffer size = "), lpLocalBuffer1, _R(" and size = "), lpLocalBuffer2, _R("...\n"));

  nWritten = 0;
  RtCopyStringWithSize(lpSource, nSize, lpBuffer, nBufferSize, &nWritten);

 /* Check function result. */
  if (nWritten != nReference)
  {
    RtConvertNumberToString(nWritten, lpLocalBuffer1, 200, &nWritten);
    RtWriteStringsToConsole(3, _R("Wrong result = "), lpLocalBuffer1, _R(".\n"));
    unResult = 1;
    goto the_end;
  }

  /* If something has been written to the buffer. */
  if (nReference > 0)
  {
    if (RtCompareStrings(lpReference, lpBuffer))
    {
      RtWriteStringsToConsole(5, _R("Resulting string = \""), lpBuffer, _R("\" is different from reference = \""), lpReference, _R("\".\n"));
      unResult = 1;
      goto the_end;
    }
  }

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtCopyStringWithSize()
{
  RT_CHAR lpBuffer[200];
  RT_UN16 unResult;

  unResult = 1;
  if (TtTestCopyStringWithSize(lpBuffer, 1, _R("FOO"), 3, 0, _R(""))) goto the_end;
  if (TtTestCopyStringWithSize(lpBuffer, 2, _R("FOO"), 3, 1, _R("F"))) goto the_end;
  if (TtTestCopyStringWithSize(lpBuffer, 200, _R("FOO"), 3, 3, _R("FOO"))) goto the_end;
  if (TtTestCopyStringWithSize(lpBuffer, 200, _R("FOO"), 0, 0, _R(""))) goto the_end;
  if (TtTestCopyStringWithSize(lpBuffer, 200, _R("FOO"), 1, 1, _R("F"))) goto the_end;
  unResult = 0;

the_end:
  return unResult;
}


RT_UN16 RT_CALL TtTestLeftPadString(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_CHAR* lpInput, RT_CHAR nChar, RT_N nSize, RT_N nReference, RT_CHAR* lpReference)
{
  RT_CHAR lpLocalBuffer1[200];
  RT_CHAR lpLocalBuffer2[200];
  RT_N nWritten;
  RT_UN16 unResult;

  RtConvertNumberToString(nBufferSize, lpLocalBuffer1, 200, &nWritten);
  RtConvertNumberToString(nSize, lpLocalBuffer2, 200, &nWritten);
  RtWriteStringsToConsole(7, _R("Testing RtLeftPadString with input string = \""), lpInput, _R("\", buffer size = "), lpLocalBuffer1, _R(" and size = "), lpLocalBuffer2, _R("...\n"));

  nWritten = 0;
  RtLeftPadString(lpInput, nChar, nSize, lpBuffer, nBufferSize, &nWritten);

 /* Check function result. */
  if (nWritten != nReference)
  {
    RtConvertNumberToString(nWritten, lpLocalBuffer1, 200, &nWritten);
    RtWriteStringsToConsole(3, _R("Wrong result = "), lpLocalBuffer1, _R(".\n"));
    unResult = 1;
    goto the_end;
  }

  /* If something has been written to the buffer. */
  if (nReference > 0)
  {
    if (RtCompareStrings(lpReference, lpBuffer))
    {
      RtWriteStringsToConsole(5, _R("Resulting string = \""), lpBuffer, _R("\" is different from reference = \""), lpReference, _R("\".\n"));
      unResult = 1;
      goto the_end;
    }
  }

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtLeftPadString()
{
  RT_CHAR lpBuffer[200];
  RT_UN16 unResult;

  unResult = 1;
  if (TtTestLeftPadString(lpBuffer, 200, _R("Foo"), _R('O'), 6, 6, _R("OOOFoo"))) goto the_end;
  unResult = 0;

the_end:
  return unResult;
}


RT_UN16 RT_CALL TtTestRightTrimString(RT_CHAR* lpInput, RT_N nReference, RT_CHAR* lpReference)
{
  RT_CHAR lpLocalBuffer[200];
  RT_N nWritten;
  RT_UN16 unResult;

  unResult = 1;

  RtWriteStringsToConsole(3, _R("Testing RtRightTrimString with input string = \""), lpInput, _R("\"...\n"));

  RtCopyString(lpInput, lpLocalBuffer, 200, &nWritten);

  RtRightTrimString(lpLocalBuffer, &nWritten);

  if (nWritten != nReference)
  {
    RtConvertNumberToString(nWritten, lpLocalBuffer, 200, &nWritten);
    RtWriteStringsToConsole(3, _R("Wrong result = "), lpLocalBuffer, _R(".\n"));
    goto the_end;
  }

  if (RtCompareStrings(lpReference, lpLocalBuffer))
  {
    RtWriteStringsToConsole(5, _R("Resulting string = \""), lpLocalBuffer, _R("\" is different from reference = \""), lpReference, _R("\".\n"));
    goto the_end;
  }

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtRightTrimString()
{
  RT_UN16 unResult;

  unResult = 1;
  if (TtTestRightTrimString(_R("foo"), 3, _R("foo"))) goto the_end;
  if (TtTestRightTrimString(_R("foo "), 3, _R("foo"))) goto the_end;
  if (TtTestRightTrimString(_R("foo  "), 3, _R("foo"))) goto the_end;
  if (TtTestRightTrimString(_R(""), 0, _R(""))) goto the_end;
  if (TtTestRightTrimString(_R(" "), 0, _R(""))) goto the_end;
  if (TtTestRightTrimString(_R("  "), 0, _R(""))) goto the_end;
  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtTestCountStringOccurrences(RT_CHAR* lpString, RT_CHAR* lpSearched, RT_N nExpected)
{
  RT_CHAR lpFound[64];
  RT_CHAR lpExpected[64];
  RT_N nFound;
  RT_N nWritten;
  RT_UN16 unResult;

  RtWriteStringsToConsole(5, _R("Testing RtCountStringOccurrences, occurences count of \""), lpSearched, _R("\" in \""), lpString, _R("\"...\n"));
  nFound = RtCountStringOccurrences(lpString, lpSearched);
  if (nFound == nExpected)
  {
    unResult = 0;
  }
  else
  {
    RtConvertNumberToString(nExpected, lpExpected, 64, &nWritten);
    RtConvertNumberToString(nFound, lpFound, 64, &nWritten);
    RtWriteStringsToConsole(5, _R("Wrong result, expected "), lpExpected, _R(" but found "), lpFound, _R(".\n"));
    unResult = 1;
  }
  return unResult;
}

RT_UN16 RT_CALL TtCountStringOccurrences()
{
  RT_UN16 unResult;

  unResult = 1;
  if (TtTestCountStringOccurrences(_R("foo"), _R("foo"), 1)) goto the_end;
  if (TtTestCountStringOccurrences(_R("foofoo"), _R("foo"), 2)) goto the_end;
  if (TtTestCountStringOccurrences(_R("ofoofoo"), _R("o"), 5)) goto the_end;
  if (TtTestCountStringOccurrences(_R("foof"), _R("o"), 2)) goto the_end;

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtTestReplaceString(RT_CHAR* lpString, RT_CHAR* lpSearched, RT_CHAR* lpReplacement, RT_CHAR* lpReference)
{
  RT_CHAR lpBuffer[64];
  RT_N nWritten;
  RT_UN16 unResult;

  unResult = 1;

  RtWriteStringsToConsole(7, _R("Replacing \""), lpSearched, _R("\" by \""), lpReplacement, _R("\" in \""), lpString, _R("\".\n"));
  if (!RtReplaceString(lpString, lpSearched, lpReplacement, lpBuffer, 64, &nWritten)) goto the_end;
  if (RtCompareStrings(lpBuffer, lpReference))
  {
    RtWriteStringsToConsole(5, _R("Wrong value \""), lpBuffer, _R("\" expected \""), lpReference, _R("\".\n"));
    goto the_end;
  }

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtReplaceString()
{
  RT_UN16 unResult;

  unResult = 1;
  if (TtTestReplaceString(_R("foo"), _R("foo"), _R("bar"), _R("bar"))) goto the_end;
  if (TtTestReplaceString(_R("foo"), _R("o"), _R("a"), _R("faa"))) goto the_end;
  if (TtTestReplaceString(_R("foobarfoo"), _R("foo"), _R("a"), _R("abara"))) goto the_end;
  if (TtTestReplaceString(_R("foobarfoo"), _R("foo"), _R("long_string"), _R("long_stringbarlong_string"))) goto the_end;

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtTestChar()
{
  RT_UN16 unResult;

  unResult = 1;

  if (TtGetStringSize()) goto the_end;
  if (TtCompareStrings()) goto the_end;
  if (TtCopyString()) goto the_end;
  if (TtCopyStringWithSize()) goto the_end;
  if (TtLeftPadString()) goto the_end;
  if (TtRightTrimString()) goto the_end;
  if (TtCountStringOccurrences()) goto the_end;
  if (TtReplaceString()) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}
