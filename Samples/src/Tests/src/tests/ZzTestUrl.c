#include "ZzTests.h"

RT_B RT_CALL ZzPrintUrlInfoItem(RT_CHAR* lpFieldName, RT_CHAR* lpFieldValue, RT_UN unFieldSize)
{
  RT_B bResult;

  if (!RtConsole_WriteString(lpFieldName)) goto handle_error;
  if (lpFieldValue)
  {
    if (!RtConsole_WriteStringWithSize(lpFieldValue, unFieldSize)) goto handle_error;
  }
  else
  {
    if (!RtConsole_WriteStringWithSize(_R("null"), 3)) goto handle_error;
  }
  if (!RtConsole_WriteStringWithSize(_R("\n"), 1)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

/*
TODO: Test optional parameters.
Test ipv4/ipv6.
*/

RT_B RT_CALL ZzPrintUrlInfo(RT_URL_INFO* lpUrlInfo)
{
  RT_CHAR lpMsg[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_B bResult;

  if (!ZzPrintUrlInfoItem(_R("Scheme = "),   lpUrlInfo->lpScheme,   lpUrlInfo->unSchemeSize)) goto handle_error;
  if (!ZzPrintUrlInfoItem(_R("User = "),     lpUrlInfo->lpUser,     lpUrlInfo->unUserSize)) goto handle_error;
  if (!ZzPrintUrlInfoItem(_R("Password = "), lpUrlInfo->lpPassword, lpUrlInfo->unPasswordSize)) goto handle_error;
  if (!ZzPrintUrlInfoItem(_R("Host = "),     lpUrlInfo->lpHost,     lpUrlInfo->unHostSize)) goto handle_error;

  if (lpUrlInfo->unPort == RT_TYPE_MAX_UN)
  {
    if (!RtConsole_WriteString(_R("Port = null\n"))) goto handle_error;
  }
  else
  {
    unWritten = 0;
    if (!RtChar_CopyString(_R("Port = "),                  &lpMsg[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
    if (!RtChar_ConvertUIntegerToString(lpUrlInfo->unPort, &lpMsg[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
    if (!RtChar_CopyString(_R("\n"),                       &lpMsg[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
    if (!RtConsole_WriteStringWithSize(lpMsg, unWritten)) goto handle_error;
  }

  if (!ZzPrintUrlInfoItem(_R("Query = "), lpUrlInfo->lpQuery, lpUrlInfo->unQuerySize)) goto handle_error;
  if (!ZzPrintUrlInfoItem(_R("Fragment = "), lpUrlInfo->lpFragment, lpUrlInfo->unFragmentSize)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL RtCheckUrlInfoItem(RT_CHAR* lpUrlInfoItem, RT_UN unUrlInfoItemSize, RT_CHAR* lpItem)
{
  RT_CHAR lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unOutputSize;
  RT_B bResult;

  if (lpUrlInfoItem)
  {
    if (!lpItem) goto handle_error;

    if (!RtChar_CopyStringWithSize(lpUrlInfoItem, unUrlInfoItemSize, lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &unOutputSize)) goto handle_error;
    if (RtChar_CompareStrings(lpBuffer, lpItem)) goto handle_error;
  }
  else
  {
    if (lpItem) goto handle_error;
    /* Ok, both are null. */
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL RtCheckUrlInfo(RT_URL_INFO* lpUrlInfo, RT_CHAR* lpScheme, RT_CHAR* lpUser, RT_CHAR* lpPassword, RT_CHAR* lpHost, RT_UN unPort, RT_CHAR* lpPath, RT_CHAR* lpQuery, RT_CHAR* lpFragment)
{
  RT_B bResult;

  if (!RtCheckUrlInfoItem(lpUrlInfo->lpScheme,    lpUrlInfo->unSchemeSize,    lpScheme))    goto handle_error;
  if (!RtCheckUrlInfoItem(lpUrlInfo->lpUser,      lpUrlInfo->unUserSize,      lpUser))      goto handle_error;
  if (!RtCheckUrlInfoItem(lpUrlInfo->lpPassword,  lpUrlInfo->unPasswordSize,  lpPassword))  goto handle_error;
  if (!RtCheckUrlInfoItem(lpUrlInfo->lpHost,      lpUrlInfo->unHostSize,      lpHost))      goto handle_error;
  if (!RtCheckUrlInfoItem(lpUrlInfo->lpPath,      lpUrlInfo->unPathSize,      lpPath))      goto handle_error;
  if (!RtCheckUrlInfoItem(lpUrlInfo->lpQuery,     lpUrlInfo->unQuerySize,     lpQuery))     goto handle_error;
  if (!RtCheckUrlInfoItem(lpUrlInfo->lpFragment,  lpUrlInfo->unFragmentSize,  lpFragment))  goto handle_error;

  if (lpUrlInfo->unPort != unPort) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestUrl()
{
  RT_URL_INFO zzUrlInfo;
  RT_B bResult;

  if (!RtUrl_Parse(_R("http://Jane:Doe@www.example.com:8888/chemin/d/acc%C3%A8s.php?q=req&q2=req2#signet"), &zzUrlInfo)) goto handle_error;
  if (!ZzPrintUrlInfo(&zzUrlInfo)) goto handle_error;
  if (!RtCheckUrlInfo(&zzUrlInfo, _R("http"), _R("Jane"), _R("Doe"), _R("www.example.com"), 8888, _R("chemin/d/acc%C3%A8s.php"), _R("q=req&q2=req2"), _R("signet"))) goto handle_error;

 if (!RtUrl_Parse(_R("http://www.example.com:8888/chemin/d/acc%C3%A8s.php?q=req&q2=req2"), &zzUrlInfo)) goto handle_error;
 if (!RtCheckUrlInfo(&zzUrlInfo, _R("http"), RT_NULL, RT_NULL, _R("www.example.com"), 8888, _R("chemin/d/acc%C3%A8s.php"), _R("q=req&q2=req2"), RT_NULL)) goto handle_error;

 if (!RtUrl_Parse(_R("http://www.example.com/chemin/d/acc%C3%A8s.php?q=req&q2=req2"), &zzUrlInfo)) goto handle_error;
 if (!RtCheckUrlInfo(&zzUrlInfo, _R("http"), RT_NULL, RT_NULL, _R("www.example.com"), RT_TYPE_MAX_UN, _R("chemin/d/acc%C3%A8s.php"), _R("q=req&q2=req2"), RT_NULL)) goto handle_error;

 if (!RtUrl_Parse(_R("http://[2607:f0d0:1002:0051:0000:0000:0000:0004]/chemin/d/acc%C3%A8s.php"), &zzUrlInfo)) goto handle_error;
 if (!RtCheckUrlInfo(&zzUrlInfo, _R("http"), RT_NULL, RT_NULL, _R("[2607:f0d0:1002:0051:0000:0000:0000:0004]"), RT_TYPE_MAX_UN, _R("chemin/d/acc%C3%A8s.php"), RT_NULL, RT_NULL)) goto handle_error;

 if (!RtUrl_Parse(_R("http://www.example.com"), &zzUrlInfo)) goto handle_error;
 if (!RtCheckUrlInfo(&zzUrlInfo, _R("http"), RT_NULL, RT_NULL, _R("www.example.com"), RT_TYPE_MAX_UN, RT_NULL, RT_NULL, RT_NULL)) goto handle_error;

 if (!RtUrl_Parse(_R("http://www.example.com:8080"), &zzUrlInfo)) goto handle_error;
 if (!RtCheckUrlInfo(&zzUrlInfo, _R("http"), RT_NULL, RT_NULL, _R("www.example.com"), 8080, RT_NULL, RT_NULL, RT_NULL)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
