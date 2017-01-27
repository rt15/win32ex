#ifndef RT_WIN32_EX_TYPES_H
#define RT_WIN32_EX_TYPES_H

/* Allows varargs usage which is more a C feature than   */
/* a library feature.                                    */
/* It is a C runtime header included in all cases but    */
/* it should have a very small footprint.                */
#include <stdarg.h>

/**
 * @file
 * This special header declare the public types used in win32ex library.
 *
 * <p>
 * It is included in most if not all win32ex library headers.
 * </p>
 *
 * <p>
 * It has no associated .c file.
 * </p>
 */


/* Operating system. */
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
  #define RT_DEFINE_WINDOWS
#else
  #define RT_DEFINE_LINUX
  /* CRT is always used on Linux. Beware that this flag cannot be used in library .c files as they are compiled once. */
  #ifndef RT_DEFINE_USE_CRT
    #define RT_DEFINE_USE_CRT
  #endif
#endif

/* Compiler. */
#ifdef __GNUC__
  #define RT_DEFINE_GCC
#else
  #define RT_DEFINE_VC
#endif

/* Targeted architecture. */
/* http://nadeausoftware.com/articles/2012/02/c_c_tip_how_detect_processor_type_using_compiler_predefined_macros */
#ifdef RT_DEFINE_VC
  #ifdef _M_X64
    #define RT_DEFINE_64
  #else
    #define RT_DEFINE_32
  #endif
#else
  #ifdef __x86_64__
    #define RT_DEFINE_64
  #else
    #define RT_DEFINE_32
  #endif
#endif

/* Compilers related defines, mostly calling conventions. */
#ifdef RT_DEFINE_GCC
  #define RT_CDECL __attribute__((cdecl))
  #define RT_STDCALL __attribute__((stdcall))
  #define RT_UNUSED(x) UNUSED_ ## x __attribute__((unused))
#else
  #define RT_CDECL __cdecl
  #define RT_STDCALL __stdcall
  #define RT_UNUSED(x) /*@unused@*/ x
#endif

/* Calling convention for private functions. */
#define RT_CALL RT_STDCALL
/* Calling convention for public stdcall functions. */
#define RT_API RT_STDCALL
/* Calling convention for few public cdecl functions. */
#define RT_CDECL_API RT_CDECL

/* On Visual C++ and GCC, char is most likely to be signed (But different from "signed char") however there are flags to make it unsigned. */
typedef char RT_CHAR8;
typedef unsigned char RT_UCHAR8;      /* BYTE             */
typedef unsigned short RT_UN16;       /* WORD             */
typedef short RT_N16;

/* Use 16 bits characters under windows, 8 bits otherwise. */
/* RT_CHAR is most likely to be signed (or not) RT_CHAR8 under Linux and unsigned RT_UN16 under Windows. */
#ifdef RT_DEFINE_WINDOWS
  #define _R(x) L ## x
  typedef RT_UN16 RT_CHAR;
  typedef RT_UN16 RT_UCHAR;
#else
  #define _R(x) x
  typedef RT_CHAR8 RT_CHAR;
  typedef RT_UCHAR8 RT_UCHAR;
#endif

/* Fixed size types */
typedef int RT_B;                    /* BOOL               */
typedef int RT_N32;                  /* INT/INT32/LONG     */
#if defined(RT_DEFINE_WINDOWS) && defined(RT_DEFINE_64)
  typedef unsigned int RT_UN32;      /* DWORD, UINT        */
#else
  typedef unsigned long RT_UN32;     /* DWORD, ULONG       */
#endif

#ifdef RT_DEFINE_GCC
typedef long long RT_N64;
typedef unsigned long long RT_UN64;
#else
typedef __int64 RT_N64;
typedef unsigned __int64 RT_UN64;
#endif

/* 32/64 bits dependent types. The size of RT_N and RT_UN must be the size of a pointer. */
#if defined(RT_DEFINE_WINDOWS) && defined(RT_DEFINE_64)
  typedef __int64 RT_N;              /* long is 32 bits on msvc win 64 */
  typedef unsigned __int64 RT_UN;
#else
  #ifdef RT_DEFINE_GCC
    typedef long RT_N;                 /* Should be ssize_t on most systems (int on linux i386, long on x86_64). Should also be off_t. */
    typedef unsigned long RT_UN;       /* Should be size_t (Result of sizeof, can be considered primitive) but not on VC 32 bits which is using unsigned int. */
  #else
    /* _W64 is used to signal to VC compiler that even if the types are 32 bits, they will be 64 bits when targeting 64 bits. */
    typedef _W64 long RT_N;
    typedef _W64 unsigned long RT_UN;
  #endif
#endif
typedef void* RT_H;                  /* HANDLE.  */

/* RT_N = LRESULT, LONG_PTR, LPARAM. */
/* RT_UN = UINT_PTR, ULONG_PTR, DWORD_PTR, WPARAM. */

/* Convert pointers into integers and integers to pointers. */

/* Can be used when the integer is 32 bits while targeting 32 bits. */
#define RT_TYPE_MAKE_POINTER(x)    ((void*)(RT_N)(x))

#ifdef RT_DEFINE_64
  #define RT_TYPE_MAKE_INTEGER(x)    ((RT_N)(x))
  #define RT_TYPE_MAKE_UINTEGER(x)   ((RT_UN)(x))
#else
  /* Convert to real 32 bits integer types (Not _W64 integers) because the pointers are 32 bits. */
  #define RT_TYPE_MAKE_INTEGER(x)    ((RT_N32)(RT_N)(x))
  #define RT_TYPE_MAKE_UINTEGER(x)   ((RT_UN32)(RT_N)(x))
#endif

/* Maximum and minimum values for types. */

#define RT_TYPE_MAX_N16    32767
#define RT_TYPE_MIN_N16    (-RT_TYPE_MAX_N16 - 1)
#define RT_TYPE_MAX_UN16   ((RT_UN16)-1)
#define RT_TYPE_MAX_N32    2147483647
#define RT_TYPE_MIN_N32    (-RT_TYPE_MAX_N32 - 1)
#define RT_TYPE_MAX_UN32   ((RT_UN32)-1)
#define RT_TYPE_MAX_N64    9223372036854775807
#define RT_TYPE_MIN_N64    (-RT_TYPE_MAX_N64 - 1)
#define RT_TYPE_MAX_UN64   ((RT_UN64)-1)
#ifdef RT_DEFINE_32
  #define RT_TYPE_MAX_N    RT_TYPE_MAX_N32
  #define RT_TYPE_MIN_N    RT_TYPE_MIN_N32
#else
  #define RT_TYPE_MAX_N    RT_TYPE_MAX_N64
  #define RT_TYPE_MIN_N    RT_TYPE_MIN_N64
#endif
#define RT_TYPE_MAX_UN     ((RT_UN)-1)

/* Never use RT_TRUE in comparisons. RT_B is a zero or non-zero value. */
#define RT_FALSE 0
#define RT_TRUE 1
#define RT_FAILURE RT_FALSE
#define RT_SUCCESS RT_TRUE


#ifdef __cplusplus
  #define RT_NULL 0
#else
  #define RT_NULL ((void*)0)
#endif

#ifdef RT_DEFINE_VC

/* Needed for Visual Studio compiler built-in, should be light... */
#include <intrin.h>

#endif

#endif /* RT_WIN32_EX_TYPES_H */
