#ifndef RT_ENCODING_H
#define RT_ENCODING_H

#include "layer000/RtWin32ExTypes.h"
#include "layer005/RtHeap.h"

/**
 * @file
 * Encode from RT_CHAR to any encoding and decode from any encoding to RT_CHAR.
 *
 * <p>
 * UTF-16 and UTF-32 from Linux iconv are not supported because:
 * </p>
 * <ul>
 * <li>They produce BOM.</li>
 * <li>The endianness of the result is system dependent.</li>
 * </ul>
 * <p>
 * Use UTF-16LE/BE and UTF-32LE/BE instead.<br>
 * They have fixed endianness, have no BOM and are aligned with Windows.
 * </p>
 *
 * <p>
 * Unicode BOMs:
 * </p>
 * <table>
 *   <tr><th>Unicode encoding</th><th>BOM</th></tr>
 *   <tr><td>UTF-16, little-endian</td><td>0xFF, 0xFE</td></tr>
 *   <tr><td>UTF-16, big-endian</td><td>0xFE, 0xFF</td></tr>
 *   <tr><td>UTF-32, little-endian</td><td>0xFF, 0xFE, 0x00, 0x00</td></tr>
 *   <tr><td>UTF-32, big-endian</td><td>0x00, 0x00, 0xFE, 0xFF</td></tr>
 *   <tr><td>UTF-8</td><td>0xEF, 0xBB, 0xBF</td></tr>
 * </table>
 */

/* IBM EBCDIC (US-Canada).                                             */
#define RT_ENCODING_IBM037            1
/* OEM United States.                                                  */
#define RT_ENCODING_IBM437            2
/* IBM EBCDIC (International).                                         */
#define RT_ENCODING_IBM500            3
/* Arabic (ASMO 708).                                                  */
#define RT_ENCODING_ASMO_708          4
/* Greek (DOS).                                                        */
#define RT_ENCODING_IBM737            5
/* Baltic (DOS).                                                       */
#define RT_ENCODING_IBM775            6
/* Western European (DOS).                                             */
#define RT_ENCODING_IBM850            7
/* Central European (DOS).                                             */
#define RT_ENCODING_IBM852            8
/* OEM Cyrillic.                                                       */
#define RT_ENCODING_IBM855            9
/* Turkish (DOS).                                                      */
#define RT_ENCODING_IBM857           10
/* Portuguese (DOS).                                                   */
#define RT_ENCODING_IBM860           11
/* Icelandic (DOS).                                                    */
#define RT_ENCODING_IBM861           12
/* French Canadian (DOS).                                              */
#define RT_ENCODING_IBM863           13
/* Arabic (864).                                                       */
#define RT_ENCODING_IBM864           14
/* Nordic (DOS).                                                       */
#define RT_ENCODING_IBM865           15
/* Cyrillic (DOS).                                                     */
#define RT_ENCODING_CP866            16
/* Greek, Modern (DOS).                                                */
#define RT_ENCODING_IBM869           17
/* IBM EBCDIC (Multilingual Latin-2).                                  */
#define RT_ENCODING_IBM870           18
/* Thai (Windows).                                                     */
#define RT_ENCODING_WINDOWS_874      19
/* IBM EBCDIC (Greek Modern).                                          */
#define RT_ENCODING_CP875            20
/* Japanese (Shift-JIS).                                               */
#define RT_ENCODING_SHIFT_JIS        21
/* Chinese Simplified (GB2312).                                        */
#define RT_ENCODING_GB2312           22
/* Korean.                                                             */
#define RT_ENCODING_CP949            23
/* Chinese Traditional (Big5).                                         */
#define RT_ENCODING_BIG5             24
/* IBM EBCDIC (Turkish Latin-5).                                       */
#define RT_ENCODING_IBM1026          25
/* IBM Latin-1.                                                        */
#define RT_ENCODING_IBM1047          26
/* Unicode (UTF-16 with BOM, system endianness).                       */
#define RT_ENCODING_UTF_16           27
/* Unicode (UTF-16 without BOM little endian).                         */
#define RT_ENCODING_UTF_16LE         28
/* Unicode (UTF-16 without BOM big endian).                            */
#define RT_ENCODING_UTF_16BE         29
/* Central European (Windows).                                         */
#define RT_ENCODING_WINDOWS_1250     30
/* Cyrillic (Windows).                                                 */
#define RT_ENCODING_WINDOWS_1251     31
/* Western European (Windows).                                         */
#define RT_ENCODING_WINDOWS_1252     32
/* Greek (Windows).                                                    */
#define RT_ENCODING_WINDOWS_1253     33
/* Turkish (Windows).                                                  */
#define RT_ENCODING_WINDOWS_1254     34
/* Hebrew (Windows).                                                   */
#define RT_ENCODING_WINDOWS_1255     35
/* Arabic (Windows).                                                   */
#define RT_ENCODING_WINDOWS_1256     36
/* Baltic (Windows).                                                   */
#define RT_ENCODING_WINDOWS_1257     37
/* Vietnamese (Windows).                                               */
#define RT_ENCODING_WINDOWS_1258     38
/* Korean (Johab).                                                     */
#define RT_ENCODING_CP1361           39
/* Western European (Mac).                                             */
#define RT_ENCODING_MAC              40
/* Unicode (UTF-32 with BOM, system endianness).                       */
#define RT_ENCODING_UTF_32           41
/* Unicode (UTF-32 without BOM little endian).                         */
#define RT_ENCODING_UTF_32LE         42
/* Unicode (UTF-32 without BOM big endian).                            */
#define RT_ENCODING_UTF_32BE         43
/* US-ASCII.                                                           */
#define RT_ENCODING_US_ASCII         44
/* T.61.                                                               */
#define RT_ENCODING_T_61             45
/* ISO-6937.                                                           */
#define RT_ENCODING_ISO_6937         46
/* IBM EBCDIC (Germany).                                               */
#define RT_ENCODING_IBM273           47
/* IBM EBCDIC (Denmark-Norway).                                        */
#define RT_ENCODING_IBM277           48
/* IBM EBCDIC (Finland-Sweden).                                        */
#define RT_ENCODING_IBM278           49
/* IBM EBCDIC (Italy).                                                 */
#define RT_ENCODING_IBM280           50
/* IBM EBCDIC (Spain).                                                 */
#define RT_ENCODING_IBM284           51
/* IBM EBCDIC (UK).                                                    */
#define RT_ENCODING_IBM285           52
/* IBM EBCDIC (Japanese katakana).                                     */
#define RT_ENCODING_IBM290           53
/* IBM EBCDIC (France).                                                */
#define RT_ENCODING_IBM297           54
/* IBM EBCDIC (Arabic).                                                */
#define RT_ENCODING_IBM420           55
/* IBM EBCDIC (Greek).                                                 */
#define RT_ENCODING_IBM423           56
/* IBM EBCDIC (Hebrew).                                                */
#define RT_ENCODING_IBM424           57
/* Cyrillic (KOI8-R).                                                  */
#define RT_ENCODING_KOI8_R           58
/* IBM EBCDIC (Icelandic).                                             */
#define RT_ENCODING_IBM871           59
/* IBM EBCDIC (Cyrillic Russian).                                      */
#define RT_ENCODING_IBM880           60
/* IBM EBCDIC (Turkish).                                               */
#define RT_ENCODING_IBM905           61
/* Cyrillic (KOI8-U).                                                  */
#define RT_ENCODING_KOI8_U           62
/* Western European (ISO).                                             */
#define RT_ENCODING_ISO_8859_1       63
/* Central European (ISO).                                             */
#define RT_ENCODING_ISO_8859_2       64
/* Latin 3 (ISO).                                                      */
#define RT_ENCODING_ISO_8859_3       65
/* Baltic (ISO).                                                       */
#define RT_ENCODING_ISO_8859_4       66
/* Cyrillic (ISO).                                                     */
#define RT_ENCODING_ISO_8859_5       67
/* Arabic (ISO).                                                       */
#define RT_ENCODING_ISO_8859_6       68
/* Greek (ISO).                                                        */
#define RT_ENCODING_ISO_8859_7       69
/* Hebrew (ISO-Visual).                                                */
#define RT_ENCODING_ISO_8859_8       70
/* Turkish (ISO).                                                      */
#define RT_ENCODING_ISO_8859_9       71
/* Estonian (ISO).                                                     */
#define RT_ENCODING_ISO_8859_13      72
/* Latin 9 (ISO).                                                      */
#define RT_ENCODING_ISO_8859_15      73
/* Japanese (JIS).                                                     */
#define RT_ENCODING_ISO_2022_JP      74
/* Japanese (JIS-Allow 1 byte Kana).                                   */
#define RT_ENCODING_CS_ISO_2022_JP   75
/* Korean (ISO).                                                       */
#define RT_ENCODING_ISO_2022_KR      76
/* Japanese (EUC).                                                     */
#define RT_ENCODING_EUC_JP           77
/* Chinese Simplified (EUC).                                           */
#define RT_ENCODING_EUC_CN           78
/* Korean (EUC).                                                       */
#define RT_ENCODING_EUC_KR           79
/* Chinese Simplified (HZ).                                            */
#define RT_ENCODING_HZ_GB2312        80
/* Chinese Simplified (GB18030).                                       */
#define RT_ENCODING_GB18030          81
/* Unicode (UTF-7).                                                    */
#define RT_ENCODING_UTF_7            82
/* Unicode (UTF-8 without BOM).                                                    */
#define RT_ENCODING_UTF_8            83

/**
 * Decode from data in given encoding to a string.
 *
 * <p>
 * A RT_NULL character is put after the string.
 * </p>
 *
 * <p>
 * The <tt>unEncoding</tt> parameter can be 0 or RT_CHAR_ENCODING_XXXX.<br>
 * If <tt>nEconding</tt> is zero then:
 * </p>
 * <ul>
 *   <li>Under Windows, system ANSI input encoding is assumed.</li>
 *   <li>Under Linux, no encoding conversion is made between the input and the result.</li>
 * </ul>
 *
 * <p>
 * Under Windows, the encoding of <tt>lpOutput</tt> is UTF-16.<br>
 * Under Linux the encoding of <tt>lpOutput</tt> is system encoding if <tt>unEncoding</tt> is non-zero, lpInput otherwise.
 * </p>
 *
 * @param unInputSize Input size, in bytes. If RT_TYPE_MAX_UN, lpInput is considered NULL terminated.
 * @param unEncoding Encoding of lpInput, 0 or RT_CHAR_ENCODING_XXXX.
 * @param unBufferSize Buffer size in characters.
 * @param lpHeapBufferSize Heap buffer size in bytes.
 * @param lpOutput Can be lpInput if no decoding is required.
 * @return The character count (Not counting the trailing RT_NULL character) or RT_TYPE_MAX_UN in case of failure.
 */
RT_UN RT_API RtDecode(RT_CHAR8* lpInput, RT_UN unInputSize, RT_UN unEncoding, RT_CHAR* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, RT_CHAR** lpOutput, RT_HEAP** lpHeap);

RT_UN RT_API RtDecodeWithHeap(RT_CHAR8* lpInput, RT_UN unInputSize, RT_UN unEncoding, RT_CHAR** lpOutput, RT_HEAP** lpHeap);

RT_UN RT_API RtDecodeWithBuffer(RT_CHAR8* lpInput, RT_UN unInputSize, RT_UN unEncoding, RT_CHAR* lpBuffer, RT_UN unBufferSize);


/**
 * Encode from a string to data in given encoding.
 *
 * <p>
 * A zero byte is put after the data.
 * </p>
 *
 * <p>
 * The <tt>unEncoding</tt> parameter can be 0 or RT_CHAR_ENCODING_XXXX.<br>
 * If <tt>nEconding</tt> is zero then:
 * </p>
 * <ul>
 *   <li>Under Windows, system ANSI output encoding is assumed.</li>
 *   <li>Under Linux, no encoding conversion is made between the input and the result.</li>
 * </ul>
 *
 * <p>
 * Under Windows, the encoding of <tt>lpInput</tt> is UTF-16.<br>
 * Under Linux the encoding of <tt>lpInput</tt> is system encoding.
 * </p>
 *
 * @param unInputSize Input size, in characters. If RT_TYPE_MAX_UN, lpInput is considered NULL terminated.
 * @param unEncoding Encoding for lpOutput, 0 or RT_CHAR_ENCODING_XXXX.
 * @param unBufferSize Buffer size in bytes.
 * @param lpHeapBufferSize Heap buffer size in bytes.
 * @param lpOutput Can be lpInput if no decoding is required.
 * @return The written bytes count in <tt>lpBuffer</tt>, not including the terminating zero bytes.
 */
RT_UN RT_API RtEncode(RT_CHAR* lpInput, RT_UN unInputSize, RT_UN unEncoding, RT_CHAR8* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap);

RT_UN RT_API RtEncodeWithHeap(RT_CHAR* lpInput, RT_UN unInputSize, RT_UN unEncoding, RT_CHAR8** lpOutput, RT_HEAP** lpHeap);

RT_UN RT_API RtEncodeWithBuffer(RT_CHAR* lpInput, RT_UN unInputSize, RT_UN unEncoding, RT_CHAR8* lpBuffer, RT_UN unBufferSize);

/**
 * Compute the size of the terminating zero in bytes.
 *
 * <p>
 * An UTF-16 string cannot be terminated by a single zero byte as there are many zero bytes in this encoding.<br>
 * A proper ending in UTF-16 is two zero bytes.<br>
 * Same thing apply to UTF-32 which require 4 zero bytes.
 * </p>
 *
 * @return Size of zero bytes to terminate a string in given <tt>unEncoding</tt>: 1, 2 or 4.
 */
RT_UN RT_API RtGetTerminatingZeroSize(RT_UN unEncoding);

/**
 * The UTF-16 encoding terminates with 2 zero bytes and the UTF-32 encoding terminates with 4.<br>
 * Terminating zeros must be aligned.
 * @return The size of the buffer in byte, not including the terminating zero bytes.
 */
RT_UN RT_API RtGetDataSize(RT_CHAR8* lpData, RT_UN unTerminatingZeroSize);

#endif /* RT_ENCODING_H */
