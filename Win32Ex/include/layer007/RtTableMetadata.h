#ifndef RT_TABLE_METADATA_H
#define RT_TABLE_METADATA_H

#include "layer000/RtWin32ExTypes.h"
#include "layer003/RtComparisonCallback.h"

typedef struct _RT_TABLE_METADATA
{
  RT_N nItemUserDataSize;                        /* Size of user data in the table.       */
  RT_N nItemSize;                                /* Size of the items in the table.       */
  RT_N nTableIndexesCount;                       /* Count of indexes in the table.        */
  RT_COMPARISON_CALLBACK* lpComparisonCallbacks; /* Comparison callbacks used by indexes. */
}
RT_TABLE_METADATA;

#endif /* RT_TABLE_METADATA_H */