#include "cbgrade.h"

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

RT_UN16 RT_CALL CbAddGrade(void* lpContext)
{
  CB_COMPETITION* lpCompetition;
  CB_GRADE* lpGrade;
  RT_CHAR lpLabel[256];
  RT_N nRead;
  RT_UN32 unResult;

  while (RT_TRUE)
  {
    RtWriteStringToConsole(_R("\nEnter a label or nothing to quit:\n"));
    nRead = RtReadLineFromConsole(lpLabel, 256);

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

    /* Too long label. */
    if (nRead >= CB_GRADE_LABEL_SIZE)
    {
      RtWriteStringToConsole(_R("Too long label.\n"));
    }
    else
    {
      lpCompetition = (CB_COMPETITION*)lpContext;
      if (!RtNewArrayItem((void**)&lpCompetition->lpGrades, (void**)&lpGrade))
      {
        nResult = 1;
        break;
      }
      RtCopyStringWithSize(lpGrade->lpLabel, CB_GRADE_LABEL_SIZE, lpLabel, nRead);
    }
  }
  return nResult;
}

RT_UN16 RT_CALL CbDeleteGrade(void* lpContext)
{
  CB_COMPETITION* lpCompetition;
  RT_N nGradesCount;
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
    nGradesCount = RtGetArraySize(lpCompetition->lpGrades);
    for (nI = 0; nI < nGradesCount; nI++)
    {
      RtConvertNToString(nI + 1, lpBuffer, 16);
      RtWriteStringsToConsole(4, lpBuffer, _R(": "), lpCompetition->lpGrades[nI].lpLabel, _R("\n"));
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
      if ((nIndex >= 1) && (nIndex <= nGradesCount))
      {
        if (!RtDeleteArrayItemIndex((void**)&lpCompetition->lpGrades, nIndex - 1))
        {
          nResult = 1;
          break;
        }
      }
    }
  }

  return nResult;
}

CB_MENU_ITEM cb_lpGradesMenuItems[2] = {{_R("Add"), &CbAddGrade}, {_R("Delete"), &CbDeleteGrade}};


RT_UN16 RT_CALL CbGradesHeaderCallback(void* lpContext)
{
  CB_COMPETITION* lpCompetition;
  RT_N nGradesCount;
  RT_N nI;

  CbWriteMenuTitle(_R("Grades"));
  lpCompetition = (CB_COMPETITION*)lpContext;

  CbWriteSeparator();
  nGradesCount = RtGetArraySize(lpCompetition->lpGrades);
  for (nI = 0; nI < nGradesCount; nI++)
  {
    RtWriteStringsToConsole(2, lpCompetition->lpGrades[nI].lpLabel, _R("\n"));
  }
  CbWriteSeparator();

  return 0;
}

RT_UN16 RT_CALL CbGrades(void* lpContext)
{
  RT_UN32 unResult;

  nResult = CbManageMenu(&CbGradesHeaderCallback, cb_lpGradesMenuItems, 2, lpContext);

  return nResult;
}
