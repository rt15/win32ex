#ifndef RT_FILE_SYSTEM_H
#define RT_FILE_SYSTEM_H

#include "layer000/RtWin32ExTypes.h"
#include "layer004/RtChar.h"

/**
 * @file
 * File system management.
 */

/**
 * Maximum file depth supported by the library.<br>
 * Many Windows programs are limited to MAX_PATH, 260 characters.
 *
 * <p>
 * This is set so that a function can have 2 local variable paths and few other things without reaching 4k size.
 * </p>
 */
#define RT_FILE_SYSTEM_MAX_FILE_PATH RT_CHAR_HALF_BIG_STRING_SIZE

/**
 * Maximum length for a file name (Without path).
 */
#define RT_FILE_SYSTEM_MAX_FILE_NAME 256

#ifdef RT_DEFINE_WINDOWS
#define RT_FILE_SYSTEM_SEPARATOR '\\'
#define RT_FILE_SYSTEM_SEPARATOR_STRING _R("\\")
#define RT_FILE_SYSTEM_IS_SEPARATOR(X) (X == '\\' || X == '/')
#else /* NOT RT_DEFINE_WINDOWS */
#define RT_FILE_SYSTEM_SEPARATOR '/'
#define RT_FILE_SYSTEM_SEPARATOR_STRING _R("/")
#define RT_FILE_SYSTEM_IS_SEPARATOR(X) (X == '/')
#endif

/* Possible type values for RT_FILE_SYSTEM_BROWSE_CALLBACK. */
#define RT_FILE_SYSTEM_TYPE_DIRECTORY 1
#define RT_FILE_SYSTEM_TYPE_FILE 2

typedef RT_B (RT_CALL *RT_FILE_SYSTEM_BROWSE_CALLBACK)(RT_CHAR* lpPath, RT_UN unType, void* lpContext);

/**
 * @param bChildrenFirst If RT_TRUE, the callback is called with children first.
 */
RT_B RT_API RtBrowsePath(RT_CHAR* lpPath, RT_FILE_SYSTEM_BROWSE_CALLBACK lpCallBack, RT_B bRecursively, RT_B bChildrenFirst, void* lpContext);

RT_B RT_API RtGetParentPath(RT_CHAR* lpPath, RT_UN unPathSize, RT_UN unBufferSize, RT_UN *lpWritten);

/**
 * <p>
 * <tt>lpBuffer</tt> can be equals to <tt>lpPath</tt>.
 * </p>
 */
RT_B RT_API RtGetNewParentPath(RT_CHAR* lpPath, RT_UN unPathSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten);

/**
 * Find index of the last interesting separator.<br>
 * Trailing separators are ignored.<br>
 * Return RT_TYPE_MAX_UN if the path does not have separator or is composed of separators only.
 */
RT_UN RT_API RtGetLastSeparator(RT_CHAR* lpPath, RT_UN unPathSize);

void RT_API RtRemoveTrailingSeparators(RT_CHAR* lpPath, RT_UN unPathSize, RT_UN *lpWritten);

/**
 * Renvoie le nom du fichier ou du répertoire à partir d'un chemin
 *
 * @param lpPath Chemin dont on veut extraire le dernier élément
 * @param lpBuffer Tampon de récupération du dernier élément
 */
RT_B RT_API RtGetFileName(RT_CHAR* lpPath, RT_UN unPathSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten);

/**
 * Move a file.
 *
 * @param lpCurrentFilePath Current file path.
 * @param lpNewPath Expected new file path (Must not be a directory).
 * @return Zero in case of failure.
 */
RT_B RT_API RtMoveFile(RT_CHAR* lpCurrentFilePath, RT_CHAR* lpFileNewPath);

RT_B RT_API RtRenameFile(RT_CHAR* lpCurrentFilePath, RT_CHAR* lpNewFileName);

RT_B RT_API RtCopyFile(RT_CHAR* lpSource, RT_CHAR* lpDestination);

/**
 *
 * <p>
 * The file must exist.
 * </p>
 */
RT_B RT_API RtDeleteFile(RT_CHAR* lpFilePath);

/**
 * Attempt to delete the file and check error code in case of error.<br>
 * Ignore the error if the file has not been found.
 */
RT_B RT_API RtDeleteFileIfExists(RT_CHAR* lpFilePath);

RT_B RT_API RtGetCurrentDirectory(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten);

RT_B RT_API RtSetCurrentDirectory(RT_CHAR* lpPath);

RT_B RT_API RtGetFileSystemFileSize(RT_CHAR* lpPath, RT_UN64* lpFileSize);

/**
 * Return RT_SUCCESS if the path is correct. Set last error in case of issue.
 *
 * <p>
 * Return RT_FAILURE if the path does not exist or if the function fails to check the path.
 * </p>
 *
 * @arg unType Type of path. RT_FILE_SYSTEM_TYPE_DIRECTORY, RT_FILE_SYSTEM_TYPE_FILE or both can be used.
 */
RT_B RT_API RtCheckPath(RT_CHAR* lpPath, RT_UN unType);

/**
 * Return RT_SUCCESS if the file is correct. Set last error in case of issue.<br>
 * Return RT_FAILURE if it is a directory, if the file does not exist or if the function fails to check the file.
 */
RT_B RT_API RtCheckFile(RT_CHAR* lpPath);

/**
 * Return RT_SUCCESS if the directory is correct. Set last error in case of issue.<br>
 * Return RT_FAILURE if it is a file, if the directory does not exist or if the function fails to check the directory.
 */
RT_B RT_API RtCheckDirectory(RT_CHAR* lpPath);

/**
 * Return RT_SUCCESS if path corresponds to a file or directory is correct. Set last error in case of issue.<br>
 * Return RT_FAILURE if there is no file or directory does not exist or if the function fails to check the directory.
 */
RT_B RT_API RtCheckFileOrDirectory(RT_CHAR* lpPath);

/**
 *
 *
 * <p>
 * The directory must not exist already.
 * </p>
 */
RT_B RT_API RtCreateDirectory(RT_CHAR* lpPath);

/**
 * Create all necessary directories to create given <tt>lpPath</tt>.
 */
RT_B RT_API RtCreateDirectories(RT_CHAR* lpPath);

/**
 *
 * <p>
 * The directory must exist and be empty.
 * </p>
 */
RT_B RT_API RtDeleteDirectory(RT_CHAR* lpPath);

/**
 * Attempt to delete the directory and check error code in case of error.<br>
 * Ignore the error if the directory has not been found.
 *
 * <p>
 * The directory must be empty.
 * </p>
 */
RT_B RT_API RtDeleteDirectoryIfExists(RT_CHAR* lpPath);

/**
 * Delete given directory and its content, recursively.
 *
 * <p>
 * The directory can exist or not.
 * </p>
 */
RT_B RT_API RtDeleteDirectoryRecursively(RT_CHAR* lpPath);

RT_B RT_API RtGetExecutableFilePath(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

RT_B RT_API RtGetApplicationDataDirectory(RT_CHAR* lpApplicationName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

RT_B RT_API RtGetTempDirectory(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

/**
 * Append a separator to <tt>lpPath</tt> if there is not already one.<br>
 * Should always be used as "/" is a valide path that can be returned by <tt>RtGetNewParentPath</tt>.<br>
 * So we cannot blindly add a separator to a path.
 */
RT_B RT_API RtAppendSeparator(RT_CHAR* lpPath, RT_UN unPathSize, RT_UN unBufferSize, RT_UN *lpWritten);

/**
 * Concatenate <tt>lpChildName</tt> after <tt>lpParentPath</tt> managing separator.
 *
 * @param lpParentPath Can ends with a separator or not.
 */
RT_B RT_API RtBuildPath(RT_CHAR* lpParentPath, RT_UN unParentPathSize, RT_CHAR* lpChildName, RT_UN unBufferSize, RT_UN* lpWritten);

/**
 * Build a new path concatenating <tt>lpParentPath</tt> and <tt>lpChildName</tt> and managing separator.
 *
 * @param lpParentPath Can ends with a separator or not.
 * @param lpWritten Incremented with the characters written to lpParentPath.
 */
RT_B RT_API RtBuildNewPath(RT_CHAR* lpParentPath, RT_UN unParentPathSize, RT_CHAR* lpChildName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

#ifdef RT_DEFINE_WINDOWS

/**
 * Some functions of Win32 api must be called with a "long path" to handles paths longer than MAX_PATH.
 */
RT_B RT_API RtGetLongPath(RT_CHAR* lpPath, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

#endif


#endif /* RT_FILE_SYSTEM_H */
