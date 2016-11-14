#include "layer006/RtConsole.h"

#include "layer001/RtWin32ExOsDefines.h"
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

/**
 * Affiche le texte indiqué dans la console
 *
 * @param lpString Texte à écrire dans la console
 * @param nSize Nombre de caractères à afficher
 * @return Zero en cas de problème
 */
RT_B RT_API RtWriteStringToConsoleWithSize(RT_CHAR* lpString, RT_N nSize)
{
#ifdef RT_DEFINE_WINDOWS
  RT_UN32 unWritten;  /* Réception du nombre de caractère écris                   */
  RT_CHAR8* lpOemText;   /* Texte traduit en OEM                                  */
  void* lpHeapBuffer;
  RT_N nHeapBufferSize;
  RT_CHAR8 lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_B bResult;
#endif

#ifdef RT_DEFINE_WINDOWS
  lpHeapBuffer = RT_NULL;
  nHeapBufferSize = 0;

  if (!RtAllocIfNeeded(lpBuffer, RT_CHAR_BIG_STRING_SIZE, &lpHeapBuffer, &nHeapBufferSize, (void**)&lpOemText, nSize)) goto handle_error;

  /* Translate àùéè characters... Never fails if arguments are different. */
  CharToOemBuff(lpString, lpOemText, nSize);

  bResult = WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), lpOemText, nSize, &unWritten, NULL);

free_resources:
  if (lpHeapBuffer)
  {
    if (!RtFree(&lpHeapBuffer))
    {
      lpHeapBuffer = RT_NULL;
      goto handle_error;
    }
    lpHeapBuffer = RT_NULL;
  }
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
#else
  return (write(1, lpString, (size_t)nSize) != -1);
#endif
}

RT_B RT_CDECL_API RtWriteStringsToConsole(RT_UN unStringsCount, ...)
{
  RT_UN unStringsLenght;
  RT_CHAR* lpString;
  va_list vaList;
  RT_N nWritten;
  RT_CHAR lpBuffer[512];
  RT_CHAR* lpStrings;
  void* lpHeapBuffer;
  RT_N nHeapBufferSize;
  RT_UN unI;
  RT_B bResult;

  if (unStringsCount <= 0)
  {
    bResult = RT_TRUE;
  }
  else if (unStringsCount == 1)
  {
    /* If there is one String, directly call RtWriteToConsole. */
    va_start(vaList, unStringsCount);
    lpString = va_arg(vaList, RT_CHAR*);
    va_end(vaList);

    bResult = RtWriteStringToConsole(lpString);
  }
  else
  {
    /* Compute total strings size. */
    va_start(vaList, unStringsCount);
    unStringsLenght = 0;
    for (unI = 0; unI < unStringsCount; unI++)
    {
      lpString = va_arg(vaList, RT_CHAR*);
      unStringsLenght += RtGetStringSize(lpString);
    }
    va_end(vaList);

    lpHeapBuffer = RT_NULL;
    nHeapBufferSize = 0;
    if (RtAllocIfNeeded(lpBuffer, 512 * sizeof(RT_CHAR), &lpHeapBuffer, &nHeapBufferSize, (void**)&lpStrings, (unStringsLenght + 1) * sizeof(RT_CHAR)))
    {
      va_start(vaList, unStringsCount);
      nWritten = 0;
      RtVConcatStrings(vaList, unStringsCount, lpStrings, unStringsLenght + 1, &nWritten);
      va_end(vaList);

      bResult = RtWriteStringToConsoleWithSize(lpStrings, nWritten);
      if (lpHeapBuffer)
      {
        bResult = (RtFree(&lpHeapBuffer) && bResult);
      }
    }
    else
    {
      bResult = RT_FALSE;
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

RT_N RT_API RtReadLineFromConsole(RT_CHAR* lpBuffer, RT_N nBufferSize)
{
#ifdef RT_DEFINE_WINDOWS
  HANDLE hInput;    /* Handle de l'entrée standard                            */
  RT_UN32 unRead;    /* Nombre d'octets lus                                    */
  RT_UN32 unOldMode; /* Sauvegarde le mode précédent pour le restituer         */
#else
  int nChar;
#endif
  RT_N nResult;

#ifdef RT_DEFINE_WINDOWS
  hInput = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(hInput, &unOldMode);
  SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
  if (ReadConsole(hInput, lpBuffer, nBufferSize, &unRead, RT_NULL))
  {
    if (unRead == nBufferSize)
    {
      /* Keep reading until LF is found to discard characters (FlushConsoleInputBuffer cannot be used with ReadConsole). */
      while (lpBuffer[unRead - 1] != _R('\n'))
      {
        if (!ReadConsole(hInput, lpBuffer, nBufferSize, &unRead, RT_NULL))
        {
          break;
        }
      }
      nResult = -1;
    }
    else
    {
      /* Skip CR/LF. */
      unRead -= 2;
      /* Zero terminated String. */
      lpBuffer[unRead] = 0;
      nResult = unRead;
    }
  }
  else
  {
    nResult = -1;
  }
  SetConsoleMode(hInput, unOldMode);
#else
  /* The getline function cannot be used as first parameter must be a pointer on memory allocated with malloc. */
  nResult = 0;
loop:
  nChar = getchar();
  if (nResult >= nBufferSize)
  {
    /* Buffer not large enough. */
    lpBuffer[0] = 0;
    nResult = -1;
  }
  else
  {
    /* Replace EOF and end of line with zero character. */
    if (nChar == EOF || nChar == '\n')
    {
      nChar = 0;
    }

    lpBuffer[nResult] = nChar;

    if (nChar)
    {
      nResult++;
      goto loop;
    }
  }
#endif
  return nResult;
}

RT_CHAR RT_API RtReadCharFromConsole()
{
#ifdef RT_DEFINE_WINDOWS
  HANDLE hInput;    /* Handle de l'entrée standard                            */
  RT_UN32 unRead;    /* Nombre d'octets lus                                    */
  RT_UN32 unOldMode; /* Sauvegarde le mode précédent pour le restituer         */
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
