#include <rpr.h>
#include <RtWin32ExMain.h>
#include <RTWin32ExMem.h>

/*
select XDBM_LOG_SCRIPT_NAME, XDBM_LOG_MODEL, XDBM_LOG_SCRIPT_VERSION from ITEM_MAIN21.XDBM_LOG
select XDBM_LOG_SCRIPT_NAME, XDBM_LOG_MODEL, XDBM_LOG_SCRIPT_VERSION from ITEM_CACHE21.XDBM_LOG
select XDBM_LOG_SCRIPT_NAME, XDBM_LOG_MODEL, XDBM_LOG_SCRIPT_VERSION from ITEM_VIEW21.XDBM_LOG
*/

#define XS_SCHEMAS_COUNT 3
rt_char *xs_lpSchemas[XS_SCHEMAS_COUNT] = { _R("Main"), _R("Cache"), _R("View") };

#define XS_TAGS_COUNT 2
rt_char *xs_lpTagsNames[XS_TAGS_COUNT] = { _R("@model"), _R("@version") };

#define XS_BUFFER_SIZE 200

typedef struct _XS_CONTEXT {
	struct rt_heap **heap;
}
XS_CONTEXT;

rt_s RT_CALL XsManageTag(rt_char *lpFileContent, rt_char *lpTagName)
{
	rt_char value[XS_BUFFER_SIZE];
	rt_n nBegin;
	rt_n nEnd;
	rt_n i;
	rt_n j;
	rt_s ret;

	nBegin = rt_char_search(lpFileContent, lpTagName);
	if (nBegin == -1) {
		rt_console_WriteCString(_R("Tag not found"));
		ret = RT_FAILED;
		goto the_end;
	}
	nBegin += rt_char_GetCStringSize(lpTagName);
	while (lpFileContent[nBegin] != _R(':')) {
		nBegin++;
	}
	nBegin++;
	while (lpFileContent[nBegin] <= _R(' ')) {
		nBegin++;
	}
	nEnd = nBegin;
	while (lpFileContent[nEnd] > _R(' ')) {
		nEnd++;
	}

	j = 0;
	for (i = nBegin; i < nEnd; i++) {
		value[j] = lpFileContent[i];
		j++;
	}
	value[j] = 0;

	rt_console_WriteStringsOrErrors(RT_TRUE, _R("\t"), value, (rt_char*)RT_NULL);

	ret = RT_OK;

the_end:
	return ret;
}

rt_s RT_CALL XsBrowseCallback(rt_char *path, rt_un type, void *context)
{
	XS_CONTEXT *lpXsContext;
	struct rt_heap **heap;
	rt_char lpLowerCasePath[RT_FILE_PATH_SIZE];
	rt_char lpFileName[RT_FILE_PATH_NAME_SIZE];
	rt_char8 *data;
	rt_char *lpFileContent;
	rt_n nFileContentSize;
	rt_n nDataSize;
	rt_un output_size;
	rt_n i;
	rt_s ret;

	ret = RT_OK;

	if (type == RT_FILE_PATH_TYPE_FILE) {
		rt_char_CopyString(path, lpLowerCasePath, RT_FILE_PATH_SIZE, &output_size);
		rt_char_fast_lower(lpLowerCasePath);
		if (rt_char_ends_with(lpLowerCasePath, output_size, _R(".sql"), 4)) {
			rt_file_path_get_name(path, rt_char_GetCStringSize(path), lpFileName, RT_FILE_PATH_NAME_SIZE, &output_size);
			/* Write file name without extension. */
			rt_console_write_string_with_size(lpFileName, output_size - 4);

			lpXsContext = (XS_CONTEXT*)context;
			heap = lpXsContext->heap;
			nDataSize = rt_small_file_read(path, &data, heap);
			if (nDataSize == -1) {
				rt_error_message_write_last(_R("Failed to read file: "));
				ret = RT_FAILED;
			}

			nFileContentSize = rt_encoding_decode_with_heap(data, nDataSize, RT_ENCODING_UTF_8, &lpFileContent, heap);
			if (nFileContentSize == -1) {
				rt_error_message_write_last(_R("Failed to decode file content: "));
				ret = RT_FAILED;
			} else {
				for (i = 0; i < XS_TAGS_COUNT; i++) {
					if (!XsManageTag(lpFileContent, xs_lpTagsNames[i])) {
						ret = RT_FAILED;
						break;
					}
				}

				if (!(*heap)->free(heap, (void**)&lpFileContent)) {
					rt_error_message_write_last(_R("Failed to free decoded file content: "));
					ret = RT_FAILED;
				}
			}

			if (!(*heap)->free(heap, (void**)&data)) {
				rt_error_message_write_last(_R("Failed to free file content: "));
				ret = RT_FAILED;
			}

			rt_console_write_string_with_size(_R("\n"), 1);
		}
	}

	return ret;
}

rt_un16 RT_CALL RtMain(rt_n32 argc, rt_char *argv[])
{
	struct rt_runtime_heap runtimeHeap;
	rt_un written;
	rt_un output_size;
	rt_char *lpSchema;
	rt_char path[XS_BUFFER_SIZE];
	XS_CONTEXT context;
	rt_n i;
	rt_un32 result;

	if (!rt_runtime_heap_create(&runtimeHeap)) {
		result = rt_error_message_write_last(_R("Runtime heap creation failed: "));
		goto the_end;
	}

	context.heap = &runtimeHeap.heap;

	for (i = 0; i < XS_SCHEMAS_COUNT; i++) {
		lpSchema = xs_lpSchemas[i];

		written = 0;
		rt_char_CopyString(_R("database/"),								&path[written], XS_BUFFER_SIZE - written, &output_size); written += output_size;
		rt_char_CopyString(lpSchema,											 &path[written], XS_BUFFER_SIZE - written, &output_size); written += output_size;
		rt_char_CopyString(_R("/database/oracle/scripts"), &path[written], XS_BUFFER_SIZE - written, &output_size); written += output_size;

		if (!rt_file_path_browse(path, &XsBrowseCallback, RT_TRUE, RT_FALSE, &context)) {
			result = 1;
			goto close_heap;
		}
		rt_console_write_string_with_size(_R("\n"), 1);
	}

	result = 0;
close_heap:
	if (!runtimeHeap.heap->close(&runtimeHeap)) {
		result = rt_error_message_write_last(_R("Failed to close runtime heap: "));
	}
the_end:
	return result;
}
