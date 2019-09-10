#ifndef RT_FILE_H
#define RT_FILE_H

#include "layer000/RtWin32ExTypes.h"
#include "layer002/RtIoDevice.h"

/**
 * @file
 * Simple file management.
 */

typedef struct _RT_FILE
{
  RT_IO_DEVICE rtIoDevice;
}
RT_FILE;

#define RT_FILE_POS_BEGIN 0
#define RT_FILE_POS_CURRENT 1
#define RT_FILE_POS_END 2

/* Open the file for read only access. Fails if the file does not exist. */
#define RT_FILE_MODE_READ 0
/* Open an existing or create a new file for read/write access. */
#define RT_FILE_MODE_READ_WRITE 1
/* Remove an existing file if it exists and create a new one with read/write access. */
#define RT_FILE_MODE_TRUNCATE 2
/* Create a new file with read/write access. If the file already exists, the function fails. */
#define RT_FILE_MODE_NEW 3

/**
 * Open the specified file.<br>
 * Use <tt>RtIoDevice_Free</tt> to close the file.
 *
 * <p>
 * The created file is not inheritable.
 * </p>
 */
RT_B RT_API RtFile_Create(RT_FILE* lpFile, RT_CHAR* lpFilePath, RT_UN unMode);

/**
 * Compute the size of an open file.
 *
 * @return RT_FAILURE in case of error (And the file pointer might be corrupted).
 */
RT_B RT_API RtFile_GetSize(RT_FILE* lpFile, RT_UN* lpFileSize);

RT_B RT_API RtFile_SetPointer(RT_FILE* lpFile, RT_N nOffset, RT_UN unFrom);

RT_B RT_API RtFile_GetPointer(RT_FILE* lpFile, RT_UN* lpOffset);

RT_B RT_API RtFile_Free(RT_FILE* lpFile);

#endif /* RT_FILE_H */
