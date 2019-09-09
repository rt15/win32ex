#ifndef RT_TEMP_FILE_H
#define RT_TEMP_FILE_H

#include "layer000/RtWin32ExTypes.h"
#include "layer005/RtFile.h"

/**
 * @file
 * Temporary files creation.
 */

/**
 * Create a temporary file.
 *
 * <p>
 * The created file is not inheritable.
 * </p>
 *
 * @param lpBuffer Receive the path to the file so the caller can delete the file.
 * @param unBufferSize Should be RT_FILE_SYSTEM_MAX_FILE_PATH.
 */
RT_B RT_API RtTempFile_Create(RT_FILE* lpFile, RT_CHAR* lpPrefix, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten);

/**
 * Create a temporary file in given path.
 *
 * <p>
 * The created file is not inheritable.
 * </p>
 *
 * @param lpParentPath Directory in which the temporary file is created.
 * @param lpBuffer Receive the path to the file so the caller can delete the file.
 * @param unBufferSize Should be RT_FILE_SYSTEM_MAX_FILE_PATH.
 */
RT_B RT_API RtTempFile_CreateWithParentPath(RT_FILE* lpFile, RT_CHAR* lpPrefix, RT_CHAR* lpParentPath, RT_UN unParentPathSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten);

#endif /* RT_TEMP_FILE_H */
