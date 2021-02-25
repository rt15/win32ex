#ifndef RT_PROPERTIES_H
#define RT_PROPERTIES_H

#include "layer000/rt_types.h"
#include "layer003/rt_heap.h"

/**
 * @file
 * Properties file management.
 */

typedef struct _RT_PROPERTY_ENTRY {
	rt_un unHash;
	rt_char *lpKey;
	rt_char *value;
}
RT_PROPERTY_ENTRY;

typedef struct _RT_PROPERTIES {
	RT_PROPERTY_ENTRY *lpPropertiesEntries;
	rt_char *data;
	struct rt_heap **heap;
}
RT_PROPERTIES;

rt_s rt_properties_Create(RT_PROPERTIES *lpProperties, rt_char *file_path, enum rt_encoding encoding, struct rt_heap **heap);

rt_char *rt_properties_GetString(RT_PROPERTIES *lpProperties, rt_char *lpKey, rt_char *lpDefaultValue);

rt_un rt_properties_GetUnsignedInteger(RT_PROPERTIES *lpProperties, rt_char *lpKey, rt_un unDefaultValue);

rt_s rt_properties_GetBoolean(RT_PROPERTIES *lpProperties, rt_char *lpKey, rt_b bDefaultValue);

rt_s rt_properties_Free(RT_PROPERTIES *lpProperties);

#endif /* RT_PROPERTIES_H */
