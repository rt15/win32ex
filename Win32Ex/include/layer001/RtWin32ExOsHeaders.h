#ifndef RT_WIN32_EX_OS_HEADERS_H
#define RT_WIN32_EX_OS_HEADERS_H

/* Needed for defines like RT_DEFINE_WINDOWS. */
#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * This special header is used internally only when functions from the operating system API have to be used.
 *
 * <p>
 * It must be included only in internal C sources files and never in headers files.
 * </p>
 *
 * <p>
 * It has no associated .c file.
 * </p>
 */

/* Prepare OS specific flags before including OS headers. */
#ifdef RT_DEFINE_WINDOWS

/* Only unicode is supported by the library under Windows. */
#ifndef UNICODE
/* UNICODE flag is used by win32 api. */
/* _UNICODE flag is used by Microsoft CRT implementation and should not be necessary for RtWin32ExOsHeaders.h. */
#define UNICODE
#endif

#if (!defined(_WIN32_WINNT)) || (_WIN32_WINNT < 0x0501)
#define _WIN32_WINNT 0x0501
#endif

#if (!defined(WINVER)) || (WINVER < 0x0500)
#define WINVER 0x0500
#endif

#if (!defined(_WIN32_IE)) || (_WIN32_IE < 0x0600)
#define _WIN32_IE 0x0600
#endif

#else /* NOT RT_DEFINE_WINDOWS */

#if (!defined(_XOPEN_SOURCE)) || (_XOPEN_SOURCE < 600)
#define _XOPEN_SOURCE 600
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef __USE_GNU
#define __USE_GNU
#endif

#endif

/* Include OS headers. */
#ifdef RT_DEFINE_WINDOWS

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#else /* NOT RT_DEFINE_WINDOWS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/eventfd.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <sched.h>
#include <time.h>
#include <pwd.h>
#include <locale.h>
#include <iconv.h>
#include <langinfo.h>
#include <sys/socket.h>
#include <netdb.h>
/* sudo apt-get install libssl-dev */
/* sudo apt-get install libssl-dev:i386 */
#include <openssl/rand.h>

#endif

#endif /* RT_WIN32_EX_OS_HEADERS_H */
