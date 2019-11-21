#include <RtWin32Ex.h>

RT_B RT_CALL ZzCheckCommandLineArg(RT_UN unExpectedArgType, RT_B bExpectedValid, RT_CHAR nExpectedShortOption, RT_CHAR* lpExpectedLongOption, RT_UN unExpectedValueCardinality, RT_CHAR* lpExpectedValue,
                                   RT_UN unArgType, RT_B bValid, RT_CHAR nShortOption, RT_CHAR* lpLongOption, RT_UN unValueCardinality, RT_CHAR* lpValue)
{
  RT_B bResult;

  if (unArgType != unExpectedArgType) goto handle_error;
  if ((bValid && !bExpectedValid) || (!bValid && bExpectedValid)) goto handle_error;
  if (nShortOption != nExpectedShortOption) goto handle_error;
  if (RtChar_CompareNullStrings(lpLongOption, lpExpectedLongOption)) goto handle_error;
  if (unValueCardinality != unExpectedValueCardinality) goto handle_error;
  if (RtChar_CompareNullStrings(lpValue, lpExpectedValue)) goto handle_error;

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
  RT_B bResult;

  lpArgumentIndex = (RT_UN*)lpContext;

  switch (*lpArgumentIndex)
  {
    case 0:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_SHORT, RT_TRUE, _R('a'), RT_NULL, RT_COMMAND_LINE_ARG_VALUE_NONE, RT_NULL,
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 1:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_SHORT, RT_TRUE, _R('b'), RT_NULL, RT_COMMAND_LINE_ARG_VALUE_NONE, RT_NULL,
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 2:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_SHORT, RT_TRUE, _R('h'), RT_NULL, RT_COMMAND_LINE_ARG_VALUE_OPTIONAL, _R("HVAL"),
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 3:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_SHORT, RT_TRUE, _R('c'), RT_NULL, RT_COMMAND_LINE_ARG_VALUE_NONE, RT_NULL,
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 4:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_SHORT, RT_TRUE, _R('o'), RT_NULL, RT_COMMAND_LINE_ARG_VALUE_REQUIRED, _R("OVAL"),
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 5:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_SHORT, RT_TRUE, _R('d'), RT_NULL, RT_COMMAND_LINE_ARG_VALUE_NONE, RT_NULL,
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 6:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_SHORT, RT_TRUE, _R('p'), RT_NULL, RT_COMMAND_LINE_ARG_VALUE_REQUIRED, _R("PVAL"),
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 7:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_LONG, RT_TRUE, 0, _R("optional1"), RT_COMMAND_LINE_ARG_VALUE_OPTIONAL, RT_NULL,
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 8:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_LONG, RT_TRUE, 0, _R("optional2"), RT_COMMAND_LINE_ARG_VALUE_OPTIONAL, _R("OPTIONAL2"),
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 9:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_LONG, RT_FALSE, 0, _R("unknown"), RT_COMMAND_LINE_ARG_VALUE_NONE, _R("UNKNOWN"),
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 10:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_SHORT, RT_FALSE, _R('v'), RT_NULL, RT_COMMAND_LINE_ARG_VALUE_NONE, RT_NULL,
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 11:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_LONG, RT_TRUE, 0, _R("required1"), RT_COMMAND_LINE_ARG_VALUE_REQUIRED, _R("REQUIRED1"),
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 12:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_LONG, RT_TRUE, 0, _R("required2"), RT_COMMAND_LINE_ARG_VALUE_REQUIRED, _R("REQUIRED2"),
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 13:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_LONG, RT_TRUE, 0, _R("none1"), RT_COMMAND_LINE_ARG_VALUE_NONE, RT_NULL,
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 14:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_LONG, RT_TRUE, 0, _R("none2"), RT_COMMAND_LINE_ARG_VALUE_NONE, _R("BAD_VALUE"),
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 15:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_SHORT, RT_TRUE, _R('e'), RT_NULL, RT_COMMAND_LINE_ARG_VALUE_NONE, RT_NULL,
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    case 16:
      if (!ZzCheckCommandLineArg(RT_COMMAND_LINE_ARG_TYPE_LONG, RT_TRUE, 0, _R("required3"), RT_COMMAND_LINE_ARG_VALUE_REQUIRED, _R("REQUIRED3"),
                                 unArgType, bValid, nShortOption, lpLongOption, unValueCardinality, lpValue)) goto handle_error;
      break;
    default:
      goto handle_error;
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
  RT_CHAR* lpArgV[22];
  RT_CHAR* lpLongOptionsWithoutArg[3];
  RT_CHAR* lpLongOptionsWithOptionalArg[4];
  RT_CHAR* lpLongOptionsWithArg[4];
  RT_CHAR* lpNonOptions[4];
  RT_ARRAY zzNonOption;
  RT_UN unArgumentIndex;
  RT_N32 nNonOptionsIndex;
  RT_N32 nI;
  RT_UN unJ;
  RT_B bResult;

  nArgC = sizeof(lpArgV) / sizeof(RT_CHAR*);
  lpArgV[0]  = _R("command");
  lpArgV[1]  = _R("-abhHVAL");
  lpArgV[2]  = _R("-coOVAL");
  lpArgV[3]  = _R("-dp");
  lpArgV[4]  = _R("PVAL");
  lpArgV[5]  = _R("--optional1");
  lpArgV[6]  = _R("--optional2=OPTIONAL2");
  lpArgV[7]  = _R("--unknown=UNKNOWN");
  lpArgV[8]  = _R("-v");
  lpArgV[9]  = _R("--required1");
  lpArgV[10] = _R("REQUIRED1");
  lpArgV[11] = _R("--required2=REQUIRED2");
  lpArgV[12] = _R("NON_OPTION1");
  lpArgV[13] = _R("--none1");
  lpArgV[14] = _R("--none2=BAD_VALUE");
  lpArgV[15] = _R("-e");
  lpArgV[16] = _R("NON_OPTION2");
  lpArgV[17] = _R("--required3");
  lpArgV[18] = _R("REQUIRED3");
  lpArgV[19] = _R("--");
  lpArgV[20] = _R("--optional3");
  lpArgV[21] = _R("-f");

  lpLongOptionsWithoutArg[0] = _R("none1");
  lpLongOptionsWithoutArg[1] = _R("none2");
  lpLongOptionsWithoutArg[2] = RT_NULL;

  lpLongOptionsWithOptionalArg[0] = _R("optional1");
  lpLongOptionsWithOptionalArg[1] = _R("optional2");
  lpLongOptionsWithOptionalArg[2] = _R("optional3");
  lpLongOptionsWithOptionalArg[3] = RT_NULL;

  lpLongOptionsWithArg[0] = _R("required1");
  lpLongOptionsWithArg[1] = _R("required2");
  lpLongOptionsWithArg[2] = _R("required3");
  lpLongOptionsWithArg[3] = RT_NULL;

  unArgumentIndex = 0;
  if (!RtCommandLineArgs_Parse(&nArgC, lpArgV, &ZzCommandLineArgsCallback, &unArgumentIndex,
                              _R("abcdefg"), _R("hijklmn"), _R("opqrstu"),
                              lpLongOptionsWithoutArg, lpLongOptionsWithOptionalArg, lpLongOptionsWithArg,
                              &nNonOptionsIndex)) goto handle_error;

  if (nArgC - nNonOptionsIndex != sizeof(lpNonOptions) / sizeof(RT_CHAR*)) goto handle_error;

  lpNonOptions[0] = _R("NON_OPTION1");
  lpNonOptions[1] = _R("NON_OPTION2");
  lpNonOptions[2] = _R("--optional3");
  lpNonOptions[3] = _R("-f");

  unJ = 0;
  for (nI = nNonOptionsIndex; nI < nArgC; nI++)
  {
    RtChar_CreateString(&zzNonOption, lpNonOptions[unJ]);
    if (!RtChar_StringEqualsCString(&zzNonOption, lpArgV[nI])) goto handle_error;
    unJ++;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
