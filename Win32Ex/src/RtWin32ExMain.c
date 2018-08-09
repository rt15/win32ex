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
 *
 * <p>
 * Under Linux, when we launch a command in a shell, the shell builds argc/argv and calls <tt>execve</tt>.<br>
 * So it is the responsibility of the process creator to build argv/argc.<br>
 * The entry point of an ELF file is defined by the <tt>e_entry</tt> field of the ELF header (It usually points on a <tt>_start</tt> symbol).<br>
 * When the entry point is called, argc is at the top of the stack, then there is argv[0], argv[1]... then NULL, then envp[0], envp[1]... and NULL.<br>
 * As a result the entry point does not follow any classical calling convention such as <tt>cdecl</tt>.<br>
 * It is probably impossible to implement a _start function in C that reads argv.
 * </p>
 *
 * <p>
 * Under Windows, processes are created through a command line.<br>
 * So it is the responsibility of the created process to build argv/argc, if it needs them.<br>
 * The entry point is specified by the AddressOfEntryPoint field of the COFF header.<br>
 * The symbol of the entry point is often WinMainCRTStartup for a GUI application and mainCRTStartup for console application.<br>
 * The entry point doesn't take any argument. Argv/argc can be built using GetCommandLineW then CommandLineToArgvW.<br>
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
