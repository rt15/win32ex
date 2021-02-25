#include <rpr.h>
#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>


rt_un16 ZzDisplayHelp(rt_un32 result)
{
	rt_console_WriteCString(_R("Replace all occurrences of a String by another.\nUsage:\nReplaceInFile searched replacement file\n"));
	return result;
}

rt_b ZzPerformWithHeap(rt_char *lpSearched, rt_char *replacement, rt_char *file_path, struct rt_heap **heap)
{
	rt_char8 *lpFileContent;
	rt_char *lpFileContentAsString;
	rt_n nFileContentAsStringSize;
	rt_char *lpNewFileContentAsString;
	rt_char8 *lpNewFileContent;
	rt_n nNewFileContentAsStringSize;
	rt_n nDelta;

	rt_un output_size;
	rt_un file_size;
	rt_un unNewFileSize;
	rt_n nOcurrencesCount;
	rt_s ret;

	lpFileContent = RT_NULL;
	lpFileContentAsString = RT_NULL;
	lpNewFileContentAsString = RT_NULL;
	lpNewFileContent = RT_NULL;

	/* Check file path. */
	if (!rt_file_system_CheckPath(file_path, RT_FILE_PATH_TYPE_FILE)) {
		rt_error_message_write_last(_R("Issue with input file: "));
		goto error;
	}

	/* Read input file content. */
	file_size = rt_small_file_read(file_path, &lpFileContent, heap);
	if (file_size == RT_TYPE_MAX_UN) {
		rt_error_message_write_last(_R("Failed to read input file: "));
		goto error;
	}

	/* Decode input file content. */
	nFileContentAsStringSize = rt_encoding_decode_with_heap(lpFileContent, file_size, 0, &lpFileContentAsString, heap);
	if (nFileContentAsStringSize == -1) {
		rt_error_message_write_last(_R("Failed to decode input file: "));
		goto error;
	}

	nOcurrencesCount = rt_char_count_occurrences(lpFileContentAsString, lpSearched);
	if (nOcurrencesCount > 0) {
		nDelta = (rt_char_GetCStringSize(replacement) - rt_char_GetCStringSize(lpSearched)) * nOcurrencesCount;
		nNewFileContentAsStringSize = nFileContentAsStringSize + nDelta;
		if (!(*heap)->alloc(heap, (void**)&lpNewFileContentAsString, (nNewFileContentAsStringSize + 1) * sizeof(rt_char), _R("New file content as string."))) {
			rt_error_message_write_last(_R("Failed to allocate result buffer: "));
			goto error;
		}
		if (!rt_char_replace(lpFileContentAsString, rt_char_GetCStringSize(lpFileContentAsString),
															lpSearched, rt_char_GetCStringSize(lpSearched),
															replacement, rt_char_GetCStringSize(replacement),
															lpNewFileContentAsString, nNewFileContentAsStringSize + 1, &output_size))
		{
			rt_error_message_write_last(_R("Replacement failed: "));
			goto error;
		}

		unNewFileSize = rt_encoding_encode_with_heap(lpNewFileContentAsString, output_size, 0, &lpNewFileContent, heap);
		if (unNewFileSize == -1) {
			rt_error_message_write_last(_R("Failed to encode output file: "));
			goto error;
		}

		rt_small_file_write(lpNewFileContent, unNewFileSize, file_path, RT_SMALL_FILE_MODE_TRUNCATE);
	}

	ret = RT_OK;
	goto free;
error:
	ret = RT_FAILED;
free:
	if (lpNewFileContent) {
		if (!(*heap)->free(heap, (void**)&lpNewFileContent)) {
			rt_error_message_write_last(_R("Failed free new file content: "));
			goto error;
		}
	}
	if (lpNewFileContentAsString) {
		if (!(*heap)->free(heap, (void**)&lpNewFileContentAsString)) {
			rt_error_message_write_last(_R("Failed free new file content as string: "));
			goto error;
		}
	}
	if (lpFileContentAsString) {
		if (!(*heap)->free(heap, (void**)&lpFileContentAsString)) {
			rt_error_message_write_last(_R("Failed free file content as string: "));
			goto error;
		}
	}
	if (lpFileContent) {
		if (!(*heap)->free(heap, (void**)&lpFileContent)) {
			rt_error_message_write_last(_R("Failed free file content: "));
			goto error;
		}
	}
	return ret;
}

rt_b ZzPerform(rt_char *lpSearched, rt_char *replacement, rt_char *file_path)
{
	struct rt_runtime_heap runtimeHeap;
	rt_b bCloseRuntimeHeap;
	rt_s ret;

	bCloseRuntimeHeap = RT_FALSE;

	if (!rt_runtime_heap_create(&runtimeHeap)) {
		rt_error_message_write_last(_R("Runtime heap creation failed: "));
		goto error;
	}
	bCloseRuntimeHeap = RT_TRUE;

	ret = ZzPerformWithHeap(lpSearched, replacement, file_path, &runtimeHeap.heap);

	goto free;
error:
	ret = RT_FAILED;
free:
	if (bCloseRuntimeHeap) {
		if (!runtimeHeap.heap->close(&runtimeHeap)) {
			rt_error_message_write_last(_R("Failed to close runtime heap: "));
			bCloseRuntimeHeap = RT_FALSE;
			goto error;
		}
		bCloseRuntimeHeap = RT_FALSE;
	}
	return ret;
}

rt_un16 RT_CALL RtMain(rt_n32 argc, rt_char *argv[])
{
	RT_ARRAY zzFirstArgument;
	rt_un32 result;

	if (argc == 4) {
		result = !ZzPerform(argv[1], argv[2], argv[3]);
	} else if (argc == 2) {
		rt_char_CreateString(&zzFirstArgument, argv[1]);
		if (rt_char_StringEqualsCString(&zzFirstArgument, _R("/?")) ||
				rt_char_StringEqualsCString(&zzFirstArgument, _R("-h")) ||
				rt_char_StringEqualsCString(&zzFirstArgument, _R("--help")))
		{
			result = 0;
		} else {
			result = 1;
		}
		result = ZzDisplayHelp(result);
	} else {
		result = ZzDisplayHelp(1);
	}

	return result;
}
