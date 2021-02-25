#include "ZzReplaceInFile.h"

#include "ZzLuaUtils.h"

#include <lauxlib.h>

rt_b ZzPerformWithHeap(rt_char *lpSearched, rt_char *replacement, rt_char *file_path, struct rt_heap **heap)
{
	rt_char8 *lpFileContent8;
	rt_char *lpFileContent;
	rt_n nFileContentSize;
	rt_char *lpNewFileContent;
	rt_char8 *lpNewFileContent8;
	rt_n nNewFileContentSize;
	rt_n nDelta;

	rt_un output_size;
	rt_un file_size;
	rt_un unNewFileSize;
	rt_n nOcurrencesCount;
	rt_s ret;

	lpFileContent8 = RT_NULL;
	lpFileContent = RT_NULL;
	lpNewFileContent = RT_NULL;
	lpNewFileContent8 = RT_NULL;

	/* Check file path. */
	if (!rt_file_system_CheckPath(file_path, RT_FILE_PATH_TYPE_FILE)) goto error;

	/* Read input file content. */
	file_size = rt_small_file_read(file_path, &lpFileContent8, heap);
	if (file_size == RT_TYPE_MAX_UN) goto error;

	/* Decode input file content. */
	nFileContentSize = rt_encoding_decode_with_heap(lpFileContent8, file_size, 0, &lpFileContent, heap);
	if (nFileContentSize == -1) goto error;

	nOcurrencesCount = rt_char_count_occurrences(lpFileContent, lpSearched);
	if (nOcurrencesCount > 0) {
		nDelta = (rt_char_GetCStringSize(replacement) - rt_char_GetCStringSize(lpSearched)) * nOcurrencesCount;
		nNewFileContentSize = nFileContentSize + nDelta;
		if (!(*heap)->alloc(heap, (void**)&lpNewFileContent, (nNewFileContentSize + 1) * sizeof(rt_char), _R("New file content as string."))) goto error;

		if (!rt_char_replace(lpFileContent, rt_char_GetCStringSize(lpFileContent),
															lpSearched, rt_char_GetCStringSize(lpSearched),
															replacement, rt_char_GetCStringSize(replacement),
															lpNewFileContent, nNewFileContentSize + 1, &output_size)) goto error;

		unNewFileSize = rt_encoding_encode_with_heap(lpNewFileContent, output_size, 0, &lpNewFileContent8, heap);
		if (unNewFileSize == -1) goto error;

		if (!rt_small_file_write(lpNewFileContent8, unNewFileSize, file_path, RT_SMALL_FILE_MODE_TRUNCATE)) goto error;
	}

	ret = RT_OK;
free:
	if (lpNewFileContent8) {
		if (!(*heap)->free(heap, (void**)&lpNewFileContent8)) goto error;
	}
	if (lpNewFileContent) {
		if (!(*heap)->free(heap, (void**)&lpNewFileContent)) goto error;
	}
	if (lpFileContent) {
		if (!(*heap)->free(heap, (void**)&lpFileContent)) goto error;
	}
	if (lpFileContent8) {
		if (!(*heap)->free(heap, (void**)&lpFileContent8)) goto error;
	}
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

rt_b ZzPerform(rt_char *lpSearched, rt_char *replacement, rt_char *file_path)
{
	struct rt_runtime_heap runtimeHeap;
	rt_b bCloseRuntimeHeap;
	rt_s ret;

	bCloseRuntimeHeap = RT_FALSE;

	if (!rt_runtime_heap_create(&runtimeHeap)) goto error;
	bCloseRuntimeHeap = RT_TRUE;

	ret = ZzPerformWithHeap(lpSearched, replacement, file_path, &runtimeHeap.heap);

	goto free;
error:
	ret = RT_FAILED;
free:
	if (bCloseRuntimeHeap) {
		if (!runtimeHeap.heap->close(&runtimeHeap)) {
			bCloseRuntimeHeap = RT_FALSE;
			goto error;
		}
		bCloseRuntimeHeap = RT_FALSE;
	}
	return ret;
}

RT_EXPORT rt_n32 RT_CDECL ZzReplaceInFile(lua_State *lpLuaState)
{
	rt_char8 *lpSearched8;
	rt_char8 *lpReplacement8;
	rt_char8 *lpFilePath8;

	rt_char lpSearched[RT_CHAR_QUARTER_BIG_STRING_SIZE];
	rt_char replacement[RT_CHAR_QUARTER_BIG_STRING_SIZE];
	rt_char file_path[RT_FILE_PATH_SIZE];

	rt_un written;
	rt_n32 ret;

	ret = 0;

	/* lpSearched. */
	lpSearched8 = (rt_char8*)luaL_checkstring(lpLuaState, 1);
	if (!lpSearched8) {
		rt_error_set_last(RT_ERROR_BAD_ARGUMENTS);
		goto error;
	}
	written = rt_encoding_decode_with_buffer(lpSearched8, -1, 0, lpSearched, RT_CHAR_QUARTER_BIG_STRING_SIZE);
	if (written == -1) goto error;

	/* replacement. */
	lpReplacement8 = (rt_char8*)luaL_checkstring(lpLuaState, 2);
	if (!lpReplacement8) {
		rt_error_set_last(RT_ERROR_BAD_ARGUMENTS);
		goto error;
	}
	written = rt_encoding_decode_with_buffer(lpReplacement8, -1, 0, replacement, RT_CHAR_QUARTER_BIG_STRING_SIZE);
	if (written == -1) goto error;

	/* file_path. */
	lpFilePath8 = (rt_char8*)luaL_checkstring(lpLuaState, 3);
	if (!lpFilePath8) {
		rt_error_set_last(RT_ERROR_BAD_ARGUMENTS);
		goto error;
	}
	written = rt_encoding_decode_with_buffer(lpFilePath8, -1, 0, file_path, RT_FILE_PATH_SIZE);
	if (written == -1) goto error;

	if (!ZzPerform(lpSearched, replacement, file_path)) goto error;

free:
	return ret;
error:
	ZzPushLastErrorMessage(lpLuaState);
	ret = 1;
	goto free;
}
