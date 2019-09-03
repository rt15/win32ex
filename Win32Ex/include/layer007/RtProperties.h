#ifndef RT_PROPERTIES_H
#define RT_PROPERTIES_H

#include "layer000/RtWin32ExTypes.h"
#include "layer003/RtHeap.h"

/**
 * @file
 * Properties file management.
 */

typedef struct _RT_PROPERTY_ENTRY
{
  RT_UN unHash;
  RT_CHAR* lpKey;
  RT_CHAR* lpValue;
}
RT_PROPERTY_ENTRY;

typedef struct _RT_PROPERTIES
{
  RT_PROPERTY_ENTRY* lpPropertiesEntries;
  RT_CHAR* lpData;
  RT_HEAP** lpHeap;
}
RT_PROPERTIES;

RT_B RT_API RtProperties_Create(RT_PROPERTIES* lpProperties, RT_CHAR* lpFilePath, RT_UN unEncoding, RT_HEAP** lpHeap);

RT_CHAR* RT_API RtProperties_GetString(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_CHAR* lpDefaultValue);

RT_N RT_API RtProperties_GetInteger(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_N nDefaultValue);

RT_B RT_API RtProperties_GetBoolean(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_B bDefaultValue);

RT_B RT_API RtProperties_Free(RT_PROPERTIES* lpProperties);

#endif /* RT_PROPERTIES_H */
