#include "layer006/RtConsole.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer003/RtMemory.h"
#include "layer004/RtChar.h"
#include "layer005/RtStaticHeap.h"

/**
 * Affiche le texte indiqué dans la console
 *
 * @param lpString Texte à écrire dans la console
 * @return Zero en cas de problème
 */
RT_B RT_API RtWriteStringToConsole(RT_CHAR* lpString)
{
  return RtWriteStringToConsoleWithSize(lpString, RtGetStringSize(lpString));
}

RT_B RT_API RtWriteStringOrErrorToConsole(RT_CHAR* lpString, RT_B bString)
{
  return RtWriteStringOrErrorToConsoleWithSize(lpString, RtGetStringSize(lpString), bString);
}

/**
 * Affiche le texte indiqué dans la console
 *
 * @param lpString Texte à écrire dans la console
 * @param unSize Nombre de caractères à afficher
 * @return Zero en cas de problème
 */
RT_B RT_API RtWriteStringOrErrorToConsoleWithSize(RT_CHAR* lpString, RT_UN unSize, RT_B bString)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unWritten;       /* Réception du nombre de caractère écris                   */
  RT_CHAR8* lpOemText;   /* Texte traduit en OEM                                  */
  void* lpHeapBuffer;
  RT_UN unHeapBufferSize;
  RT_CHAR8 lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  HANDLE hStream;
  RT_B bResult;
#else
  int nFileDescriptor;
#endif

#ifdef RT_DEFINE_WINDOWS
  lpHeapBuffer = RT_NULL;
  unHeapBufferSize = 0;

  if (!RtAllocIfNeeded(lpBuffer, RT_CHAR_BIG_STRING_SIZE * sizeof(RT_CHAR8), &lpHeapBuffer, &unHeapBufferSize, (void**)&lpOemText, unSize)) goto handle_error;

  /* Translate àùéè characters... Never fails if arguments are different. */
  CharToOemBuff(lpString, lpOemText, (DWORD)unSize);

  if (bString)
  {
    /* TODO: Check GetStdHandle result. */
    hStream = GetStdHandle(STD_OUTPUT_HANDLE);
  }
  else
  {
    /* TODO: Check GetStdHandle result. */
    hStream = GetStdHandle(STD_ERROR_HANDLE);
  }

  /* WriteConsoleA must not be used as it does not manage redirection to a file. */
  bResult = WriteFile(hStream, lpOemText, (DWORD)unSize, &unWritten, NULL);

free_resources:
  if (lpHeapBuffer)
  {
    if (!RtFree(&lpHeapBuffer)) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
#else
  if (bString)
  {
    nFileDescriptor = 1;
  }
  else
  {
    nFileDescriptor = 2;
  }
  return (write(nFileDescriptor, lpString, (size_t)unSize) != -1);
#endif
}

RT_B RT_API RtWriteStringToConsoleWithSize(RT_CHAR* lpString, RT_UN unSize)
{
  return RtWriteStringOrErrorToConsoleWithSize(lpString, unSize, RT_TRUE);
}

RT_B RT_API RtWriteErrorToConsole(RT_CHAR* lpString)
{
  return RtWriteErrorToConsoleWithSize(lpString, RtGetStringSize(lpString));
}

RT_B RT_API RtWriteErrorToConsoleWithSize(RT_CHAR* lpString, RT_UN unSize)
{
  return RtWriteStringOrErrorToConsoleWithSize(lpString, unSize, RT_FALSE);
}

RT_B RT_CDECL_API RtWriteStringsOrErrorsToConsole(RT_B bString, ...)
{
  va_list lpVaList;
  RT_B bResult;

  va_start(lpVaList, bString);
  bResult = RtVWriteStringsOrErrorsToConsole(lpVaList, bString);
  va_end(lpVaList);

  return bResult;
}

RT_B RT_API RtVWriteStringsOrErrorsToConsole(va_list lpVaList, RT_B bString)
{
  va_list lpVaList2;
  va_list lpVaList3;
  RT_UN unStringsCount;
  RT_UN unStringsLenght;
  RT_CHAR* lpString;
  RT_UN unWritten;
  RT_CHAR lpBuffer[512];
  RT_CHAR* lpStrings;
  void* lpHeapBuffer;
  RT_UN unHeapBufferSize;
  RT_UN unI;
  RT_B bResult;

  RT_VA_COPY(lpVaList, lpVaList2);
  RT_VA_COPY(lpVaList, lpVaList3);

  unStringsCount = 0;
  while (RT_TRUE)
  {
    if (!va_arg(lpVaList, RT_CHAR*)) break;
    unStringsCount++;
  }

  if (unStringsCount <= 0)
  {
    bResult = RT_SUCCESS;
  }
  else if (unStringsCount == 1)
  {
    /* If there is one String, directly call RtWriteToConsole. */
    lpString = va_arg(lpVaList2, RT_CHAR*);

    bResult = RtWriteStringToConsole(lpString);
  }
  else
  {
    /* Compute total strings size. */
    unStringsLenght = 0;
    for (unI = 0; unI < unStringsCount; unI++)
    {
      lpString = va_arg(lpVaList2, RT_CHAR*);
      unStringsLenght += RtGetStringSize(lpString);
    }

    lpHeapBuffer = RT_NULL;
    unHeapBufferSize = 0;
    if (RtAllocIfNeeded(lpBuffer, 512 * sizeof(RT_CHAR), &lpHeapBuffer, &unHeapBufferSize, (void**)&lpStrings, (unStringsLenght + 1) * sizeof(RT_CHAR)))
    {
      unWritten = 0;
      RtVConcatStrings(lpVaList3, lpStrings, unStringsLenght + 1, &unWritten);

      bResult = RtWriteStringToConsoleWithSize(lpStrings, unWritten);
      if (lpHeapBuffer)
      {
        bResult = (RtFree(&lpHeapBuffer) && bResult);
      }
    }
    else
    {
      bResult = RT_FAILURE;
    }
  }

  va_end(lpVaList3);
  va_end(lpVaList2);
  return bResult;
}

/**
 * Attend que l'utilisateur presse une touche avant de continuer
 */
void RT_API RtPauseConsole()
{
  RtReadCharFromConsole();
}

#ifdef RT_DEFINE_WINDOWS

/**
 * ReadConsole cannot work if stdin has been redirected to a pipe.<br>
 * So ReadFile is used instead.
 */
RT_UN RT_CALL RtReadLineFromFile(RT_CHAR* lpCalleeBuffer, RT_UN unBufferSize, RT_H hInput)
{
  RT_CHAR8 lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  void* lpHeapBuffer;
  RT_UN unHeapBufferSize;
  RT_CHAR8* lpReadChars;
  RT_CHAR8 nReadChar;
  DWORD unNumberOfBytesRead;
  RT_UN unResult;

  lpHeapBuffer = RT_NULL;

  /* Allocate a buffer to receive chars from ReadFile. */
  unHeapBufferSize = 0;
  if (!RtAllocIfNeeded(lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &lpHeapBuffer, &unHeapBufferSize, (void**)&lpReadChars, unBufferSize)) goto handle_error;

  RT_MEMORY_ZERO(lpReadChars, unBufferSize);

  unResult = 0;
  while (unResult < unBufferSize - 1)
  {
    if (!ReadFile(hInput, &nReadChar, 1, &unNumberOfBytesRead, RT_NULL)) goto handle_error;
    if (nReadChar == '\n')
    {
      break;
    }
    else if (nReadChar != '\r')
    {
      lpReadChars[unResult] = nReadChar;
      unResult++;
    }
  }
  if (unResult == unBufferSize - 1) goto handle_error;

  if (!OemToCharBuff(lpReadChars, lpCalleeBuffer, (DWORD)unBufferSize)) goto handle_error;
  lpBuffer[unResult] = 0;

free_resources:
  if (lpHeapBuffer)
  {
    if ((!RtFree(&lpHeapBuffer)) && (unResult != -1)) goto handle_error;
  }
  return unResult;

handle_error:
  unResult = -1;
  goto free_resources;
}

#endif

RT_UN RT_API RtReadLineFromConsole(RT_CHAR* lpBuffer, RT_UN unBufferSize)
{
#ifdef RT_DEFINE_WINDOWS
  HANDLE hInput;    /* Handle de l'entrée standard                            */
  DWORD unRead;    /* Nombre d'octets lus                                    */
  DWORD unOldMode; /* Sauvegarde le mode précédent pour le restituer         */
#else
  int nChar;
#endif
  RT_UN unResult;

#ifdef RT_DEFINE_WINDOWS
  /* TODO: Check GetStdHandle result. */
  hInput = GetStdHandle(STD_INPUT_HANDLE);

  if (GetConsoleMode(hInput, &unOldMode))
  {
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
    if (ReadConsole(hInput, lpBuffer, (DWORD)unBufferSize, &unRead, RT_NULL))
    {
      if (unRead == unBufferSize)
      {
        /* Keep reading until LF is found to discard characters (FlushConsoleInputBuffer cannot be used with ReadConsole). */
        while (lpBuffer[unRead - 1] != _R('\n'))
        {
          if (!ReadConsole(hInput, lpBuffer, (DWORD)unBufferSize, &unRead, RT_NULL))
          {
            break;
          }
        }
        unResult = RT_TYPE_MAX_UN;
      }
      else
      {
        /* Skip CR/LF. */
        unRead -= 2;
        /* Zero terminated String. */
        lpBuffer[unRead] = 0;
        unResult = unRead;
      }
    }
    else
    {
      unResult = RT_TYPE_MAX_UN;
    }
    SetConsoleMode(hInput, unOldMode);
  }
  else
  {
    /* GetConsoleMode has failed, we assume that the handle is a pipe handle. */
    unResult = RtReadLineFromFile(lpBuffer, unBufferSize, hInput);
  }
#else
  /* The getline function cannot be used as first parameter must be a pointer on memory allocated with malloc. */
  unResult = 0;
loop:
  nChar = getchar();
  if (unResult >= unBufferSize)
  {
    /* Buffer not large enough. */
    lpBuffer[0] = 0;
    unResult = RT_TYPE_MAX_UN;
  }
  else
  {
    /* Replace EOF and end of line with zero character. */
    if (nChar == EOF || nChar == '\n')
    {
      nChar = 0;
    }

    lpBuffer[unResult] = nChar;

    if (nChar)
    {
      unResult++;
      goto loop;
    }
  }
#endif
  return unResult;
}

RT_CHAR RT_API RtReadCharFromConsole()
{
#ifdef RT_DEFINE_WINDOWS
  HANDLE hInput;    /* Handle de l'entrée standard                            */
  RT_UN unRead;    /* Nombre d'octets lus                                    */
  RT_UN32 unOldMode; /* Sauvegarde le mode précédent pour le restituer         */
#else
  struct termios termConfig;
  struct termios backupConfig;
  RT_CHAR nChar;
  int fileDescriptorFlags;
#endif
  RT_CHAR nResult;

#ifdef RT_DEFINE_WINDOWS
  /* TODO: Check GetStdHandle result. */
  hInput = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(hInput, &unOldMode);
  SetConsoleMode(hInput, 0);
  ReadConsole(hInput, &nResult, 1, &unRead, NULL);
  SetConsoleMode(hInput, unOldMode);
#else
  tcgetattr(0, &termConfig);
  memcpy(&backupConfig, &termConfig, sizeof(backupConfig));

  termConfig.c_lflag &= ~(ICANON | ECHO);
  termConfig.c_cc[VTIME] = 0;
  termConfig.c_cc[VMIN] = 1;

  tcsetattr(0, TCSANOW, &termConfig);
  nResult = getchar();

  /* Get current stdin file descriptor flags. */
  fileDescriptorFlags = fcntl(0, F_GETFL, 0);

  /* Set stdin non-blocking. */
  fcntl(0, F_SETFL, fileDescriptorFlags | O_NONBLOCK);

  /* Discard (most of) possible trailing UTF-8 characters. */
  while (RT_TRUE)
  {
    nChar = getchar();
    if (nChar == EOF || nChar == _R('\n'))
    {
      break;
    }
  }

  /* Restore blocking stin using initial flags. */
  fcntl(0, F_SETFL, fileDescriptorFlags);

  /* Restore terminal attributes. */
  tcsetattr(0, TCSANOW, &backupConfig);
#endif
  return nResult;
}
