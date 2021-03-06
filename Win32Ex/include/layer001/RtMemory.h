#ifndef RT_MEMORY_H
#define RT_MEMORY_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Memory utility functions.
 *
 * <p>
 * To compare/copy/move/set memory, use macros.<br>
 * If the CRT is used, then CRT/intrinsics will be used.<br>
 * If CRT is not used then implementations from RtWin32ExMem.h/intrinsicts will be used.<br>
 * Intrinsics are much faster than CRT/RtWin32ExMem.h (They are "inlined" and the size is known).
 * </p>
 */

#ifdef RT_DEFINE_GCC

  #define RT_MEMORY_SWAP_BYTES16(VAR) __builtin_bswap16(VAR)
  #define RT_MEMORY_SWAP_BYTES32(VAR) __builtin_bswap32(VAR)

#else /* NOT RT_DEFINE_GCC */

  #define RT_MEMORY_SWAP_BYTES16(VAR) _byteswap_ushort(VAR)
  #define RT_MEMORY_SWAP_BYTES32(VAR) _byteswap_ulong(VAR)

#endif

/* Find the offset of given member in given structure. */
#ifdef RT_DEFINE_VC
  /* __builtin_offsetof does not exist in VC 2005. It may exist in VC 2019. */
  #define RT_MEMORY_OFFSET_OF(STRUCT, MEMBER) ((size_t)((char *)&((STRUCT *)0)->MEMBER - (char *)0))
#else
  #define RT_MEMORY_OFFSET_OF(STRUCT, MEMBER) __builtin_offsetof(STRUCT, MEMBER)
#endif

/* Find the address of the "container" of given member. */
#define RT_MEMORY_CONTAINER_OF(POINTER, STRUCT, MEMBER) ((STRUCT*)((char*)(POINTER) - RT_MEMORY_OFFSET_OF(STRUCT, MEMBER)))

/* UINT % sizeof(RT_UN). */
/* Notice that most compilers should automatically perform this optimization as sizeof(RT_UN) is known at runtime. */
/* This macro only applies to positive integers. */
#ifdef RT_DEFINE_32
  #define RT_MEMORY_MODULO_RT_UN_SIZE(UINT) (UINT & 3)
#else
  #define RT_MEMORY_MODULO_RT_UN_SIZE(UINT) (UINT & 7)
#endif

/* C modulo operator % is very often dramatically slow because it is an integer division. */
/* On the other hand, modulo power of two is very fast because it is only a bitwise operation. */
/* As a result, you should try to always use following macro instead of % operator. */
#define RT_MEMORY_MODULO_POWER_OF_TWO(UINT, POWER_OF_TWO) (UINT & (POWER_OF_TWO - 1))

/* Can be used to check that a given value is power of two to be able to use RT_MEMORY_MODULO_POWER_OF_TWO later on. */
#define RT_MEMORY_IS_POWER_OF_TWO(UINT) (UINT && ((UINT & (UINT - 1)) == 0))

/* Compute the count of chunks given <tt>unSize</tt> (The elements count) and the size of a chunk. */
/* CHUNK_SIZE must be a power of 2. */
#ifdef RT_DEFINE_GCC
  #define RT_MEMORY_GET_CHUNKS_COUNT(SIZE, CHUNK_SIZE) ((SIZE >> __builtin_ctzl(CHUNK_SIZE)) + (RT_MEMORY_MODULO_POWER_OF_TWO(SIZE, CHUNK_SIZE) ? 1 : 0))
#else
  /* TODO */
#endif

#ifndef RT_DEFINE_USE_CRT
#ifdef RT_DEFINE_VC
/* memmove is not available as intrinsic for VC and not declared outside CRT. */
void*  __cdecl memmove(void*, const void*, size_t);
#else

/* No CRT so we define built-ins. */
int __cdecl memcmp(const void*, const void*, size_t);
void* __cdecl memcpy(void*, const void*, size_t);
void* __cdecl memmove(void*, const void*, size_t);
void* __cdecl memset(void*, int, size_t);

#endif
#else

/* We will use CRT memcmp/memcpy/memmove/memset below. */
#include <string.h>

#endif

/**
 * Compare two memory areas.<br>
 * Return a positive number if the first different byte is greater in <tt>AREA1</tt>
 *
 * <p>
 * Like for memcmp, values inside areas are considered unsigned chars.
 * </p>
 *
 * @return RT_N32 (memcmp returns an int).
 */
#define RT_MEMORY_COMPARE(AREA1, AREA2, SIZE) memcmp(AREA1, AREA2, SIZE)

/**
 * Copy SIZE bytes from SOURCE to DESTINATION.
 *
 * <p>
 * Order of source and destination are the opposite of memcpy.
 * </p>
 *
 * @return <tt>DESTINATION</tt>
 */
#define RT_MEMORY_COPY(SOURCE, DESTINATION, SIZE) memcpy(DESTINATION, SOURCE, SIZE)

/**
 * Copy SIZE bytes from SOURCE to DESTINATION.
 *
 * <p>
 * Act like if a temporary buffer would have been used to avoid overwriting issues.
 * </p>
 *
 * @return <tt>DESTINATION</tt>
 */
#define RT_MEMORY_MOVE(SOURCE, DESTINATION, SIZE) memmove(DESTINATION, SOURCE, SIZE)

/**
 * Set SIZE bytes with VALUE in AREA.
 *
 * @param VALUE Value set to each byte in the area.
 */
#define RT_MEMORY_SET(AREA, VALUE, SIZE) memset(AREA, VALUE, SIZE)

#define RT_MEMORY_ZERO(AREA, SIZE) memset(AREA, 0, SIZE)

RT_N32 RT_API RtMemory_Compare(void* lpArea1, void* lpArea2, RT_UN unSize);
void* RT_API RtMemory_Copy(void* lpSource, void* lpDestination, RT_UN unSize);
void* RT_API RtMemory_Move(void* lpSource, void* lpDestination, RT_UN unSize);
void* RT_API RtMemory_Set(void* lpArea, RT_N32 nValue, RT_UN unSize);

void* RT_API RtMemory_Zero(void* lpArea, RT_UN unSize);
void RT_API RtMemory_Swap(void* lpArea1, void* lpArea2, RT_UN unSize);

/**
 * Copy from lpSource to lpDestination, swapping bytes
 *
 * <p>
 * Useful to convert 16 bits words between big endian and little endian.
 * </p>
 *
 * @param unSize Size in 16 bits words.
 * @return The destination in all cases.
 */
RT_UN16* RT_API RtMemory_SwapBytes16(RT_UN16* lpSource, RT_UN16* lpDestination, RT_UN unSize);

/**
 * Copy from lpSource to lpDestination, swapping bytes.
 *
 * <p>
 * Useful to convert 32 bits words between big endian and little endian.
 * </p>
 *
 * @param unSize Size in 32 bits words.
 * @return The destination in all cases.
 */
RT_UN32* RT_API RtMemory_SwapBytes32(RT_UN32* lpSource, RT_UN32* lpDestination, RT_UN unSize);

#endif /* RT_MEMORY_H */
