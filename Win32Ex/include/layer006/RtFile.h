#ifndef RT_FILE_H
#define RT_FILE_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Simple file management.
 */

typedef struct _RT_FILE
{
#ifdef RT_DEFINE_WINDOWS
  RT_H hFile;
#else /* NOT RT_DEFINE_WINDOWS */
  RT_N32 nFile;
#endif
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
 * Ouvre le fichier spécifié
 *
 * Le fichier est créé s'il n'existe pas.
 *
 * @param lpFilePath Le fichier à ouvrir
 * @param nMode Le mode d'ouverture RT_FILE_MODE_...
 * @return Zero en cas d'échec
 */
RT_B RT_API RtCreateFile(RT_FILE* lpFile, RT_CHAR* lpFileName, RT_UN unMode);

/**
 * Create a temporary file.
 *
 * @param lpBuffer Receive the path to the file so the caller can delete the file.
 * @param unBufferSize Should be RT_FILE_SYSTEM_MAX_FILE_PATH.
 */
RT_B RT_API RtCreateTempFile(RT_FILE* lpFile, RT_CHAR* lpPrefix, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten);

/**
 * Create a temporary file in given path.
 *
 * @param lpParentPath Directory in which the temporary file is created.
 * @param lpBuffer Receive the path to the file so the caller can delete the file.
 * @param unBufferSize Should be RT_FILE_SYSTEM_MAX_FILE_PATH.
 */
RT_B RT_API RtCreateTempFileWithParentPath(RT_FILE* lpFile, RT_CHAR* lpPrefix, RT_CHAR* lpParentPath, RT_UN unParentPathSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten);

/**
 * <p>
 * Do not call RtFreeFile on this RT_FILE.
 * </p>
 */
RT_B RT_API RtCreateStdInput(RT_FILE* lpFile);

/**
 * <p>
 * Do not call RtFreeFile on this RT_FILE.
 * </p>
 */
RT_B RT_API RtCreateStdOutput(RT_FILE* lpFile);

/**
 * <p>
 * Do not call RtFreeFile on this RT_FILE.
 * </p>
 */
RT_B RT_API RtCreateStdError(RT_FILE* lpFile);

/**
 * Lit un certain nombre d'octets dans un fichier
 *
 * @param lpBuffer Récupération des données
 * @param nNumberOfBytesToRead Nombre d'octets à lire
 * @return Zero en cas d'échec
 */
RT_B RT_API RtReadFromFile(RT_FILE* lpFile, RT_CHAR8* lpBuffer, RT_UN unBytesToRead);

/**
 * Ecrit un certain nombre d'octets dans un fichier
 *
 * @param lpData Données à écrire
 * @param nNumberOfBytesToWrite Nombre d'octets à écrire
 * @return Zero en cas d'échec
 */
RT_B RT_API RtWriteToFile(RT_FILE* lpFile, RT_CHAR8* lpData, RT_UN unBytesToWrite);

/**
 * Compute the size of an open file.
 *
 * @return RT_TYPE_MAX_UN in case of error (And the file pointer might be corrupted).
 */
RT_UN RT_API RtComputeFileSize(RT_FILE* lpFile);

/**
 * Déplace le pointeur dans le fichier
 *
 * @param nOffset Déplacement à effectué
 * @param nFrom Type de positionnement : RT_FILE_POS_...
 * @return Zero en cas d'échec
 */
RT_B RT_API RtSetFilePointer(RT_FILE* lpFile, RT_N nOffset, RT_UN unFrom);

/**
 * Récupère la position courante dans le fichier
 *
 * @return La position courante, RT_TYPE_MAX_UN en cas de problème.
 */
RT_UN RT_API RtGetFilePointer(RT_FILE* lpFile);

/**
 * Ferme le fichier spécifié
 */
RT_B RT_API RtFreeFile(RT_FILE* lpFile);

#endif /* RT_FILE_H */
