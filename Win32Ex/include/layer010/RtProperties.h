#ifndef RT_PROPERTIES_H
#define RT_PROPERTIES_H

#include "layer000/RtWin32ExTypes.h"
#include "layer005/RtHeap.h"

/**
 * @file
 * Properties file management.
 */

typedef struct _RT_PROPERTY_ENTRY
{
  RT_N nHash;
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

RT_B RT_API RtCreateProperties(RT_PROPERTIES* lpProperties, RT_CHAR* lpFilePath, RT_N nEncoding, RT_HEAP** lpHeap);

RT_CHAR* RT_API RtGetStringProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_CHAR* lpDefaultValue);

RT_N RT_API RtGetNumberProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_N nDefaultValue);

RT_B RT_API RtGetBooleanProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_B bDefaultValue);

RT_B RT_API RtFreeProperties(RT_PROPERTIES* lpProperties);

#endif /* RT_PROPERTIES_H */
