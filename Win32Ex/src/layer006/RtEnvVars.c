#include "layer006/RtEnvVars.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer004/RtChar.h"
#include "layer005/RtStaticHeap.h"

RT_B RT_API RtCreateEnvVars(RT_ENV_VARS* lpEnvVars)
{
#ifdef RT_DEFINE_LINUX
  RT_CHAR** lpEnvVarsArray;
  RT_CHAR* lpEnvVarsBlock;
  RT_UN unBlockSize;
  RT_UN unWritten;
#endif
  RT_B bResult;

  lpEnvVars->lpEnvVarsArray = RT_NULL;

#ifdef RT_DEFINE_WINDOWS

  lpEnvVars->bWindowsBlock = RT_TRUE;

  /* GetEnvironmentStrings returns null in case of error. */
  lpEnvVars->lpEnvVarsBlock = GetEnvironmentStrings();
  if (!lpEnvVars->lpEnvVarsBlock) goto handle_error;

#else /* NOT RT_DEFINE_WINDOWS */

  lpEnvVars->lpEnvVarsBlock = RT_NULL;

  /* environ point on a null terminated array of pointers to null terminated variables. */
  lpEnvVarsArray = environ;

  unBlockSize = 0;
  while (*lpEnvVarsArray)
  {
    unBlockSize += RtGetStringSize(*lpEnvVarsArray);
    unBlockSize++; /* Null character for the variable. */
    lpEnvVarsArray++;
  }
  /* Null character after the last null character. */
  unBlockSize++;

  if (!RtAlloc((void**)&lpEnvVars->lpEnvVarsBlock, unBlockSize * sizeof(RT_CHAR))) goto handle_error;

  /* Notice that environ might have been modified by another thread. Environment is not thread safe under Linux. */
  lpEnvVarsArray = environ;
  lpEnvVarsBlock = lpEnvVars->lpEnvVarsBlock;

  unWritten = 0;
  while (*lpEnvVarsArray)
  {
    if (!RtCopyString(*lpEnvVarsArray, &lpEnvVarsBlock[unWritten], unBlockSize - unWritten, &unWritten)) goto handle_error;
    /* We need to keep the trailing null. */
    unWritten++;
    lpEnvVarsArray++;
  }
  unWritten--; /* We will overwrite last null and RtCopyStringWithSize will add one. */
  if (!RtCopyStringWithSize(_R("\0"), 1, &lpEnvVarsBlock[unWritten], unBlockSize - unWritten, &unWritten)) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  RtFreeEnvVars(lpEnvVars);
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtGetEnvVarsBlock(RT_ENV_VARS* lpEnvVars, RT_CHAR** lpEnvVarsBlock)
{
  *lpEnvVarsBlock = lpEnvVars->lpEnvVarsBlock;
  return RT_SUCCESS;
}

RT_B RT_API RtGetEnvVarsArray(RT_ENV_VARS* lpEnvVars, RT_CHAR*** lpEnvVarsArray)
{
  RT_CHAR* lpEnvVarsBlock;
  RT_UN unVarsCount;
  RT_UN unI;
  RT_B bResult;

  if (!lpEnvVars->lpEnvVarsArray)
  {
    lpEnvVarsBlock = lpEnvVars->lpEnvVarsBlock;

    unVarsCount = 0;
    if (*lpEnvVarsBlock)
    {
      while (RT_TRUE)
      {
        unVarsCount++;

        /* Find next null character. */
        while (*lpEnvVarsBlock)
        {
          lpEnvVarsBlock++;
        }

        /* Quit if next one is null too. */
        lpEnvVarsBlock++;
        if (!*lpEnvVarsBlock) break;
      }
    }

    /* Alloc variables count plus one for the trailing null. */
    if (!RtAlloc((void**)&lpEnvVars->lpEnvVarsArray, (unVarsCount + 1) * sizeof(RT_CHAR*))) goto handle_error;

    if (unVarsCount)
    {
      lpEnvVarsBlock = lpEnvVars->lpEnvVarsBlock;
      unI = 0;
      while (RT_TRUE)
      {
        lpEnvVars->lpEnvVarsArray[unI] = lpEnvVarsBlock;
        unI++;

        /* Find next null character. */
        while (*lpEnvVarsBlock)
        {
          lpEnvVarsBlock++;
        }

        /* Quit if next one is null too. */
        lpEnvVarsBlock++;
        if (!*lpEnvVarsBlock)
        {
          lpEnvVars->lpEnvVarsArray[unI] = RT_NULL;
          break;
        }

      }
    }
    else
    {
      lpEnvVars->lpEnvVarsArray[0] = RT_NULL;
    }
  }

  *lpEnvVarsArray = lpEnvVars->lpEnvVarsArray;
  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFreeEnvVars(RT_ENV_VARS* lpEnvVars)
{
  RT_B bResult;

  bResult = RT_SUCCESS;

  /* On Windows, lpEnvVarsBlock might have been allocated by Windows. */
#ifdef RT_DEFINE_WINDOWS
  if (lpEnvVars->bWindowsBlock)
  {
    /* lpEnvVarsBlock allocated by Windows */
    if (lpEnvVars->lpEnvVarsBlock)
    {
      if (!FreeEnvironmentStrings(lpEnvVars->lpEnvVarsBlock)) bResult = RT_FAILURE;
      lpEnvVars->lpEnvVarsBlock = RT_NULL;
    }
  }
  else
  {
    /* lpEnvVarsBlock allocated by the library. */
    if (!(RtFree((void**)&lpEnvVars->lpEnvVarsBlock))) bResult = RT_FAILURE;
  }
#else
  if (!(RtFree((void**)&lpEnvVars->lpEnvVarsBlock))) bResult = RT_FAILURE;
#endif

  if (!(RtFree((void**)&lpEnvVars->lpEnvVarsArray))) bResult = RT_FAILURE;

  return bResult;
}
