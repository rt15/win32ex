#include "ZzTests.h"

RT_B RT_CALL ZzDoTestBase64WithSize(RT_CHAR8* lpInput, RT_UN unInputSize, RT_CHAR8* lpBase64)
{
  RT_CHAR8 lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;

  RT_B bResult;

  unWritten = 0;
  if (!RtEncodeToBase64WithSize(lpInput, unInputSize, lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (unWritten != RtGetString8Size(lpBase64)) goto handle_error;

  if (RtCompareString8s(lpBuffer, lpBase64)) goto handle_error;

  unWritten = 0;
  if (!RtDecodeFromBase64(lpBase64, lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (unWritten != unInputSize) goto handle_error;

  if (RtCompareString8s(lpBuffer, lpInput)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestBase64(RT_CHAR8* lpInput, RT_CHAR8* lpBase64)
{
  return ZzDoTestBase64WithSize(lpInput, RtGetString8Size(lpInput), lpBase64);
}

RT_B RT_CALL ZzTestBase64()
{
  RT_B bResult;

  if (!ZzDoTestBase64WithSize("\0",    1, "AA==")) goto handle_error;
  if (!ZzDoTestBase64WithSize("\xE9",  1, "6Q==")) goto handle_error;

  if (!ZzDoTestBase64("f",        "Zg=="))          goto handle_error;
  if (!ZzDoTestBase64("fo",       "Zm8="))          goto handle_error;
  if (!ZzDoTestBase64("foo",      "Zm9v"))          goto handle_error;
  if (!ZzDoTestBase64("foob",     "Zm9vYg=="))      goto handle_error;
  if (!ZzDoTestBase64("fooba",    "Zm9vYmE="))      goto handle_error;
  if (!ZzDoTestBase64("foobar",   "Zm9vYmFy"))      goto handle_error;
  if (!ZzDoTestBase64("foobart",  "Zm9vYmFydA=="))  goto handle_error;

  if (!ZzDoTestBase64("Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.",
                      "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4="
                      )) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
