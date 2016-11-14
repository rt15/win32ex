#include "cbclimber.h"

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

RT_UN16 RT_CALL CbAddClimber(void* lpContext)
{
  CB_COMPETITION* lpCompetition;
  CB_CLIMBER* lpClimber;
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
    if (nRead >= CB_CLIMBER_NAME_SIZE)
    {
      RtWriteStringToConsole(_R("Too long name.\n"));
    }
    else
    {
      lpCompetition = (CB_COMPETITION*)lpContext;
      if (!RtNewArrayItem((void**)&lpCompetition->lpClimbers, (void**)&lpClimber))
      {
        nResult = 1;
        break;
      }
      RtCopyStringWithSize(lpClimber->lpName, CB_CLIMBER_NAME_SIZE, lpName, nRead);
    }
  }
  return nResult;
}

RT_UN16 RT_CALL CbDeleteClimber(void* lpContext)
{
  CB_COMPETITION* lpCompetition;
  RT_N nClimbersCount;
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
    nClimbersCount = RtGetArraySize(lpCompetition->lpClimbers);
    for (nI = 0; nI < nClimbersCount; nI++)
    {
      RtConvertNToString(nI + 1, lpBuffer, 16);
      RtWriteStringsToConsole(4, lpBuffer, _R(": "), lpCompetition->lpClimbers[nI].lpName, _R("\n"));
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
      if ((nIndex >= 1) && (nIndex <= nClimbersCount))
      {
        if (!RtDeleteArrayItemIndex((void**)&lpCompetition->lpClimbers, nIndex - 1))
        {
          nResult = 1;
          break;
        }
      }
    }
  }

  return nResult;
}

CB_MENU_ITEM cb_lpClimbersMenuItems[2] = {{_R("Add"), &CbAddClimber}, {_R("Delete"), &CbDeleteClimber}};


RT_UN16 RT_CALL CbClimbersHeaderCallback(void* lpContext)
{
  CB_COMPETITION* lpCompetition;
  RT_N nClimbersCount;
  RT_N nI;

  CbWriteMenuTitle(_R("Climbers"));
  lpCompetition = (CB_COMPETITION*)lpContext;

  CbWriteSeparator();
  nClimbersCount = RtGetArraySize(lpCompetition->lpClimbers);
  for (nI = 0; nI < nClimbersCount; nI++)
  {
    RtWriteStringsToConsole(2, lpCompetition->lpClimbers[nI].lpName, _R("\n"));
  }
  CbWriteSeparator();

  return 0;
}

RT_UN16 RT_CALL CbClimbers(void* lpContext)
{
  RT_UN32 unResult;

  nResult = CbManageMenu(&CbClimbersHeaderCallback, cb_lpClimbersMenuItems, 2, lpContext);

  return nResult;
}

