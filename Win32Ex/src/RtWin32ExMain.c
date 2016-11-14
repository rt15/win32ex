#include "layer000/RtWin32ExTypes.h"

#include "layer001/RtWin32ExOsDefines.h"

/**
 * @file
 * Implement some OS specific functions used in RtWin32ExMain.h.<br>
 * These functions cannot be used directly in RtWin32ExMain.h as it would mean including RtWin32ExOsDefines.h in it.
 *
 * <p>
 * It must not include RtWin32ExMain.h because it would define symbols like main/WinMainCRTStartup/mainCRTStartup.
 * </p>
 */

#ifdef RT_DEFINE_WINDOWS

void RT_API RtMainExitProcess(RT_UN32 unResult)
{
  ExitProcess(unResult);
}

void RT_API RtMainLocalFree(RT_CHAR** lpArgV)
{
  LocalFree(lpArgV);
}

RT_CHAR** RT_API RtMainCommandLineToArgvW(RT_N32* lpArgC)
{
  return CommandLineToArgvW(GetCommandLineW(), lpArgC);
}

#else /* NOT RT_DEFINE_WINDOWS */

void RT_API RtSetLocale()
{
  /* Ensure that functions like nl_langinfo return system info and not default "C" locale. */
  setlocale(LC_ALL, "");
}

#endif
