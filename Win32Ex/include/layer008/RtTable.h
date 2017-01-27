#ifndef RT_TABLE_H
#define RT_TABLE_H

#include "layer000/RtWin32ExTypes.h"
#include "layer005/RtHeap.h"
#include "layer007/RtTableMetadata.h"

typedef struct _RT_TABLE_INDEX
{
  RT_UN* lpIndexes;
  RT_B bEnabled;
}
RT_TABLE_INDEX;

typedef struct _RT_TABLE
{
  RT_TABLE_METADATA* lpTableMetadata;
  void* lpTableData;
  RT_TABLE_INDEX* lpTableIndexes;
}
RT_TABLE;

RT_B RT_API RtCreateTable(RT_TABLE* lpTable, RT_TABLE_METADATA* lpTableMetadata, RT_HEAP** lpHeap);


void* RT_API RtAcquireTableItem(RT_TABLE* lpTable, void** lpItems);

/**
 * When an item has been added in the data array, it often must be then indexed.
 */
RT_B RT_API RtIndexNewTableItem(RT_TABLE* lpTable);

void RT_API RtIndexUpdatedTableItem(RT_TABLE* lpTable, RT_UN unItemIndex, RT_UN unIndex);

/**
 * Delete an item from the data table and remove indexes entries.
 */
RT_B RT_API RtDeleteTableItem(RT_TABLE* lpTable, RT_UN unItemIndex);

RT_B RT_API RtFreeTable(RT_TABLE* lpTable);

#endif /* RT_TABLE_H */
