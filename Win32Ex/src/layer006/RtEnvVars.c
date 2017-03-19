#include "layer006/RtEnvVars.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer003/RtMemory.h"
#include "layer004/RtChar.h"
#include "layer005/RtStaticHeap.h"
#include "layer005/RtSystemInfo.h"


RT_B RT_API RtCreateEnvVars(RT_ENV_VARS* lpEnvVars)
{
  RT_UN unBlockSize;
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR* lpWindowsEnvVarsBlock;
  RT_CHAR* lpLibraryEnvVarsBlock;
  RT_B bGreaterOrEqual;
#else
  RT_CHAR** lpEnvVarsArray;
  RT_UN unWritten;
  RT_CHAR* lpEnvVarsBlock;
#endif
  RT_B bResult;

  lpEnvVars->lpEnvVarsArray = RT_NULL;

#ifdef RT_DEFINE_WINDOWS

  lpEnvVars->bWindowsBlock = RT_TRUE;

  /* GetEnvironmentStrings returns null in case of error. */
  lpEnvVars->lpEnvVarsBlock = GetEnvironmentStrings();
  if (!lpEnvVars->lpEnvVarsBlock) goto handle_error;
  
  /* In Vista 6.0.6001.18631 kernel32.dll disassembly, GetEnvironmentStrings returns a copy of the environment variables block. */
  if (!RtIsOsVersionGreaterOrEqualTo(6, 0, 0, &bGreaterOrEqual)) goto handle_error;
  if (!bGreaterOrEqual)
  {
    /* GetEnvironmentStrings return direct pointer on the environment on old (At least XP) Windows versions. */
    /* As a result the block is updated when a variable is added to the environment. */
    /* To avoid that, we copy the block if we are not under Vista. */
  
    lpWindowsEnvVarsBlock = lpEnvVars->lpEnvVarsBlock;
  
    unBlockSize = 0;
    while (RT_TRUE)
    {
      /* Find next null character. */
      while (lpWindowsEnvVarsBlock[unBlockSize])
      {
        unBlockSize++;
      }

      /* Quit if next one is null too. */
      unBlockSize++;
      if (!lpWindowsEnvVarsBlock[unBlockSize]) break;
    }
    /* Count trailing zero. */
    unBlockSize++;
    
    if (!RtAlloc((void**)&lpLibraryEnvVarsBlock, unBlockSize * sizeof(RT_CHAR))) goto handle_error;
    lpEnvVars->bWindowsBlock = RT_FALSE;
    lpEnvVars->lpEnvVarsBlock = lpLibraryEnvVarsBlock;
    RtCopyMemory(lpWindowsEnvVarsBlock, lpLibraryEnvVarsBlock, unBlockSize * sizeof(RT_CHAR));
    if (!FreeEnvironmentStrings(lpWindowsEnvVarsBlock)) goto handle_error;
  }

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

/**
 * Find a pointer on <tt>lpEnvVarName</tt> in <tt>lpEnvVars->lpEnvVarsBlock</tt>
 */
RT_B RT_CALL RtGetEnvVarPointer(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_CHAR** lpResult)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLocalEnvVarName[RT_CHAR_HALF_BIG_STRING_SIZE];
#else
  RT_CHAR* lpLocalEnvVarName;
#endif
  RT_CHAR* lpEnvVarsBlock;
  RT_CHAR lpCurrentEnvVarName[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_UN unI;
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* Build upper case variable name under Windows. */
  unWritten = 0;
  if (!RtCopyString(lpEnvVarName, lpLocalEnvVarName, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  RtFastUpperString(lpLocalEnvVarName);
#else
  lpLocalEnvVarName = lpEnvVarName;
#endif

  lpEnvVarsBlock = lpEnvVars->lpEnvVarsBlock;
  if (!*lpEnvVarsBlock)
  {
    /* Empty block. */
    *lpResult = RT_NULL;
  }
  else
  {
    while (RT_TRUE)
    {
      unI = 0;

      /* Find next equals character. */
      while (lpEnvVarsBlock[unI] != _R('=') && lpEnvVarsBlock[unI])
      {
        unI++;
      }

#ifdef RT_DEFINE_WINDOWS
      /* There are variable names starting with '=' under Windows. */
      if (lpEnvVarsBlock[unI] == _R('=') && !unI)
      {
        unI++;
        while (lpEnvVarsBlock[unI] != _R('=') && lpEnvVarsBlock[unI])
        {
          /* Find next equals character. */
          unI++;
        }
      }
#endif

      /* Null before equals. Something went wrong. */
      if (!lpEnvVarsBlock[unI]) goto handle_error;

      unWritten = 0;
      if (!RtCopyStringWithSize(lpEnvVarsBlock, unI, lpCurrentEnvVarName, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;

#ifdef RT_DEFINE_WINDOWS
      /* Build upper case variable name under Windows. */
      RtFastUpperString(lpCurrentEnvVarName);
#endif
      /* Check if we found the variable. */
      if (!RtCompareStrings(lpCurrentEnvVarName, lpLocalEnvVarName))
      {
        *lpResult = lpEnvVarsBlock;
        break;
      }

      /* Find next null character. */
      lpEnvVarsBlock = &lpEnvVarsBlock[unI + 1];
      while (*lpEnvVarsBlock)
      {
        lpEnvVarsBlock++;
      }

      /* Quit if next one is null too. */
      lpEnvVarsBlock++;
      if (!*lpEnvVarsBlock)
      {
        /* Variable not found. */
        *lpResult = RT_NULL;
        break;
      } 
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtEnvVarsContains(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_B* lpContains)
{
  RT_CHAR* lpEnvVar;
  RT_B bResult;

  if (!RtGetEnvVarPointer(lpEnvVars, lpEnvVarName, &lpEnvVar)) goto handle_error;
  if (lpEnvVar)
  {
    *lpContains = RT_TRUE;
  }
  else
  {
    *lpContains = RT_FALSE;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtGetEnvVarFromEnvVars(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  RT_CHAR* lpEnvVar;
  RT_B bResult;

  if (!RtGetEnvVarPointer(lpEnvVars, lpEnvVarName, &lpEnvVar)) goto handle_error;
  if (!lpEnvVar)
  {
    /* Variable not found. */
    goto handle_error;
  }

  /* Copy the value, after the name and '='. */
  if (!RtCopyString(&lpEnvVar[RtGetStringSize(lpEnvVarName) + 1], lpBuffer, unBufferSize, lpWritten)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtRemoveEnvVarFromEnvVars(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName)
{
  RT_CHAR* lpEnvVar;
  RT_CHAR* lpDestination;
  RT_B bWasNull;
  RT_B bResult;

  /* The array will be built back if and when needed. */
  if (!RtFree((void**)&lpEnvVars->lpEnvVarsArray)) goto handle_error;

  if (!RtGetEnvVarPointer(lpEnvVars, lpEnvVarName, &lpEnvVar)) goto handle_error;
  
  /* Remove only if exists. */
  if (lpEnvVar)
  {
    lpDestination = lpEnvVar;

    /* Skip the variable. */
    while (*lpEnvVar) lpEnvVar++;
    lpEnvVar++; /* Skip the trailing zero. */

    bWasNull = RT_TRUE;
    while (RT_TRUE)
    {
      if (!*lpEnvVar)
      {
        if (bWasNull)
        {
          /* End of the block. */
          *lpDestination = 0;
          break;
        }
        bWasNull = RT_TRUE;
      }
      else
      {
        bWasNull = RT_FALSE;
      }
      *lpDestination = *lpEnvVar;
      lpDestination++;
      lpEnvVar++;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtAddEnvVarIntoEnvVars(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_CHAR* lpValue)
{
  RT_CHAR* lpOldEnvVarsBlock;
  RT_CHAR* lpNewEnvVarsBlock;
  RT_UN unOldEnvVarsBlockSize;
  RT_UN unNewEnvVarsBlockSize;
  RT_UN unWritten;
  RT_B bResult;

  /* Will be free in case of error. */
  lpNewEnvVarsBlock = RT_NULL;

  /* The array will be built back if and when needed. */
  if (!RtFree((void**)&lpEnvVars->lpEnvVarsArray)) goto handle_error;

  lpOldEnvVarsBlock = lpEnvVars->lpEnvVarsBlock;

  unOldEnvVarsBlockSize = 0;
  while (RT_TRUE)
  {
    /* Find next null character. */
    while (lpOldEnvVarsBlock[unOldEnvVarsBlockSize])
    {
      unOldEnvVarsBlockSize++;
    }

    /* Quit if next one is null too. */
    unOldEnvVarsBlockSize++;
    if (!lpOldEnvVarsBlock[unOldEnvVarsBlockSize]) break;
  }
  /* Count trailing zero. */
  unOldEnvVarsBlockSize++;

  unNewEnvVarsBlockSize = unOldEnvVarsBlockSize;

  unNewEnvVarsBlockSize += RtGetStringSize(lpEnvVarName);
  unNewEnvVarsBlockSize++; /* Equals. */
  unNewEnvVarsBlockSize += RtGetStringSize(lpValue);
  unNewEnvVarsBlockSize++; /* Terminating null. */

#ifdef RT_DEFINE_WINDOWS
  if (lpEnvVars->bWindowsBlock)
  {
    if (!RtAlloc((void**)&lpNewEnvVarsBlock, unNewEnvVarsBlockSize * sizeof(RT_CHAR*))) goto handle_error;
    RT_MEMORY_COPY(lpOldEnvVarsBlock, lpNewEnvVarsBlock, unOldEnvVarsBlockSize * sizeof(RT_CHAR*));
  }
  else
  {
    if (!RtReAlloc((void**)&lpNewEnvVarsBlock, lpOldEnvVarsBlock, unNewEnvVarsBlockSize * sizeof(RT_CHAR*))) goto handle_error;
  }
#else
  if (!RtReAlloc((void**)&lpNewEnvVarsBlock, lpOldEnvVarsBlock, unNewEnvVarsBlockSize * sizeof(RT_CHAR*))) goto handle_error;
#endif

  unWritten = unOldEnvVarsBlockSize - 1;
  if (!RtCopyString(lpEnvVarName,         &lpNewEnvVarsBlock[unWritten], unNewEnvVarsBlockSize - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyStringWithSize(_R("="), 1,   &lpNewEnvVarsBlock[unWritten], unNewEnvVarsBlockSize - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(lpValue,              &lpNewEnvVarsBlock[unWritten], unNewEnvVarsBlockSize - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyStringWithSize(_R("\0"), 1,  &lpNewEnvVarsBlock[unWritten], unNewEnvVarsBlockSize - unWritten, &unWritten)) goto handle_error;

#ifdef RT_DEFINE_WINDOWS
  if (lpEnvVars->bWindowsBlock)
  {
    /* The block is now necessarily managed by us. */
    lpEnvVars->bWindowsBlock = RT_FALSE;

    /* Free Windows block. */
    if (!FreeEnvironmentStrings(lpEnvVars->lpEnvVarsBlock))
    {
      lpEnvVars->lpEnvVarsBlock = RT_NULL;
      goto handle_error;
    }
    lpEnvVars->lpEnvVarsBlock = RT_NULL;
  }
#endif

  lpEnvVars->lpEnvVarsBlock = lpNewEnvVarsBlock;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  RtFree((void**)&lpNewEnvVarsBlock);
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtMergeEnvVarIntoEnvVars(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_CHAR* lpValue)
{
  RT_B bContains;
  RT_B bResult;

  if (!RtEnvVarsContains(lpEnvVars, lpEnvVarName, &bContains)) goto handle_error;
  if (bContains)
  {
    if (!RtRemoveEnvVarFromEnvVars(lpEnvVars, lpEnvVarName)) goto handle_error;
  }
  if (!RtAddEnvVarIntoEnvVars(lpEnvVars, lpEnvVarName, lpValue)) goto handle_error;

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
