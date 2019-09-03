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
  RT_UN unWritten;
  RT_N nI;

  lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];

  CbWriteSeparator();
  nObjectsCount = RtArray_GetSize(lpTable->lpTableData);
  for (nI = 0; nI < nObjectsCount; nI++)
  {
    RtArray_GetItem(lpTable->lpTableData, lpTable->lpTableIndexes[0].lpIndexes[nI], (void**)&lpObject);
    if (bWithIndex)
    {
      RtChar_ConvertIntegerToString(nI + 1, lpBuffer, 16, &unWritten);
      RtConsole_WriteStringsOrErrors(RT_TRUE, lpBuffer, _R(": "), lpObject->lpName, _R("\n"), (RT_CHAR*)RT_NULL);
    }
    else
    {
      RtConsole_WriteStringsOrErrors(RT_TRUE, lpObject->lpName, _R("\n"), (RT_CHAR*)RT_NULL);
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
  RT_UN unRead;
  RT_UN unWritten;
  RT_UN32 unResult;

  while (RT_TRUE)
  {
    RtConsole_WriteString(_R("\nEnter a name or nothing to quit:\n"));
    unRead = RtConsole_ReadLine(lpName, 256);

    /* Error. */
    if (unRead == RT_TYPE_MAX_UN)
    {
      unResult = 1;
      break;
    }

    /* Exit. */
    if (unRead == 0)
    {
      unResult = 0;
      break;
    }

    /* Too long name. */
    if (unRead >= CB_NAME_SIZE)
    {
      RtConsole_WriteString(_R("Too long name.\n"));
    }
    else
    {
      lpManagerContext = (CB_MANAGER_CONTEXT*)lpContext;
      lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];
      lpTableData = &lpTable->lpTableData;
      if (!RtArray_NewItem(lpTableData, (void**)&lpObject))
      {
        unResult = 1;
        break;
      }
      RtChar_CopyStringWithSize(lpName, unRead, lpObject->lpName, CB_NAME_SIZE, &unWritten);
      if (!RtTable_IndexNewItem(lpTable))
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
  RT_UN unRead;
  RT_UN32 unResult;

  lpManagerContext = (CB_MANAGER_CONTEXT*)lpContext;
  lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];

  while (RT_TRUE)
  {
    nObjectsCount = RtArray_GetSize(lpTable->lpTableData);

    RtConsole_WriteString(_R("\nEnter an index or nothing to quit\n"));

    CbDisplayList(lpManagerContext, RT_TRUE);

    unRead = RtConsole_ReadLine(lpIndex, 256);

    /* Error. */
    if (unRead == RT_TYPE_MAX_UN)
    {
      unResult = 1;
      break;
    }

    /* Exit. */
    if (unRead == 0)
    {
      unResult = 0;
      break;
    }

    /* Parse index. */
    if (RtChar_ConvertStringToInteger(lpIndex, &nIndex))
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

        if (!RtTable_DeleteItem(lpTable, nIndex))
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
  RT_UN unRead;
  RT_UN32 unResult;

  lpManagerContext = (CB_MANAGER_CONTEXT*)lpContext;
  lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];

  while (RT_TRUE)
  {
    nObjectsCount = RtArray_GetSize(lpTable->lpTableData);

    RtConsole_WriteString(_R("\nEnter an index or nothing to quit\n"));

    CbDisplayList(lpManagerContext, RT_TRUE);

    unRead = RtConsole_ReadLine(lpIndex, 256);

    /* Error. */
    if (unRead == RT_TYPE_MAX_UN)
    {
      unResult = 1;
      break;
    }

    /* Exit. */
    if (unRead == 0)
    {
      unResult = 0;
      break;
    }

    /* Parse index. */
    if (RtChar_ConvertStringToInteger(lpIndex, &nIndex))
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

        if (!RtTable_DeleteItem(lpTable, nIndex))
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

