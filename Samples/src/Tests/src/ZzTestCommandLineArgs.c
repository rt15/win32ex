#include <RtWin32Ex.h>

RT_B RT_CALL ZzCheckCommandLineArg(RT_UN unExpectedArgType, RT_B bExpectedValid, RT_CHAR nExpectedShortOption, RT_CHAR* lpExpectedLongOption, RT_UN unExpectedValueCardinality, RT_CHAR* lpExpectedValue,
                                   RT_UN unArgType, RT_B bValid, RT_CHAR nShortOption, RT_CHAR* lpLongOption, RT_UN unValueCardinality, RT_CHAR* lpValue)
{
  RT_B bResult;

  if (unArgType != unExpectedArgType) goto handle_error;
  if ((bValid && !bExpectedValid) || (!bValid && bExpectedValid)) goto handle_error;
  if (nShortOption != nExpectedShortOption) goto handle_error;
  if (RtCompareNullStrings(lpLongOption, lpExpectedLongOption)) goto handle_error;
  if (unValueCardinality != unExpectedValueCardinality) goto handle_error;
  if (RtCompareNullStrings(lpValue, lpExpectedValue)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzCommandLineArgsCallback(RT_UN unArgType, RT_B bValid, RT_CHAR nShortOption, RT_CHAR* lpLongOption,
                                       RT_UN unValueCardinality, RT_CHAR* lpValue, void* lpContext)
{
  RT_UN* lpArgumentIndex;
  RT_CHAR lpShortOption[8];
  RT_CHAR* lpOptionName;
  RT_UN unWritten;
  RT_B bResult;

  lpArgumentIndex = (RT_UN*)lpContext;

  switch (*lpArgumentIndex)
  {
    case 0:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_SHORT, RT_TRUE, _R('a'), RT_NULL, RT_COMMAND_LINE_ARG_VALUE_NONE, RT_NULL, 
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 1:
      break;
  }

  if (unArgType == RT_COMMAND_LINE_ARG_TYPE_SHORT)
  {
    RtCopyStringWithSize(&nShortOption, 1, lpShortOption, 8, &unWritten);
    lpOptionName = lpShortOption;

  }
  else
  {
    /* RT_COMMAND_LINE_ARG_TYPE_LONG */
    
    lpOptionName = lpLongOption;
  }

  if (lpValue)
  {
    if (!RtWriteStringsOrErrorsToConsole(RT_TRUE, lpOptionName, _R(" => "), lpValue, _R("\n"), (RT_CHAR*)RT_NULL)) goto handle_error;
  }
  else
  {
    if (!RtWriteStringsOrErrorsToConsole(RT_TRUE, lpOptionName, _R("\n"), (RT_CHAR*)RT_NULL)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  /* Next argument. */
  (*lpArgumentIndex)++;
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestCommandLineArgs()
{
  RT_N32 nArgC;
  RT_CHAR* lpArgV[18];
  RT_CHAR* lpLongOptionsWithOptionalArg[4];
  RT_CHAR* lpLongOptionsWithArg[4];
  RT_UN unArgumentIndex;
  RT_N32 nNonOptionsIndex;
  RT_N32 nI;
  RT_B bResult;

  nArgC = sizeof(lpArgV) / sizeof(RT_CHAR*);
  lpArgV[0]  = _R("command");
  lpArgV[1]  = _R("-abhHVAL");
  lpArgV[2]  = _R("-coOVAL");
  lpArgV[3]  = _R("-dp");
  lpArgV[4]  = _R("PVAL");
  lpArgV[5]  = _R("--optional1");
  lpArgV[6]  = _R("--optional2=OPTIONAL2");
  lpArgV[7]  = _R("--required1");
  lpArgV[8]  = _R("REQUIRED1");
  lpArgV[9]  = _R("--required2=REQUIRED2");
  lpArgV[10] = _R("NON_OPTION1");
  lpArgV[11] = _R("-e");
  lpArgV[12] = _R("NON_OPTION2");
  lpArgV[13] = _R("--required3");
  lpArgV[14] = _R("REQUIRED3");
  lpArgV[15] = _R("--");
  lpArgV[16] = _R("--optional3");
  lpArgV[17] = _R("-f");

  lpLongOptionsWithOptionalArg[0] = _R("optional1");
  lpLongOptionsWithOptionalArg[1] = _R("optional2");
  lpLongOptionsWithOptionalArg[2] = _R("optional3");
  lpLongOptionsWithOptionalArg[3] = RT_NULL;

  lpLongOptionsWithArg[0] = _R("required1");
  lpLongOptionsWithArg[1] = _R("required2");
  lpLongOptionsWithArg[2] = _R("required3");
  lpLongOptionsWithArg[3] = RT_NULL;
 
  unArgumentIndex = 0;
  if (!RtParseCommandLineArgs(&nArgC, lpArgV, &ZzCommandLineArgsCallback, &unArgumentIndex,
                              _R("abcdefg"), _R("hijklmn"), _R("opqrstu"),
                              RT_NULL, lpLongOptionsWithOptionalArg, lpLongOptionsWithArg,
                              &nNonOptionsIndex)) goto handle_error;

  RtWriteStringToConsole(_R("\n\nNon-options:\n"));

  for (nI = nNonOptionsIndex; nI < nArgC; nI++)
  {
    RtWriteStringsOrErrorsToConsole(RT_TRUE, lpArgV[nI], _R("\n"), (RT_CHAR*)RT_NULL);
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
