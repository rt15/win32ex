#include <RtWin32Ex.h>

RT_CHAR* tt_System_0 = _R("ea");
RT_UCHAR8 tt_Ascii[3] = { 0x65, 0x61, 0x00 };

RT_CHAR* tt_System_1 = _R("ιθ");
RT_UCHAR8 tt_ISO88591[3] = { 0xE9, 0xE8, 0x00 };
RT_UCHAR8 tt_Utf8_1[5] = { 0xC3, 0xA9, 0xC3, 0xA8, 0x00 };

RT_UCHAR8 tt_Utf16[8] = { 0xFF, 0xFE, 0xE9, 0x00, 0xE8, 0x00, 0x00, 0x00 };
RT_UCHAR8 tt_Utf16BeWithBom[8] = { 0xFE, 0xFF, 0x00, 0xE9, 0x00, 0xE8, 0x00, 0x00 };
RT_UCHAR8 tt_Utf16Le[6] = { 0xE9, 0x00, 0xE8, 0x00, 0x00, 0x00 };
RT_UCHAR8 tt_Utf16Be[6] = { 0x00, 0xE9, 0x00, 0xE8, 0x00, 0x00 };

RT_UCHAR8 tt_Utf32[62] = { 0xFF, 0xFE, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
RT_UCHAR8 tt_Utf32BeWithBom[62] = { 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00 };
RT_UCHAR8 tt_Utf32Le[12] = { 0xE9, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
RT_UCHAR8 tt_Utf32Be[12] = { 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00 };

/* Small t with cedilla (U+0163) is available in ISO-8859-2 but not in ISO-8859-1. */
#ifdef RT_DEFINE_GCC
RT_CHAR* tt_System_2 = _R("ι\u0163");
#else
RT_CHAR* tt_System_2 = _R("ι\u0163");
#endif
RT_UCHAR8 tt_ISO88592[3] = { 0xE9, 0xFE, 0x00 };
RT_UCHAR8 tt_Utf8_2[5] = { 0xC3, 0xA9, 0xC5, 0xA3, 0x00 };

RT_UN16 RT_CALL TtTestEncodeDecode(RT_CHAR* lpCharacters, RT_UCHAR8* lpEncoded, RT_UN unEncoding, RT_UN unEncodedLength, RT_UN unEncodedCharSize, RT_B bDecodeOnly)
{
  RT_UN unExpectedStringSize;
  RT_CHAR8 lpBuffer[512];
  RT_CHAR lpCharBuffer[512];
  RT_UN16 unResult;

  unResult = 1;

  if (RtGetTerminatingZeroSize(unEncoding) != unEncodedCharSize) goto the_end;
  if (RtGetDataSize((RT_CHAR8*)lpEncoded, unEncodedCharSize) != unEncodedLength) goto the_end;

  /* Encode. */
  if (!bDecodeOnly)
  {
    if (RtEncodeWithBuffer(lpCharacters, RT_TYPE_MAX_UN, unEncoding, lpBuffer, 512) != unEncodedLength) goto the_end;
    if (RT_MEMORY_COMPARE(lpBuffer, lpEncoded, unEncodedLength + unEncodedCharSize)) goto the_end;
  }

  /* Decode. */
  unExpectedStringSize = RtGetStringSize(lpCharacters);
  if (RtDecodeWithBuffer((RT_CHAR8*)lpEncoded, RT_TYPE_MAX_UN, unEncoding, lpCharBuffer, 512) != unExpectedStringSize) goto the_end;
  if (RtCompareStrings(lpCharBuffer, lpCharacters)) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtTestEncoding()
{
  RT_UN16 unResult;

  unResult = 1;

  if (TtTestEncodeDecode(tt_System_0, tt_Ascii, RT_ENCODING_US_ASCII, 2, 1, RT_FALSE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_0, tt_Ascii, RT_ENCODING_UTF_8, 2, 1, RT_FALSE)) goto the_end;

  if (TtTestEncodeDecode(tt_System_1, tt_ISO88591, RT_ENCODING_ISO_8859_1, 2, 1, RT_FALSE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_1, tt_Utf8_1, RT_ENCODING_UTF_8, 4, 1, RT_FALSE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_1, tt_Utf16, RT_ENCODING_UTF_16, 6, 2, RT_FALSE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_1, tt_Utf16BeWithBom, RT_ENCODING_UTF_16, 6, 2, RT_TRUE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_1, tt_Utf16Le, RT_ENCODING_UTF_16LE, 4, 2, RT_FALSE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_1, tt_Utf16Be, RT_ENCODING_UTF_16BE, 4, 2, RT_FALSE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_1, tt_Utf32, RT_ENCODING_UTF_32, 12, 4, RT_FALSE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_1, tt_Utf32BeWithBom, RT_ENCODING_UTF_32, 12, 4, RT_TRUE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_1, tt_Utf32Le, RT_ENCODING_UTF_32LE, 8, 4, RT_FALSE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_1, tt_Utf32Be, RT_ENCODING_UTF_32BE, 8, 4, RT_FALSE)) goto the_end;

  if (TtTestEncodeDecode(tt_System_2, tt_ISO88592, RT_ENCODING_ISO_8859_2, 2, 1, RT_FALSE)) goto the_end;
  if (TtTestEncodeDecode(tt_System_2, tt_Utf8_2, RT_ENCODING_UTF_8, 4, 1, RT_FALSE)) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}
