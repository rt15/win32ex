#ifndef RT_TABLE_H
#define RT_TABLE_H

#include "layer000/RtWin32ExTypes.h"
#include "layer003/RtHeap.h"
#include "layer006/RtTableMetadata.h"

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

RT_B RT_API RtTable_Create(RT_TABLE* lpTable, RT_TABLE_METADATA* lpTableMetadata, RT_HEAP** lpHeap);


void* RT_API RtTable_AcquireItem(RT_TABLE* lpTable, void** lpItems);

/**
 * When an item has been added in the data array, it often must be then indexed.
 */
RT_B RT_API RtTable_IndexNewItem(RT_TABLE* lpTable);

void RT_API RtTable_IndexUpdatedItem(RT_TABLE* lpTable, RT_UN unItemIndex, RT_UN unIndex);

/**
 * Delete an item from the data table and remove indexes entries.
 */
RT_B RT_API RtTable_DeleteItem(RT_TABLE* lpTable, RT_UN unItemIndex);

RT_B RT_API RtTable_Free(RT_TABLE* lpTable);

#endif /* RT_TABLE_H */
