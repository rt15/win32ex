#ifndef RT_SYSTEM_INFO_H
#define RT_SYSTEM_INFO_H

#include "layer000/RtWin32ExTypes.h"

/**
 * Returns the major version and optionally the minor and patch versions.
 *
 * <p>
 * Under Windows, lpPatch is the major SP version, 3 for SP3.
 * </p>
 *
 * <p>
 * Under Linux, it is the kernel version, not the distribution version.
 * </p>
 *
 * <pre>
 * Windows 10                 10.0
 * Windows Server 2016        10.0
 * Windows 8.1                 6.3
 * Windows Server 2012 R2      6.3
 * Windows 8                   6.2
 * Windows Server 2012         6.2
 * Windows 7                   6.1
 * Windows Server 2008 R2      6.1
 * Windows Server 2008         6.0
 * Windows Vista               6.0
 * Windows Server 2003 R2      5.2
 * Windows Server 2003         5.2
 * Windows XP 64-Bit Edition   5.2
 * Windows XP                  5.1
 * Windows 2000                5.0
 * </pre>
 *
 * @param lpMajor Cannot be RT_NULL.
 * @param lpMinor Can be RT_NULL.
 * @param lpPatch Can be RT_NULL.
 */
RT_B RT_API RtGetOsVersion(RT_UN* lpMajor, RT_UN* lpMinor, RT_UN* lpPatch);

RT_B RT_API RtIsOsVersionEqualOrGreaterTo(RT_UN unMajor, RT_UN unMinor, RT_UN unPatch, RT_B* lpResult);

#endif /* RT_SYSTEM_INFO_H */
