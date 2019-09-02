#ifndef RT_FILE_UTILS_H
#define RT_FILE_UTILS_H

#include "layer000/RtWin32ExTypes.h"

/**
 * If <tt>bTruncate</tt> is true and corresponding file exists, it is replaced by an empty one.<br>
 * If <tt>bTruncate</tt> is false, there is an error if the file already exists.
 */
RT_B RT_API RtCreateEmptyFile(RT_CHAR* lpPath, RT_B bTruncate);

#endif /* RT_FILE_UTILS_H */
