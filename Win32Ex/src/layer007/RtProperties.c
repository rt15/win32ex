#include "layer007/rt_properties.h"

#include "layer002/rt_binary_search.h"
#include "layer003/rt_char.h"
#include "layer004/rt_encoding.h"
#include "layer005/rt_sortable_array.h"
#include "layer006/rt_small_file.h"

rt_s RT_CALL rt_properties_GetEntries(RT_PROPERTY_ENTRY **lpPropertiesEntries, rt_char *data)
{
	RT_PROPERTY_ENTRY *lpPropertyEntry;
	rt_char *lpKey;
	rt_char *value;
	rt_un unCurrentIndex;
	rt_char current_char;
	rt_un unContext; /* 0 = in end of lines, 1 = in key, 2 = in separator, 3 = in value. */
	rt_s ret;

	lpKey = RT_NULL;
	value = RT_NULL;

	ret = RT_FAILED;

	unCurrentIndex = 0;
	unContext = 0;
	while (data[unCurrentIndex]) {
		current_char = data[unCurrentIndex];
		if (unContext == 0) {
			/* Manage end of lines. */
			switch (current_char)
			{
				case _R('\r'):
				case _R('\n'):
				case _R(' '):
					/* Skip all end of line until something interesting is found. */
					data[unCurrentIndex] = 0;
					break;
				case _R('='):
					unContext = 2;
					data[unCurrentIndex] = 0;
					lpKey = &data[unCurrentIndex];
					break;
				default:
					unContext = 1;
					lpKey = &data[unCurrentIndex];
			}
		} else if (unContext == 1) {
			/* Manage key and spaces until '='. */
			switch (current_char)
			{
				case _R('='):
					unContext = 2;
				case _R(' '):
					data[unCurrentIndex] = 0;
					break;
				case _R('\n'):
				case _R('\r'):
					if (!rt_array_new_item((void**)lpPropertiesEntries, (void**)&lpPropertyEntry)) {
						goto the_end;
					}
					/* Fill the new entry. */
					data[unCurrentIndex] = 0;
					value = &data[unCurrentIndex];
					lpPropertyEntry->unHash = rt_char_GetStringHash(lpKey);
					lpPropertyEntry->lpKey = lpKey;
					lpPropertyEntry->value = value;
					unContext = 0;
					break;
			}
		} else if (unContext == 2) {
			/* Manage spaces between '=' and value. */
			switch (current_char)
			{
				case _R(' '):
					/* Skip all spaces after '='. */
					data[unCurrentIndex] = 0;
					break;
				case _R('\r'):
				case _R('\n'):
					data[unCurrentIndex] = 0;
				default:
					/* Beginning of the value. */
					value = &data[unCurrentIndex];
					unContext = 3;
			}
		} else if (unContext == 3) {
			/* Manage value. */
			switch (current_char)
			{
				case _R('\r'):
				case _R('\n'):
					if (!rt_array_new_item((void**)lpPropertiesEntries, (void**)&lpPropertyEntry)) {
						goto the_end;
					}
					/* Fill the new entry. */
					lpPropertyEntry->unHash = rt_char_GetStringHash(lpKey);
					lpPropertyEntry->lpKey = lpKey;
					lpPropertyEntry->value = value;
					data[unCurrentIndex] = 0;
					unContext = 0;
					break;
			}
		}

		unCurrentIndex++;
	}

	switch (unContext)
	{
		case 1:
		case 2:
			value = &data[unCurrentIndex];
		case 3:
			if (!rt_array_new_item((void**)lpPropertiesEntries, (void**)&lpPropertyEntry)) {
				goto the_end;
			}
			/* Fill the new entry. */
			lpPropertyEntry->unHash = rt_char_GetStringHash(lpKey);
			lpPropertyEntry->lpKey = lpKey;
			lpPropertyEntry->value = value;
			break;
	}

	ret = rt_sortable_array_sort(*lpPropertiesEntries);
the_end:
	return ret;
}

rt_s RT_CALL rt_properties_CreateFromString(RT_PROPERTIES *lpProperties, rt_char *data, struct rt_heap **heap)
{
	RT_PROPERTY_ENTRY *lpPropertiesEntries;
	rt_s ret;

	if (!rt_sortable_array_create((void**)&lpPropertiesEntries, heap, 0, sizeof(RT_PROPERTY_ENTRY), &rt_comparison_DefaultCallback, RT_NULL)) {
		ret = RT_FAILED;
		goto the_end;
	}

	if (rt_properties_GetEntries(&lpPropertiesEntries, data)) {
		/* Fill the structure. */
		lpProperties->lpPropertiesEntries = lpPropertiesEntries;
		lpProperties->data = data;
		lpProperties->heap = heap;
		ret = RT_OK;
	} else {
		rt_array_free((void**)&lpPropertiesEntries);
		ret = RT_FAILED;
	}

the_end:
	return ret;
}

rt_s RT_CALL rt_properties_CreateFromBuffer(RT_PROPERTIES *lpProperties, rt_char8 *buffer, rt_un buffer_capacity, enum rt_encoding encoding, struct rt_heap **heap)
{
	rt_char *data;
	rt_s ret;

	ret = RT_FAILED;

	if (rt_encoding_decode_with_heap(buffer, buffer_capacity, encoding, &data, heap) == RT_TYPE_MAX_UN) {
		goto the_end;
	}

	ret = rt_properties_CreateFromString(lpProperties, data, heap);
	if (!ret) {
		(*heap)->free(heap, (void**)&data);
	}
the_end:
	return ret;
}

rt_s rt_properties_Create(RT_PROPERTIES *lpProperties, rt_char *file_path, enum rt_encoding encoding, struct rt_heap **heap)
{
	rt_char8 *lpFileContent;
	rt_un file_size;
	rt_s ret;

	ret = RT_FAILED;

	file_size = rt_small_file_read(file_path, &lpFileContent, heap);
	if (file_size == RT_TYPE_MAX_UN) {
		goto the_end;
	}

	ret = rt_properties_CreateFromBuffer(lpProperties, lpFileContent, file_size, encoding, heap);

	ret = ret && (*heap)->free(heap, (void**)&lpFileContent);
the_end:
	return ret;
}

rt_char *rt_properties_GetString(RT_PROPERTIES *lpProperties, rt_char *lpKey, rt_char *lpDefaultValue)
{
	rt_un unHash;
	RT_PROPERTY_ENTRY *lpPropertiesEntries;
	rt_un index;
	rt_un size;
	rt_char *result;

	result = lpDefaultValue;

	unHash = rt_char_GetStringHash(lpKey);
	lpPropertiesEntries = lpProperties->lpPropertiesEntries;

	size = rt_array_get_size(lpPropertiesEntries);

	/* No need to check result as the comparison function cannot trigger error. */
	rt_sortable_array_search_item_index(lpPropertiesEntries, &unHash, &index);
	if (index != RT_TYPE_MAX_UN) {
		/* Find the first item with the given hash. */
		while (index > 0 && (lpPropertiesEntries[index - 1].unHash == unHash)) {
			index--;
		}

		/* Find the right item by checking the key. */
		while ((index < RT_TYPE_MAX_N) && (index < size) && (lpPropertiesEntries[index].unHash == unHash)) {
			if (!rt_char1337_CompareStrings(lpPropertiesEntries[index].lpKey, lpKey)) {
				result = lpPropertiesEntries[index].value;
				break;
			}
			index++;
		}
	}

	return result;
}

rt_un rt_properties_GetUnsignedInteger(RT_PROPERTIES *lpProperties, rt_char *lpKey, rt_un unDefaultValue)
{
	RT_ARRAY rtString;
	rt_char *lpStringProperty;
	rt_un result;

	lpStringProperty = rt_properties_GetString(lpProperties, lpKey, RT_NULL);
	if (!lpStringProperty)
		goto error;
	if (!rt_char_convert_to_un(rt_char_CreateString(&rtString, lpStringProperty), &result))
		goto error;

	goto free;
error:
	result = unDefaultValue;
free:
	return result;
}

rt_s rt_properties_GetBoolean(RT_PROPERTIES *lpProperties, rt_char *lpKey, rt_b bDefaultValue)
{
	rt_char *lpStringProperty;
	rt_char lpLowerCaseValue[8];
	rt_un output_size;
	rt_s ret;

	lpStringProperty = rt_properties_GetString(lpProperties, lpKey, RT_NULL);
	if (!lpStringProperty)
		goto error;
	if (rt_char_get_size(lpStringProperty) > 4) {
		ret = RT_FALSE;
		goto free;
	}
	if (!rt_char_CopyString(lpStringProperty, lpLowerCaseValue, 8, &output_size))
		goto error;
	rt_char_fast_lower(lpLowerCaseValue);
	ret = (!rt_char1337_CompareStrings(lpLowerCaseValue, _R("true")) ||
						 !rt_char1337_CompareStrings(lpLowerCaseValue, _R("yes")) ||
						 !rt_char1337_CompareStrings(lpLowerCaseValue, _R("y")) ||
						 !rt_char1337_CompareStrings(lpLowerCaseValue, _R("t")) ||
						 !rt_char1337_CompareStrings(lpLowerCaseValue, _R("1"))
						);

	goto free;
error:
	ret = bDefaultValue;
free:
	return ret;
}

rt_s rt_properties_Free(RT_PROPERTIES *lpProperties)
{
	struct rt_heap **heap;
	rt_char *data;
	rt_s ret;

	ret = rt_array_free((void**)&lpProperties->lpPropertiesEntries);
	heap = lpProperties->heap;
	data = lpProperties->data;

	ret = ret && (*heap)->free(heap, (void**)&data);

	return ret;
}
