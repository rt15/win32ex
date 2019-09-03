#include "layer004/RtUrl.h"

#include "layer002/RtError.h"
#include "layer003/RtChar.h"

RT_B RT_API RtUrl_Parse(RT_CHAR* lpUrl, RT_URL_INFO* lpUrlInfo)
{
  RT_CHAR lpPort[16];
  RT_UN unWritten;
  RT_B bAuthentication;
  RT_UN unI;
  RT_UN unJ;
  RT_B bResult;

  unI = 0;
  while (lpUrl[unI] != _R(':'))
  {
    if (((lpUrl[unI] >= _R('a')) && (lpUrl[unI] <= _R('z'))) ||
        ((lpUrl[unI] >= _R('A')) && (lpUrl[unI] <= _R('Z'))) ||
        ((lpUrl[unI] >= _R('0')) && (lpUrl[unI] <= _R('9'))) ||
        (lpUrl[unI] == _R('+')) ||
        (lpUrl[unI] == _R('-')) ||
        (lpUrl[unI] == _R('.')))
    {
      unI++;
    }
    else
    {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }
  }

  lpUrlInfo->lpScheme = lpUrl;
  lpUrlInfo->unSchemeSize = unI;

  /* Skip ':'. */
  unI++;

  /* Read "//". */
  for (unJ = 0; unJ < 2; unJ++)
  {
    if (lpUrl[unI] != _R('/'))
    {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }
    unI++;
  }

  /* Search for authentication part user:password@. Stop at first slash or end of string. */
  bAuthentication = RT_FALSE;
  unJ = unI;
  while (lpUrl[unJ] && lpUrl[unJ] != _R('/'))
  {
    if (lpUrl[unJ] == _R('@'))
    {
      bAuthentication = RT_TRUE;
      break;
    }
    unJ++;
  }

  if (bAuthentication)
  {
    unJ = unI;
    while (lpUrl[unJ] != _R(':') && lpUrl[unJ] != _R('@'))
    {
      unJ++;
    }
    lpUrlInfo->lpUser = &lpUrl[unI];
    lpUrlInfo->unUserSize = unJ - unI;

    unI = unJ;
    if (lpUrl[unI] == _R(':'))
    {
      /* Skip ':'. */
      unI++;

      unJ = unI;
      while (lpUrl[unJ] != _R('@'))
      {
        unJ++;
      }
      lpUrlInfo->lpPassword = &lpUrl[unI];
      lpUrlInfo->unPasswordSize = unJ - unI;
    }
    else
    {
      /* No password. */
      lpUrlInfo->lpPassword = RT_NULL;
      lpUrlInfo->unPasswordSize = 0;
    }

    unI = unJ;
    /* Skip '@'. */
    unI++;
  }
  else
  {
    lpUrlInfo->lpUser = RT_NULL;
    lpUrlInfo->unUserSize = 0;
    lpUrlInfo->lpPassword = RT_NULL;
    lpUrlInfo->unPasswordSize = 0;
  }

  unJ = unI;

  /* IPv6 addresses are enclosed in brackets. */
  if (lpUrl[unJ] == _R('['))
  {
    /* Skip opening bracket. */
    unJ++;
    while (lpUrl[unJ] && lpUrl[unJ] != _R(']'))
    {
      unJ++;
    }
    /* No closing bracket. */
    if (!lpUrl[unJ])
    {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }
    /* Skip closing bracket. */
    unJ++;
  }
  else
  {
    while (lpUrl[unJ] && lpUrl[unJ] != _R(':') && lpUrl[unJ] != _R('/'))
    {
      unJ++;
    }
  }
  lpUrlInfo->lpHost = &lpUrl[unI];
  lpUrlInfo->unHostSize = unJ - unI;

  unI = unJ;

  /* Port? */
  if (lpUrl[unI] == _R(':'))
  {
    /* Skip ':'. */
    unI++;
    unJ = unI;
    while (lpUrl[unJ] && lpUrl[unJ] != _R('/'))
    {
      unJ++;
    }
    unWritten = 0;
    if (!RtChar_CopyStringWithSize(&lpUrl[unI], unJ - unI, lpPort, 16, &unWritten)) goto handle_error;
    if (!RtChar_ConvertStringToUInteger(lpPort, &lpUrlInfo->unPort)) goto handle_error;
    unI = unJ;
  }
  else
  {
    lpUrlInfo->unPort = RT_TYPE_MAX_UN;
  }

  /* Initialize remaining fields. */
  lpUrlInfo->lpPath = RT_NULL;
  lpUrlInfo->unPathSize = 0;
  lpUrlInfo->lpQuery = RT_NULL;
  lpUrlInfo->unQuerySize = 0;
  lpUrlInfo->lpFragment = RT_NULL;
  lpUrlInfo->unFragmentSize = 0;

  if (lpUrl[unI])
  {
    /* Path is "optional" but if there is a path, there must be a slash. */
    if (lpUrl[unI] != _R('/'))
    {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }

    /* Skip '/'. */
    unI++;
    unJ = unI;

    while (lpUrl[unJ] && lpUrl[unJ] != _R('?') && lpUrl[unJ] != _R('#'))
    {
      unJ++;
    }
    lpUrlInfo->lpPath = &lpUrl[unI];
    lpUrlInfo->unPathSize = unJ - unI;

    unI = unJ;

    /* Is there a query? */
    if (lpUrl[unI] == _R('?'))
    {
      /* Skip '?'. */
      unI++;
      unJ = unI;

      while (lpUrl[unJ] && lpUrl[unJ] != _R('#'))
      {
        unJ++;
      }
      lpUrlInfo->lpQuery = &lpUrl[unI];
      lpUrlInfo->unQuerySize = unJ - unI;

      unI = unJ;
    }

    /* Is there a fragment? */
    if (lpUrl[unI] == _R('#'))
    {
      /* Skip '#'. */
      unI++;
      unJ = unI;

      while (lpUrl[unJ])
      {
        unJ++;
      }
      lpUrlInfo->lpFragment = &lpUrl[unI];
      lpUrlInfo->unFragmentSize = unJ - unI;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
