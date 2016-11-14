#ifndef RT_MEMORY_H
#define RT_MEMORY_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Memory utility functions.
 */

#ifdef RT_DEFINE_GCC

#define RT_MEMORY_SWAP_BYTES16(VAR) __builtin_bswap16(VAR)
#define RT_MEMORY_SWAP_BYTES32(VAR) __builtin_bswap32(VAR)

#else /* NOT RT_DEFINE_GCC */

#define RT_MEMORY_SWAP_BYTES16(VAR) _byteswap_ushort(VAR)
#define RT_MEMORY_SWAP_BYTES32(VAR) _byteswap_ulong(VAR)

#endif

/**
 * <p>
 * Exactly same prototype as <tt>memcmp</tt> except calling convention.
 * </p>
 */
RT_N RT_API RtCompareMemory(void* lpArea1, void* lpArea2, RT_UN unSize);

/**
 * <p>
 * Exactly same prototype as <tt>memcpy</tt> except calling convention.
 * </p>
 */
void* RT_API RtCopyMemory(void* lpSource, void* lpDestination, RT_UN unSize);

/**
 * <p>
 * Exactly same prototype as <tt>memmove</tt> except calling convention.
 * </p>
 */
void* RT_API RtMoveMemory(void* lpSource, void* lpDestination, RT_UN unSize);

/**
 * <p>
 * Exactly same prototype as <tt>memset</tt> except calling convention.
 * </p>
 */
void* RT_API RtSetMemory(void* lpArea, RT_N nValue, RT_UN unSize);

void* RT_API RtZeroMemory(void* lpArea, RT_UN unSize);
void RT_API RtSwapMemory(void* lpArea1, void* lpArea2, RT_UN unSize);

/**
 * Copy from lpSource to lpDestination, swapping bytes
 *
 * <p>
 * Useful to convert 16 bits words between big endian and little endian.
 * </p>
 *
 * @param nSize Size in 16 bits words.
 * @return The destination in all cases.
 */
RT_UN16* RT_API RtSwapBytes16(RT_UN16* lpSource, RT_UN16* lpDestination, RT_UN unSize);

/**
 * Copy from lpSource to lpDestination, swapping bytes.
 *
 * <p>
 * Useful to convert 32 bits words between big endian and little endian.
 * </p>
 *
 * @param nSize Size in 32 bits words.
 * @return The destination in all cases.
 */
RT_UN32* RT_API RtSwapBytes32(RT_UN32* lpSource, RT_UN32* lpDestination, RT_UN unSize);

/**
 * Compute the count of chunks given <tt>nSize</tt> (The elements count) and the size of a chunk.
 *
 * <p>
 * Return RT_TYPE_MAX_UN in case of error.
 * </p>
 */
RT_UN RT_API RtComputeChunksCount(RT_UN32 unSize, RT_UN32 unChunkSize);

#endif /* RT_MEMORY_H */
