#include "cbmanager.h"

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

void RT_CALL CbDisplayList(CB_MANAGER_CONTEXT* lpManagerContext, RT_B bWithIndex)
{
  RT_TABLE* lpTable;
  RT_N nObjectsCount;
  CB_OBJECT* lpObject;
  RT_CHAR lpBuffer[16];
  RT_N nWritten;
  RT_N nI;

  lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];

  CbWriteSeparator();
  nObjectsCount = RtGetArraySize(lpTable->lpTableData);
  for (nI = 0; nI < nObjectsCount; nI++)
  {
    RtGetArrayItem(lpTable->lpTableData, lpTable->lpTableIndexes[0].lpIndexes[nI], (void**)&lpObject);
    if (bWithIndex)
    {
      RtConvertNumberToString(nI + 1, lpBuffer, 16, &nWritten);
      RtWriteStringsToConsole(4, lpBuffer, _R(": "), lpObject->lpName, _R("\n"));
    }
    else
    {
      RtWriteStringsToConsole(2, lpObject->lpName, _R("\n"));
    }
  }
  CbWriteSeparator();
}

RT_UN16 RT_CALL CbAddObject(void* lpContext)
{
  CB_MANAGER_CONTEXT* lpManagerContext;
  RT_TABLE* lpTable;
  void** lpTableData;
  CB_OBJECT* lpObject;
  RT_CHAR lpName[256];
  RT_N nRead;
  RT_N nWritten;
  RT_UN32 unResult;

  while (RT_TRUE)
  {
    RtWriteStringToConsole(_R("\nEnter a name or nothing to quit:\n"));
    nRead = RtReadLineFromConsole(lpName, 256);

    /* Error. */
    if (nRead == -1)
    {
      unResult = 1;
      break;
    }

    /* Exit. */
    if (nRead == 0)
    {
      unResult = 0;
      break;
    }

    /* Too long name. */
    if (nRead >= CB_NAME_SIZE)
    {
      RtWriteStringToConsole(_R("Too long name.\n"));
    }
    else
    {
      lpManagerContext = (CB_MANAGER_CONTEXT*)lpContext;
      lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];
      lpTableData = &lpTable->lpTableData;
      if (!RtNewArrayItem(lpTableData, (void**)&lpObject))
      {
        unResult = 1;
        break;
      }
      RtCopyStringWithSize(lpName, nRead, lpObject->lpName, CB_NAME_SIZE, &nWritten);
      if (!RtIndexNewTableItem(lpTable))
      {
        unResult = 1;
        break;
      }
    }
  }
  return unResult;
}

RT_UN16 RT_CALL CbDeleteObject(void* lpContext)
{
  CB_MANAGER_CONTEXT* lpManagerContext;
  RT_TABLE* lpTable;
  RT_N nObjectsCount;
  RT_CHAR lpIndex[256];
  RT_N nIndex;
  RT_N nRead;
  RT_UN32 unResult;

  lpManagerContext = (CB_MANAGER_CONTEXT*)lpContext;
  lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];

  while (RT_TRUE)
  {
    nObjectsCount = RtGetArraySize(lpTable->lpTableData);

    RtWriteStringToConsole(_R("\nEnter an index or nothing to quit\n"));

    CbDisplayList(lpManagerContext, RT_TRUE);

    nRead = RtReadLineFromConsole(lpIndex, 256);

    /* Error. */
    if (nRead == -1)
    {
      unResult = 1;
      break;
    }

    /* Exit. */
    if (nRead == 0)
    {
      unResult = 0;
      break;
    }

    /* Parse index. */
    if (RtConvertStringToNumber(lpIndex, &nIndex))
    {
      if (!nIndex)
      {
        unResult = 0;
        break;
      }
      if ((nIndex >= 1) && (nIndex <= nObjectsCount))
      {
        /* Retrieve data index from table index. */
        nIndex = lpTable->lpTableIndexes[0].lpIndexes[nIndex - 1];

        if (!RtDeleteTableItem(lpTable, nIndex))
        {
          unResult = 1;
          break;
        }
      }
    }
  }

  return unResult;
}

RT_UN16 RT_CALL CbEditObject(void* lpContext)
{
  CB_MANAGER_CONTEXT* lpManagerContext;
  RT_TABLE* lpTable;
  RT_N nObjectsCount;
  RT_CHAR lpIndex[256];
  RT_N nIndex;
  RT_N nRead;
  RT_UN32 unResult;

  lpManagerContext = (CB_MANAGER_CONTEXT*)lpContext;
  lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];

  while (RT_TRUE)
  {
    nObjectsCount = RtGetArraySize(lpTable->lpTableData);

    RtWriteStringToConsole(_R("\nEnter an index or nothing to quit\n"));

    CbDisplayList(lpManagerContext, RT_TRUE);

    nRead = RtReadLineFromConsole(lpIndex, 256);

    /* Error. */
    if (nRead == -1)
    {
      unResult = 1;
      break;
    }

    /* Exit. */
    if (nRead == 0)
    {
      unResult = 0;
      break;
    }

    /* Parse index. */
    if (RtConvertStringToNumber(lpIndex, &nIndex))
    {
      if (!nIndex)
      {
        unResult = 0;
        break;
      }
      if ((nIndex >= 1) && (nIndex <= nObjectsCount))
      {
        /* Retrieve data index from table index. */
        nIndex = lpTable->lpTableIndexes[0].lpIndexes[nIndex - 1];

        if (!RtDeleteTableItem(lpTable, nIndex))
        {
          unResult = 1;
          break;
        }
      }
    }
  }

  return unResult;
}


CB_MENU_ITEM cb_lpMenuItems[3] = {{_R("Add"), &CbAddObject}, {_R("Edit"), &CbEditObject},  {_R("Delete"), &CbDeleteObject}};


RT_UN16 RT_CALL CbHeaderCallback(void* lpContext)
{
  CB_MANAGER_CONTEXT* lpManagerContext;

  lpManagerContext = (CB_MANAGER_CONTEXT*)lpContext;

  CbWriteMenuTitle(lpManagerContext->lpClassName);

  CbDisplayList(lpManagerContext, RT_FALSE);

  return 0;
}

RT_UN16 RT_CALL CbManageEntities(CB_MANAGER_CONTEXT* lpContext)
{
  return CbManageMenu(&CbHeaderCallback, cb_lpMenuItems, 3, lpContext);
}

