#ifndef RT_SMALL_FILE_H
#define RT_SMALL_FILE_H

#include "layer000/RtWin32ExTypes.h"
#include "layer005/RtHeap.h"

/**
 * Load a file in a buffer.
 *
 * <p>
 * A RT_NULL character is put after the file content.<br>
 * As a result, the size of the buffer is the size of the file + 1.
 * </p>
 *
 * <p>
 * Beware that some unicode files starts with a BOM (Byte Order Mark).<br>
 * For UTF-8, this is not really a Byte Order Mark because there is no endianness issue with this encoding.<br>
 * It is 3 bytes that indicates that the file is encoded in UTF-8 (0xEF, 0xBB, 0xBF).<br>
 * For UTF-16, the size of the BOM is 2 bytes and it indicates the endianness.<br>
 * 0xFF, 0xFE for little endian (Used by Windows).<br>
 * 0xFE, 0xFF for big endian.
 * </p>
 *
 * @return The file size (so without trailing RT_NULL character) or RT_TYPE_MAX_UN in case of failure.
 */
RT_UN RT_API RtReadFromSmallFile(RT_CHAR* lpFilePath, RT_CHAR8** lpOutput, RT_HEAP** lpHeap);

/**
 * Allow to use a given buffer if enough.
 *
 * <p>
 * Can be for example used when loading multiple files successively.
 * </p>
 *
 * @return The file size or RT_TYPE_MAX_UN in case of failure.
 */
RT_UN RT_API RtReadFromSmallFileWithBuffer(RT_CHAR* lpFilePath, RT_CHAR8* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap);

/* Open an existing or create a new file and append data into it. */
#define RT_SMALL_FILE_MODE_APPEND 0
/* Remove an existing file if it exists and create a new one. */
#define RT_SMALL_FILE_MODE_TRUNCATE 1
/* Create a new file. If the file already exists, the function fails. */
#define RT_SMALL_FILE_MODE_NEW 2

RT_B RT_API RtWriteToSmallFile(RT_CHAR8* lpInput, RT_UN unDataSize, RT_CHAR* lpFilePath, RT_UN unMode);

#endif /* RT_SMALL_FILE_H */
