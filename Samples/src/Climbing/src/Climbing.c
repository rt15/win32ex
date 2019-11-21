#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

#include "cbmanager.h"

RT_B RT_CALL CbCompetitionHeaderCallback(void* lpContext)
{
  CbWriteMenuTitle(_R("Competition"));
  return RT_SUCCESS;
}

RT_UN16 RT_CALL CbCallManager(void* lpContext, RT_N nClass, RT_CHAR* lpClassName)
{
  CB_MANAGER_CONTEXT managerContext;

  managerContext.lpCompetition = lpContext;
  managerContext.nClass = nClass;
  managerContext.lpClassName = lpClassName;

  return CbManageEntities(&managerContext);
}

RT_B RT_CALL CbClimbers(void* lpContext)
{
  return CbCallManager(lpContext, CB_CLIMBERS_INDEX, _R("Climbers"));
}

RT_B RT_CALL CbRoutes(void* lpContext)
{
  return CbCallManager(lpContext, CB_ROUTES_INDEX, _R("Routes"));
}

RT_B RT_CALL CbGrades(void* lpContext)
{
  return CbCallManager(lpContext, CB_GRADES_INDEX, _R("Grades"));
}


CB_MENU_ITEM cb_lpCompetitionMenuItems[3] = {{_R("Climbers"), &CbClimbers},
                                             {_R("Routes"), &CbRoutes},
                                             {_R("Grades"), &CbGrades}};


RT_B RT_CALL CbNameComparisonCallback(void* lpItem1, void* lpItem2, void* lpContext, RT_N* lpComparisonResult)
{
  RT_TABLE* lpTable;
  RT_N* lpIndex1;
  RT_N* lpIndex2;
  RT_ARRAY zzString1;
  RT_ARRAY zzString2;
  RT_CHAR* lpString1;
  RT_CHAR* lpString2;

  lpTable = (RT_TABLE*)lpContext;

  lpIndex1 = (RT_N*)lpItem1;
  lpIndex2 = (RT_N*)lpItem2;

  lpString1 = (RT_CHAR*)(((RT_UCHAR8*)lpTable->lpTableData) + (lpTable->lpTableMetadata->unItemSize * *lpIndex1));
  lpString2 = (RT_CHAR*)(((RT_UCHAR8*)lpTable->lpTableData) + (lpTable->lpTableMetadata->unItemSize * *lpIndex2));

  RtChar_CreateString(&zzString1, lpString1);
  RtChar_CreateString(&zzString2, lpString2);
  *lpComparisonResult = RtChar_CompareStrings(&zzString1, &zzString2);
  return RT_TRUE;
}

RT_COMPARISON_CALLBACK cb_lpComparisonCallbacks[] = { &CbNameComparisonCallback };

RT_TABLE_METADATA cb_lpTablesMetadata[] = {
                                            { sizeof(CB_CLIMBER), sizeof(CB_CLIMBER_ITEM), 1, cb_lpComparisonCallbacks },
                                            { sizeof(CB_ROUTE),   sizeof(CB_ROUTE_ITEM),   1, cb_lpComparisonCallbacks },
                                            { sizeof(CB_GRADE),   sizeof(CB_GRADE_ITEM),   1, cb_lpComparisonCallbacks }
                                          };

RT_B RT_CALL ZzMain()
{
  CB_COMPETITION competition;
  RT_B bTableCreated[CB_TABLES_COUNT];
  RtRuntimeHeap runtimeHeap;
  RT_B bRuntimeHeapCreated;
  RT_UN unI;
  RT_B bResult;

  bRuntimeHeapCreated = RT_FALSE;

  for (unI = 0; unI < CB_TABLES_COUNT; unI++)
  {
    bTableCreated[unI] = RT_FALSE;
  }

  if (!RtRuntimeHeap_Create(&runtimeHeap))
  {
    RtErrorMessage_WriteLast(_R("Runtime heap creation failed: "));
    goto handle_error;
  }
  bRuntimeHeapCreated = RT_TRUE;

  for (unI = 0; unI < CB_TABLES_COUNT; unI++)
  {
    if (!RtTable_Create(&competition.lpTables[unI], &cb_lpTablesMetadata[unI], &runtimeHeap.lpHeap))
    {
      RtErrorMessage_WriteLast(_R("Tables creation failed: "));
      goto handle_error;
    }
    bTableCreated[unI] = RT_TRUE;
  }

  if (!CbManageMenu(&CbCompetitionHeaderCallback, cb_lpCompetitionMenuItems, 3, &competition)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  for (unI = 0; unI < CB_TABLES_COUNT; unI++)
  {
    if (bTableCreated[unI])
    {
      bTableCreated[unI] = RT_FALSE;
      if (!RtTable_Free(&competition.lpTables[unI]) && bResult)
      {
        RtErrorMessage_WriteLast(_R("Failed to free tables: "));
        goto handle_error;
      }
    }
  }
  if (bRuntimeHeapCreated)
  {
    bRuntimeHeapCreated = RT_FALSE;
    if (!runtimeHeap.lpHeap->lpClose(&runtimeHeap) && bResult)
    {
      RtErrorMessage_WriteLast(_R("Failed to close runtime heap: "));
      goto handle_error;
    }
  }

  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_UN16 unResult;
  if (ZzMain())
  {
    unResult = 0;
  }
  else
  {
    unResult = 1;
  }
  return unResult;
}
