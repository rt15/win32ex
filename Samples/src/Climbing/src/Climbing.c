#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

#include "cbmanager.h"

RT_UN16 RT_CALL CbCompetitionHeaderCallback(void* lpContext)
{
  CbWriteMenuTitle(_R("Competition"));
  return 0;
}

RT_UN16 RT_CALL CbCallManager(void* lpContext, RT_N nClass, RT_CHAR* lpClassName)
{
  CB_MANAGER_CONTEXT managerContext;

  managerContext.lpCompetition = lpContext;
  managerContext.nClass = nClass;
  managerContext.lpClassName = lpClassName;

  return CbManageEntities(&managerContext);
}

RT_UN16 RT_CALL CbClimbers(void* lpContext)
{
  return CbCallManager(lpContext, CB_CLIMBERS_INDEX, _R("Climbers"));
}

RT_UN16 RT_CALL CbRoutes(void* lpContext)
{
  return CbCallManager(lpContext, CB_ROUTES_INDEX, _R("Routes"));
}

RT_UN16 RT_CALL CbGrades(void* lpContext)
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
  RT_CHAR* lpString1;
  RT_CHAR* lpString2;

  lpTable = (RT_TABLE*)lpContext;

  lpIndex1 = (RT_N*)lpItem1;
  lpIndex2 = (RT_N*)lpItem2;

  lpString1 = (RT_CHAR*)(((RT_UCHAR8*)lpTable->lpTableData) + (lpTable->lpTableMetadata->unItemSize * *lpIndex1));
  lpString2 = (RT_CHAR*)(((RT_UCHAR8*)lpTable->lpTableData) + (lpTable->lpTableMetadata->unItemSize * *lpIndex2));

  *lpComparisonResult = RtCompareStrings(lpString1, lpString2);
  return RT_TRUE;
}

RT_COMPARISON_CALLBACK cb_lpComparisonCallbacks[] = { &CbNameComparisonCallback };

RT_TABLE_METADATA cb_lpTablesMetadata[] = {
                                            { sizeof(CB_CLIMBER), sizeof(CB_CLIMBER_ITEM), 1, cb_lpComparisonCallbacks },
                                            { sizeof(CB_ROUTE),   sizeof(CB_ROUTE_ITEM),   1, cb_lpComparisonCallbacks },
                                            { sizeof(CB_GRADE),   sizeof(CB_GRADE_ITEM),   1, cb_lpComparisonCallbacks }
                                          };

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  CB_COMPETITION competition;
  RtRuntimeHeap runtimeHeap;
  RT_N nI, unJ;
  RT_UN32 unResult;

  if (!RtRuntimeHeapCreate(&runtimeHeap))
  {
    unResult = CbWriteLastErrorMessage(_R("Runtime heap creation failed: "));
    goto the_end;
  }

  for (nI = 0; nI < CB_TABLES_COUNT; nI++)
  {
    if (!RtCreateTable(&competition.lpTables[nI], &cb_lpTablesMetadata[nI], &runtimeHeap.lpHeap))
    {
      unResult = CbWriteLastErrorMessage(_R("Tables creation failed: "));
      goto cleanup_tables;
    }
  }

  unResult = CbManageMenu(&CbCompetitionHeaderCallback, cb_lpCompetitionMenuItems, 3, &competition);

cleanup_tables:
  /* Cleaning up. */
  for (unJ = 0; unJ < nI; unJ++)
  {
    if (!RtFreeTable(&competition.lpTables[unJ]))
    {
      CbWriteLastErrorMessage(_R("Failed to free tables: "));
      unResult = 1;
    }
  }

  if (!runtimeHeap.lpHeap->lpClose(&runtimeHeap))
  {
    CbWriteLastErrorMessage(_R("Failed to close runtime heap: "));
    unResult = 1;
  }
the_end:
  return unResult;
}
