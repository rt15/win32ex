#include "cbroute.h"

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

RT_UN16 RT_CALL CbAddRoute(void* lpContext)
{
  CB_COMPETITION* lpCompetition;
  CB_ROUTE* lpRoute;
  RT_CHAR lpName[256];
  RT_N nRead;
  RT_UN32 unResult;

  while (RT_TRUE)
  {
    RtWriteStringToConsole(_R("\nEnter a name or nothing to quit:\n"));
    nRead = RtReadLineFromConsole(lpName, 256);

    /* Error. */
    if (nRead == -1)
    {
      nResult = 1;
      break;
    }

    /* Exit. */
    if (nRead == 0)
    {
      nResult = 0;
      break;
    }

    /* Too long name. */
    if (nRead >= CB_ROUTE_NAME_SIZE)
    {
      RtWriteStringToConsole(_R("Too long name.\n"));
    }
    else
    {
      lpCompetition = (CB_COMPETITION*)lpContext;
      if (!RtNewArrayItem((void**)&lpCompetition->lpRoutes, (void**)&lpRoute))
      {
        nResult = 1;
        break;
      }
      RtCopyStringWithSize(lpRoute->lpName, CB_ROUTE_NAME_SIZE, lpName, nRead);
    }
  }
  return nResult;
}

RT_UN16 RT_CALL CbDeleteRoute(void* lpContext)
{
  CB_COMPETITION* lpCompetition;
  RT_N nRoutesCount;
  RT_CHAR lpBuffer[16];
  RT_CHAR lpIndex[256];
  RT_N nIndex;
  RT_N nRead;
  RT_N nI;
  RT_UN32 unResult;

  lpCompetition = (CB_COMPETITION*)lpContext;

  while (RT_TRUE)
  {
    RtWriteStringToConsole(_R("\nEnter an index or nothing to quit\n"));

    CbWriteSeparator();
    nRoutesCount = RtGetArraySize(lpCompetition->lpRoutes);
    for (nI = 0; nI < nRoutesCount; nI++)
    {
      RtConvertNToString(nI + 1, lpBuffer, 16);
      RtWriteStringsToConsole(4, lpBuffer, _R(": "), lpCompetition->lpRoutes[nI].lpName, _R("\n"));
    }
    CbWriteSeparator();

    nRead = RtReadLineFromConsole(lpIndex, 256);

    /* Error. */
    if (nRead == -1)
    {
      nResult = 1;
      break;
    }

    /* Exit. */
    if (nRead == 0)
    {
      nResult = 0;
      break;
    }

    /* Parse index. */
    if (RtParseNFromString(lpIndex, &nIndex))
    {
      if ((nIndex >= 1) && (nIndex <= nRoutesCount))
      {
        if (!RtDeleteArrayItemIndex((void**)&lpCompetition->lpRoutes, nIndex - 1))
        {
          nResult = 1;
          break;
        }
      }
    }
  }

  return nResult;
}

CB_MENU_ITEM cb_lpRoutesMenuItems[2] = {{_R("Add"), &CbAddRoute}, {_R("Delete"), &CbDeleteRoute}};


RT_UN16 RT_CALL CbRoutesHeaderCallback(void* lpContext)
{
  CB_COMPETITION* lpCompetition;
  RT_N nRoutesCount;
  RT_N nI;

  CbWriteMenuTitle(_R("Routes"));
  lpCompetition = (CB_COMPETITION*)lpContext;

  CbWriteSeparator();
  nRoutesCount = RtGetArraySize(lpCompetition->lpRoutes);
  for (nI = 0; nI < nRoutesCount; nI++)
  {
    RtWriteStringsToConsole(2, lpCompetition->lpRoutes[nI].lpName, _R("\n"));
  }
  CbWriteSeparator();

  return 0;
}

RT_UN16 RT_CALL CbRoutes(void* lpContext)
{
  RT_UN32 unResult;

  nResult = CbManageMenu(&CbRoutesHeaderCallback, cb_lpRoutesMenuItems, 2, lpContext);

  return nResult;
}
