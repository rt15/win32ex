#ifndef RT_BASE_64_H
#define RT_BASE_64_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Base64 encoding/decoding.
 *
 * <p>
 * Base64 is designed to store binary data into simple characters widely supported by encodings.
 * </p>
 *
 * <p>
 * Each group of 6 bits are converted in a character in 'A-Z', 'a-z', '0-9', '+' and '/' (2^6 = 64 characters).<br>
 * As a result, the input length is considered to be a multiple of 6 bits (And obviously 8 bits), so 3, 6, 9, ... characters.<br>
 * The size of the result is the considered input bits count divided by 6, so 4, 8, 12.<br>
 * The last 6 bits group of input is padded with zeros if necessary and the resulting base64 string is padded with '=' to be a multiple of 4.
 * </p>
 *
 * <ol>
 *   <li>The last bits of the 6 bits block are set to zero and the 6 bits block is converted normally.</li>
 *   <li>Some '=' can be added to the base64 characters to complete
 * </ol>
 *
 */

RT_B RT_API RtBase64_Encode(RT_CHAR8* lpString, RT_UN unStringSize, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

RT_B RT_API RtBase64_Decode(RT_CHAR8* lpBase64, RT_UN unBase64Size, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

#endif /* RT_BASE_64_H */
