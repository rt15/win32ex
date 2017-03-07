#include "layer006/RtConsole.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer004/RtChar.h"
#include "layer005/RtStaticHeap.h"

/**
 * Affiche le texte indiqu� dans la console
 *
 * @param lpString Texte � �crire dans la console
 * @return Zero en cas de probl�me
 */
RT_B RT_API RtWriteStringToConsole(RT_CHAR* lpString)
{
  return RtWriteStringToConsoleWithSize(lpString, RtGetStringSize(lpString));
}

/**
 * Affiche le texte indiqu� dans la console
 *
 * @param lpString Texte � �crire dans la console
 * @param unSize Nombre de caract�res � afficher
 * @return Zero en cas de probl�me
 */
RT_B RT_CALL RtWriteToConsoleWithSize(RT_CHAR* lpString, RT_UN unSize, RT_B bError)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unWritten;       /* R�ception du nombre de caract�re �cris                   */
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

  /* Translate ���� characters... Never fails if arguments are different. */
  CharToOemBuff(lpString, lpOemText, (DWORD)unSize);

  if (bError)
  {
    hStream = GetStdHandle(STD_ERROR_HANDLE);
  }
  else
  {
    hStream = GetStdHandle(STD_OUTPUT_HANDLE);
  }

  bResult = WriteConsoleA(hStream, lpOemText, (DWORD)unSize, &unWritten, NULL);

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
  if (bError)
  {
    nFileDescriptor = 2;
  }
  else
  {
    nFileDescriptor = 1;
  }
  return (write(nFileDescriptor, lpString, (size_t)unSize) != -1);
#endif
}

RT_B RT_API RtWriteStringToConsoleWithSize(RT_CHAR* lpString, RT_UN unSize)
{
  return RtWriteToConsoleWithSize(lpString, unSize, RT_FALSE);
}

RT_B RT_API RtWriteErrorToConsole(RT_CHAR* lpString)
{
  return RtWriteErrorToConsoleWithSize(lpString, RtGetStringSize(lpString));
}

RT_B RT_API RtWriteErrorToConsoleWithSize(RT_CHAR* lpString, RT_UN unSize)
{
  return RtWriteToConsoleWithSize(lpString, unSize, RT_TRUE);
}

RT_B RT_CDECL_API RtWriteStringsToConsole(RT_UN unStringsCount, ...)
{
  RT_UN unStringsLenght;
  RT_CHAR* lpString;
  va_list lpVaList;
  RT_UN unWritten;
  RT_CHAR lpBuffer[512];
  RT_CHAR* lpStrings;
  void* lpHeapBuffer;
  RT_UN unHeapBufferSize;
  RT_UN unI;
  RT_B bResult;

  if (unStringsCount <= 0)
  {
    bResult = RT_SUCCESS;
  }
  else if (unStringsCount == 1)
  {
    /* If there is one String, directly call RtWriteToConsole. */
    va_start(lpVaList, unStringsCount);
    lpString = va_arg(lpVaList, RT_CHAR*);
    va_end(lpVaList);

    bResult = RtWriteStringToConsole(lpString);
  }
  else
  {
    /* Compute total strings size. */
    va_start(lpVaList, unStringsCount);
    unStringsLenght = 0;
    for (unI = 0; unI < unStringsCount; unI++)
    {
      lpString = va_arg(lpVaList, RT_CHAR*);
      unStringsLenght += RtGetStringSize(lpString);
    }
    va_end(lpVaList);

    lpHeapBuffer = RT_NULL;
    unHeapBufferSize = 0;
    if (RtAllocIfNeeded(lpBuffer, 512 * sizeof(RT_CHAR), &lpHeapBuffer, &unHeapBufferSize, (void**)&lpStrings, (unStringsLenght + 1) * sizeof(RT_CHAR)))
    {
      va_start(lpVaList, unStringsCount);
      unWritten = 0;
      RtVConcatStrings(lpVaList, unStringsCount, lpStrings, unStringsLenght + 1, &unWritten);
      va_end(lpVaList);

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

  return bResult;
}

/**
 * Attend que l'utilisateur presse une touche avant de continuer
 */
void RT_API RtPauseConsole()
{
  RtReadCharFromConsole();
}

RT_UN RT_API RtReadLineFromConsole(RT_CHAR* lpBuffer, RT_UN unBufferSize)
{
#ifdef RT_DEFINE_WINDOWS
  HANDLE hInput;    /* Handle de l'entr�e standard                            */
  DWORD unRead;    /* Nombre d'octets lus                                    */
  DWORD unOldMode; /* Sauvegarde le mode pr�c�dent pour le restituer         */
#else
  int nChar;
#endif
  RT_UN unResult;

#ifdef RT_DEFINE_WINDOWS
  hInput = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(hInput, &unOldMode);
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
  HANDLE hInput;    /* Handle de l'entr�e standard                            */
  RT_UN unRead;    /* Nombre d'octets lus                                    */
  RT_UN32 unOldMode; /* Sauvegarde le mode pr�c�dent pour le restituer         */
#else
  struct termios termConfig;
  struct termios backupConfig;
  RT_CHAR nChar;
  int fileDescriptorFlags;
#endif
  RT_CHAR nResult;

#ifdef RT_DEFINE_WINDOWS
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
