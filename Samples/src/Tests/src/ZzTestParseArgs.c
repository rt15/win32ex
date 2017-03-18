#include "ZzTestParseArgs.h"

RT_B RT_CALL ZzParseArgsCallback(RT_UN unArgType, RT_B bValid, RT_CHAR nShortOption, RT_CHAR* lpLongOption,
                                       RT_UN unValueCardinality, RT_CHAR* lpValue, void* lpContext)
{
  RT_CHAR lpMsg[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;

  if (unArgType == RT_COMMAND_LINE_ARG_TYPE_SHORT)
  {
    if (!RtCopyStringWithSize(&nShortOption, 1,  &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
    if (!RtCopyString(_R(", short"),              &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  }
  else
  {
    /* RT_COMMAND_LINE_ARG_TYPE_LONG */

    if (!RtCopyString(lpLongOption,   &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
    if (!RtCopyString(_R(", long"),  &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  }

  if (bValid)
  {
   if (!RtCopyString(_R(", valid"),  &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  }
  else
  {
   if (!RtCopyString(_R(", invalid"),  &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  }

  switch (unValueCardinality)
  {
    case RT_COMMAND_LINE_ARG_VALUE_NONE:
      if (!RtCopyString(_R(", without value"),       &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
      break;
    case RT_COMMAND_LINE_ARG_VALUE_OPTIONAL:
      if (!RtCopyString(_R(", with optional value"),  &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
      break;
    case RT_COMMAND_LINE_ARG_VALUE_REQUIRED:
      if (!RtCopyString(_R(", with required value"),  &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
      break;
    default:
      goto handle_error;
  }

  if (lpValue)
  {
    if (!RtCopyString(_R(" => "),  &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
    if (!RtCopyString(lpValue,     &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  }
  else
  {
    if (!RtCopyString(_R(", empty"), &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  }

  if (!RtCopyStringWithSize(_R("\n"), 1, &lpMsg[unWritten], unWritten - RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (!RtWriteStringToConsoleWithSize(lpMsg, unWritten)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestParseArgs(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_CHAR* lpLongOptionsWithoutArg[6];
  RT_CHAR* lpLongOptionsWithOptionalArg[6];
  RT_CHAR* lpLongOptionsWithArg[6];
  RT_N32 nNonOptionsIndex;
  RT_N32 nI;
  RT_B bResult;

  lpLongOptionsWithoutArg[0] = _R("none1");
  lpLongOptionsWithoutArg[1] = _R("none2");
  lpLongOptionsWithoutArg[2] = _R("none3");
  lpLongOptionsWithoutArg[3] = _R("none4");
  lpLongOptionsWithoutArg[4] = _R("none5");
  lpLongOptionsWithoutArg[5] = RT_NULL;

  lpLongOptionsWithOptionalArg[0] = _R("optional1");
  lpLongOptionsWithOptionalArg[1] = _R("optional2");
  lpLongOptionsWithOptionalArg[2] = _R("optional3");
  lpLongOptionsWithOptionalArg[3] = _R("optional4");
  lpLongOptionsWithOptionalArg[4] = _R("optional5");
  lpLongOptionsWithOptionalArg[5] = RT_NULL;

  lpLongOptionsWithArg[0] = _R("required1");
  lpLongOptionsWithArg[1] = _R("required2");
  lpLongOptionsWithArg[2] = _R("required3");
  lpLongOptionsWithArg[3] = _R("required4");
  lpLongOptionsWithArg[4] = _R("required5");
  lpLongOptionsWithArg[5] = RT_NULL;

  if (!RtParseCommandLineArgs(&nArgC, lpArgV, &ZzParseArgsCallback, RT_NULL,
                              _R("abcdefg"), _R("hijklmn"), _R("opqrstu"),
                              lpLongOptionsWithoutArg, lpLongOptionsWithOptionalArg, lpLongOptionsWithArg,
                              &nNonOptionsIndex)) goto handle_error;

  RtWriteStringToConsole(_R("\nNon-options:\n"));

  for (nI = nNonOptionsIndex; nI < nArgC; nI++)
  {
    if (!RtWriteStringsOrErrorsToConsole(RT_TRUE, lpArgV[nI], _R("\n"), (RT_CHAR*)RT_NULL)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  RtWriteStringToConsole(_R("Parsing has failed.\n"));
  bResult = RT_FAILURE;
  goto free_resources;
}
