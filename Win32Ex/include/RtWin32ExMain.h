#ifndef RT_WIN32_EX_MAIN_H
#define RT_WIN32_EX_MAIN_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * This special header can be used by the consumer to easily write its application entry point.<br>
 * If the CRT is not used, then the compiler expect the application to provide either WinMainCRTStartup or mainCRTStartup.<br>
 * This header provides a unified entry point with argc/argv.<br>
 * It is optional, the win32ex library can be used without defining a RtMain entry point.
 *
 * <p>
 * It is an header as the library cannot export symbols like main/WinMainCRTStartup/mainCRTStartup.<br>
 * It must not be included in the library .c files.
 * </p>
 *
 * <p>
 * Customer application should never call directly ExitProcess/exit to terminate.<br>
 * It should return from the main method instead.
 * </p>
 *
 * <p>
 * Define RT_DEFINE_USE_CRT when your application is using CRT on Windows. CRT is always used on linux.
 * Under Windows, either _CONSOLE or _WINDOWS (For a Windows GUI) must be defined.
 * </p>
 */

/**
 * Declaration that must be used for API consumer entry point.
 *
 * @param nArgC Size of <tt>lpArgV</tt>.
 * @param lpArgV Program arguments. <code>lpArgV[0]</code> is the name of the program if <tt>lpArgV</tt> is greater than zero.
 * @return Process exit code. Should be zero if the execution is a success.
 */
RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[]);

#ifdef RT_DEFINE_WINDOWS

#if defined(_WINDOWS) && defined(_CONSOLE)
  #error Both _WINDOWS and _CONSOLE are defined.
#endif

#if !defined(_WINDOWS) && !defined(_CONSOLE)
  #error Either _WINDOWS or _CONSOLE must be defined.
#endif

/**
 * Should not be use outside this file.
 */
void RT_API RtMain_ExitProcess(RT_UN32 unResult);

/**
 * Should not be use outside this file.
 */
void RT_API RtMain_LocalFree(RT_CHAR** lpArgV);

/**
 * Should not be use outside this file.
 */
RT_CHAR** RT_API RtMain_CommandLineToArgvW(RT_N32* lpArgC);

#else /* NOT RT_DEFINE_WINDOWS */

void RT_API RtMain_SetLocale();

#endif


#ifdef RT_DEFINE_WINDOWS
#ifdef RT_DEFINE_USE_CRT
int RT_CDECL main(int argc, char* argv[])
#else /* NOT RT_DEFINE_USE_CRT */
#ifdef _WINDOWS
int RT_CDECL WinMainCRTStartup()
#else /* NOT _WINDOWS */
int RT_CDECL mainCRTStartup()
#endif
#endif
{
  RT_CHAR** lpArgV;
  RT_N32 nArgC;
  RT_UN32 unResult;

  lpArgV = RtMain_CommandLineToArgvW(&nArgC);
  unResult = RtMain(nArgC, lpArgV);
  RtMain_LocalFree(lpArgV);

#ifndef RT_DEFINE_USE_CRT
  /* It is safer to call ExitProcess instead of simply returning from the entry point (There might be other threads). */
  RtMain_ExitProcess(unResult);
#endif
  /* Let CRT clean up. */
  return unResult;
}
#else /* NOT RT_DEFINE_WINDOWS */
int RT_CDECL main(int argc, char* argv[])
{
  /* Default locale is "C". This call will align locale with system locale. */
  RtMain_SetLocale();
  return RtMain(argc, argv);
}
#endif

#endif /* RT_WIN32_EX_MAIN_H */
