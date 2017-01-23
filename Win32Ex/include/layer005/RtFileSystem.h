#ifndef RT_FILE_SYSTEM_H
#define RT_FILE_SYSTEM_H

#include "layer000/RtWin32ExTypes.h"

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

typedef RT_B (RT_CALL *RT_FILE_SYSTEM_BROWSE_CALLBACK)(RT_CHAR* lpPath, RT_N nType, void* lpContext);

RT_B RT_API RtBrowsePath(RT_CHAR* lpPath, RT_FILE_SYSTEM_BROWSE_CALLBACK lpCallBack, RT_B bRecursively, void* lpContext);

/**
 * <p>
 * <tt>lpBuffer</tt> can be equals to <tt>lpPath</tt>.
 * </p>
 */
RT_B RT_API RtExtractParentPath(RT_CHAR* lpPath, RT_N nPathSize, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N *lpWritten);

/**
 * Find index of the last interesting separator.<br>
 * Trailing separators are ignored.<br>
 * Return -1 if the path does not have separator or is composed of separators only.
 */
RT_N RT_API RtGetLastSeparator(RT_CHAR* lpPath, RT_N nPathSize);

void RT_API RtRemoveTrailingSeparators(RT_CHAR* lpPath, RT_N nPathSize, RT_N *lpWritten);

/**
 * Renvoie le nom du fichier ou du répertoire à partir d'un chemin
 *
 * @param lpPath Chemin dont on veut extraire le dernier élément
 * @param lpBuffer Tampon de récupération du dernier élément
 */
RT_B RT_API RtExtractFileName(RT_CHAR* lpPath, RT_N nPathSize, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N *lpWritten);

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
 * Supprime un fichier
 *
 * Considère que le fichier est supprimé s'il n'existait pas
 *
 * @param lpFilePath Le fichier à supprimer
 * @return Zero en cas d'échec
 */
RT_B RT_API RtDeleteFile(RT_CHAR* lpFilePath);

RT_B RT_API RtGetCurrentDirectory(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N *lpWritten);

RT_B RT_API RtSetCurrentDirectory(RT_CHAR* lpPath);

RT_N RT_API RtGetFileSize(RT_CHAR* lpPath);

/**
 * Return non-zero if the path is correct. Set last error in case of issue.
 * @arg nType Type of path. RT_FILE_SYSTEM_TYPE_DIRECTORY, RT_FILE_SYSTEM_TYPE_FILE or both can be used.
 */
RT_B RT_API RtCheckPath(RT_CHAR* lpPath, RT_N nType);

RT_B RT_API RtCreateDirectory(RT_CHAR* lpPath);

RT_B RT_API RtGetExecutableFilePath(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten);

RT_B RT_API RtGetApplicationDataDirectory(RT_CHAR* lpApplicationName, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten);

RT_B RT_API RtGetTempDirectory(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten);

/**
 * Append a separator to <tt>lpPath</tt> if there is not already one.<br>
 * Should always be used as "/" is a valide path that can be returned by <tt>RtExtractParentPath</tt>.<br>
 * So we cannot blindly add a separator to a path.
 */
RT_B RT_API RtAppendSeparator(RT_CHAR* lpPath, RT_N nPathSize, RT_N nBufferSize, RT_N *lpWritten);

/**
 * Concatenate <tt>lpChildName</tt> after <tt>lpParentPath</tt> managing separator.
 *
 * @param lpParentPath Can ends with a separator or not.
 */
RT_B RT_API RtBuildPath(RT_CHAR* lpParentPath, RT_N nParentPathSize, RT_CHAR* lpChildName, RT_N nBufferSize, RT_N* lpWritten);

/**
 * Build a new path concatenating <tt>lpParentPath</tt> and <tt>lpChildName</tt> and managing separator.
 *
 * @param lpParentPath Can ends with a separator or not.
 */
RT_B RT_API RtBuildNewPath(RT_CHAR* lpParentPath, RT_N nParentPathSize, RT_CHAR* lpChildName, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten);

#ifdef RT_DEFINE_WINDOWS

/**
 * Some functions of Win32 api must be called with a "long path" to handles paths longer than MAX_PATH.
 */
RT_B RT_API RtComputeLongPath(RT_CHAR* lpPath, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten);

#endif


#endif /* RT_FILE_SYSTEM_H */
