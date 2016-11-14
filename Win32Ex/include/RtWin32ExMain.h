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
 * Define RT_DEFINE_GUI before including this header to build a GUI application.<br>
 * Define RT_DEFINE_USE_CRT when your application is using CRT on Windows. CRT is always used on linux.
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

/**
 * Should not be use outside this file.
 */
void RT_API RtMainExitProcess(RT_UN32 unResult);

/**
 * Should not be use outside this file.
 */
void RT_API RtMainLocalFree(RT_CHAR** lpArgV);

/**
 * Should not be use outside this file.
 */
RT_CHAR** RT_API RtMainCommandLineToArgvW(RT_N32* lpArgC);

#else /* NOT RT_DEFINE_GUI */

void RT_API RtSetLocale();

#endif


#ifdef RT_DEFINE_WINDOWS
#ifdef RT_DEFINE_USE_CRT
int RT_CDECL main(int argc, char* argv[])
#else /* NOT RT_DEFINE_USE_CRT */
#ifdef RT_DEFINE_GUI
int RT_CDECL WinMainCRTStartup()
#else /* NOT RT_DEFINE_GUI */
int RT_CDECL mainCRTStartup()
#endif
#endif
{
  RT_CHAR** lpArgV;
  RT_N32 nArgC;
  RT_UN32 unResult;

  lpArgV = RtMainCommandLineToArgvW(&nArgC);
  unResult = RtMain(nArgC, lpArgV);
  RtMainLocalFree(lpArgV);

#ifndef RT_DEFINE_USE_CRT
  /* It is safer to call ExitProcess instead of simply returning from the entry point (There might be other threads). */
  RtMainExitProcess(unResult);
#endif
  /* Let CRT clean up. */
  return unResult;
}
#else /* NOT RT_DEFINE_WINDOWS */
int RT_CDECL main(int argc, char* argv[])
{
  /* Default locale is "C". This call will align locale with system locale. */
  RtSetLocale();
  return RtMain(argc, argv);
}
#endif

#endif /* RT_WIN32_EX_MAIN_H */
