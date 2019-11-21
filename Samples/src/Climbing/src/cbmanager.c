#include "cbmanager.h"

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

#define ZZ_MENU_ITEMS_COUNT 2

RT_B RT_CALL CbDisplayList(CB_MANAGER_CONTEXT* lpManagerContext, RT_B bWithIndex)
{
  RT_ARRAY rtMessage;
  RT_CHAR lpBuffer[256];
  RT_TABLE* lpTable;
  RT_N nObjectsCount;
  CB_OBJECT* lpObject;
  RT_N nI;
  RT_B bResult;

  lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];

  RtArray_Create(&rtMessage, lpBuffer, sizeof(RT_CHAR), 256);

  CbWriteSeparator();
  nObjectsCount = RtArray_GetSize(lpTable->lpTableData);
  for (nI = 0; nI < nObjectsCount; nI++)
  {
    RtArray_GetItem(lpTable->lpTableData, lpTable->lpTableIndexes[0].lpIndexes[nI], (void**)&lpObject);
    if (bWithIndex)
    {
      if (!RtChar_ConvertIntegerToString(nI + 1, &rtMessage)) goto handle_error;
      if (!RtChar_AppendCString(&rtMessage, _R(": "))) goto handle_error;
    }

    if (!RtChar_AppendCString(&rtMessage, lpObject->lpName)) goto handle_error;
    if (!RtChar_AppendCString(&rtMessage, _R("\n"))) goto handle_error;

    if (!RtConsole_WriteString(&rtMessage)) goto handle_error;
    if (!RtArray_SetSize(&rtMessage, 0)) goto handle_error;
  }
  CbWriteSeparator();

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL CbAddObject(void* lpContext)
{
  CB_MANAGER_CONTEXT* lpManagerContext;
  RT_TABLE* lpTable;
  void** lpTableData;
  CB_OBJECT* lpObject;
  RT_CHAR lpName[256];
  RT_UN unRead;
  RT_UN unOutputSize;
  RT_B bResult;

  while (RT_TRUE)
  {
    RtConsole_WriteCString(_R("\nEnter a name or nothing to quit:\n"));
    unRead = RtConsole_ReadLine(lpName, 256);

    /* Error. */
    if (unRead == RT_TYPE_MAX_UN) goto handle_error;

    /* Exit. */
    if (unRead == 0)
    {
      break;
    }

    /* Too long name. */
    if (unRead >= CB_NAME_SIZE)
    {
      RtConsole_WriteCString(_R("Too long name.\n"));
    }
    else
    {
      lpManagerContext = (CB_MANAGER_CONTEXT*)lpContext;
      lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];
      lpTableData = &lpTable->lpTableData;
      if (!RtArray_NewItem(lpTableData, (void**)&lpObject)) goto handle_error;
      RtChar_CopyStringWithSize(lpName, unRead, lpObject->lpName, CB_NAME_SIZE, &unOutputSize);
      if (!RtTable_IndexNewItem(lpTable)) goto handle_error;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL CbDeleteObject(void* lpContext)
{
  RT_ARRAY zzString;
  CB_MANAGER_CONTEXT* lpManagerContext;
  RT_TABLE* lpTable;
  RT_N nObjectsCount;
  RT_CHAR lpIndex[256];
  RT_UN unIndex;
  RT_UN unRead;
  RT_B bResult;

  lpManagerContext = (CB_MANAGER_CONTEXT*)lpContext;
  lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];

  while (RT_TRUE)
  {
    nObjectsCount = RtArray_GetSize(lpTable->lpTableData);

    RtConsole_WriteCString(_R("\nEnter an index or nothing to quit\n"));

    CbDisplayList(lpManagerContext, RT_TRUE);

    unRead = RtConsole_ReadLine(lpIndex, 256);

    /* Error. */
    if (unRead == RT_TYPE_MAX_UN) goto handle_error;

    /* Exit. */
    if (unRead == 0)
    {
      break;
    }

    /* Parse index. */
    if (RtChar_ConvertStringToUnsignedInteger(RtChar_CreateString(&zzString, lpIndex), &unIndex))
    {
      if (!unIndex)
      {
        break;
      }
      if (unIndex <= nObjectsCount)
      {
        /* Retrieve data index from table index. */
        unIndex = lpTable->lpTableIndexes[0].lpIndexes[unIndex - 1];

        if (!RtTable_DeleteItem(lpTable, unIndex)) goto handle_error;
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

CB_MENU_ITEM cb_lpMenuItems[ZZ_MENU_ITEMS_COUNT] = {{_R("Add"), &CbAddObject}, {_R("Delete"), &CbDeleteObject}};

RT_B RT_CALL CbHeaderCallback(void* lpContext)
{
  CB_MANAGER_CONTEXT* lpManagerContext;

  lpManagerContext = (CB_MANAGER_CONTEXT*)lpContext;

  CbWriteMenuTitle(lpManagerContext->lpClassName);

  CbDisplayList(lpManagerContext, RT_FALSE);

  return RT_SUCCESS;
}

RT_B RT_CALL CbManageEntities(CB_MANAGER_CONTEXT* lpContext)
{
  return CbManageMenu(&CbHeaderCallback, cb_lpMenuItems, ZZ_MENU_ITEMS_COUNT, lpContext);
}
