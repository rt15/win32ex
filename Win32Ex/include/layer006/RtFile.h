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
 * Ouvre le fichier sp�cifi�
 *
 * Le fichier est cr�� s'il n'existe pas.
 *
 * @param lpFilePath Le fichier � ouvrir
 * @param nMode Le mode d'ouverture RT_FILE_MODE_...
 * @return Zero en cas d'�chec
 */
RT_B RT_API RtCreateFile(RT_FILE* lpFile, RT_CHAR* lpFileName, RT_UN unMode);

/**
 * Lit un certain nombre d'octets dans un fichier
 *
 * @param lpBuffer R�cup�ration des donn�es
 * @param nNumberOfBytesToRead Nombre d'octets � lire
 * @return Zero en cas d'�chec
 */
RT_B RT_API RtReadFromFile(RT_FILE* lpFile, RT_CHAR8* lpBuffer, RT_UN32 unBytesToRead);

/**
 * Ecrit un certain nombre d'octets dans un fichier
 *
 * @param lpData Donn�es � �crire
 * @param nNumberOfBytesToWrite Nombre d'octets � �crire
 * @return Zero en cas d'�chec
 */
RT_B RT_API RtWriteToFile(RT_FILE* lpFile, RT_CHAR8* lpData, RT_UN32 unBytesToWrite);

/**
 * Compute the size of an open file.
 *
 * @return -1 in case of error (And the file pointer might be corrupted).
 */
RT_N RT_API RtComputeFileSize(RT_FILE* lpFile);

/**
 * D�place le pointeur dans le fichier
 *
 * @param nOffset D�placement � effectu�
 * @param nFrom Type de positionnement : RT_FILE_POS_...
 * @return Zero en cas d'�chec
 */
RT_B RT_API RtSetFilePointer(RT_FILE* lpFile, RT_N nOffset, RT_UN unFrom);

/**
 * R�cup�re la position courante dans le fichier
 *
 * @return La position courante, -1 en cas de probl�me.
 */
RT_N RT_API RtGetFilePointer(RT_FILE* lpFile);

/**
 * Ferme le fichier sp�cifi�
 */
RT_B RT_API RtFreeFile(RT_FILE* lpFile);

#endif /* RT_FILE_H */