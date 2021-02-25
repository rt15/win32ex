#define RT_DEFINE_USE_CRT

#include <rpr.h>
#include <RtWin32ExMain.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libexif/exif-data.h>

#define ORIGINAL_DATE_SIZE 64

typedef struct _FILE_INFO {
	rt_char lpFileName[RT_FILE_PATH_NAME_SIZE];
	rt_char lpOriginalDate[ORIGINAL_DATE_SIZE];
}
FILE_INFO;

rt_un16 DisplayHelp(rt_un32 result)
{
	rt_console_WriteCString(_R("Rename jpg files by date.\nUsage:\nPicturesNamer directory\n"));
	return result;
}

/**
 * buffer size should be at least ORIGINAL_DATE_SIZE.
 */
rt_n RetrieveOriginalDate(rt_char *file, rt_char *buffer)
{
	rt_char8 pathBuffer[RT_FILE_PATH_SIZE];
	rt_char8 lpOriginalDateBuffer[ORIGINAL_DATE_SIZE];
	ExifData *exifData;
	ExifEntry *exifEntry;
	rt_n ret;

	ret = 0;

	if (rt_encoding_encode_with_buffer(file, -1, 0, pathBuffer, RT_FILE_PATH_SIZE) == -1) {
		rt_error_message_write_last(_R("Failed to convert to multibytes: "));
		goto the_end;
	}

	exifData = exif_data_new_from_file(pathBuffer);
	if (exifData) {
		exifEntry = exif_data_get_entry(exifData, EXIF_TAG_DATE_TIME_ORIGINAL);
		if (exifEntry) {
			strncpy(lpOriginalDateBuffer, (rt_char8*)exifEntry->data, ORIGINAL_DATE_SIZE);
		} else {
			/* TODO: Write error message and file path. */
		}
		exif_data_free(exifData);

		ret = rt_encoding_decode_with_buffer(lpOriginalDateBuffer, -1, 0, buffer, ORIGINAL_DATE_SIZE);
	} else {
		/* TODO: Write error message and file path. */
	}

the_end:
	return ret;
}

rt_s RT_CALL BrowseProc(rt_char *path, rt_un type, void *context)
{
	RT_ARRAY zzExtension;
	rt_char lpExtension[5];
	rt_un path_size;
	rt_un output_size;
	FILE_INFO *lpFileInfo;
	rt_s ret;

	if (type == RT_FILE_PATH_TYPE_FILE) {
		path_size = rt_char_GetCStringSize(path);
		if (path_size > 4) {
			/* Extract extension. */
			rt_char_CopyString(&path[path_size - 4], lpExtension, 5, &output_size);
			rt_char_fast_lower(lpExtension);
			if (rt_char_StringEqualsCString(rt_char_CreateString(&zzExtension, lpExtension), _R(".jpg"))) {
				if (!rt_array_new_item((void**)context, (void**)&lpFileInfo)) {
					rt_error_message_write_last(_R("New item failed: "));
					ret = RT_FAILED;
					goto the_end;
				}

				/* Fill file info. */
				rt_file_path_get_name(path, path_size, lpFileInfo->lpFileName, RT_FILE_PATH_NAME_SIZE, &output_size);
				RetrieveOriginalDate(path, lpFileInfo->lpOriginalDate);
			}
		}
	}
	ret = RT_OK;
the_end:
	return ret;
}

rt_s RT_CALL CompareFileInfos(void *item1, void *item2, void *context, rt_n *comparison_result)
{
	FILE_INFO *lpFileInfo1;
	FILE_INFO *lpFileInfo2;
	RT_ARRAY zzOriginalDate1;
	RT_ARRAY zzOriginalDate2;

	lpFileInfo1 = (FILE_INFO*)item1;
	lpFileInfo2 = (FILE_INFO*)item2;

	rt_char_CreateString(&zzOriginalDate1, lpFileInfo1->lpOriginalDate);
	rt_char_CreateString(&zzOriginalDate2, lpFileInfo2->lpOriginalDate);

	*comparison_result = rt_char_CompareStrings(&zzOriginalDate1, &zzOriginalDate2);
	return RT_TRUE;
}

rt_s RT_CALL ZzConvertUnsignedIntegerToString(rt_un unInput, rt_char *buffer)
{
	RT_ARRAY rtConversionBuffer;
	rt_s ret;

	rt_array_Create(&rtConversionBuffer, buffer, sizeof(rt_char), 64);

	if (rt_char_append_un(unInput, &rtConversionBuffer)) goto error;

	ret = RT_OK;
free:
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

rt_un16 Perform(rt_char *path)
{
	rt_char destination_file_name[20];
	rt_char message[RT_FILE_PATH_SIZE + 200];
	rt_char destination_file_path[RT_FILE_PATH_SIZE];
	rt_char lpOldFilePath[RT_FILE_PATH_SIZE];
	rt_char lpConversionBuffer[64];
	rt_un written;
	rt_un output_size;
	FILE_INFO *lpFileInfos;
	struct rt_runtime_heap runtimeHeap;
	rt_un32 unArraySize;
	rt_n i;
	rt_un32 result;

	result = 1;

	if (!rt_file_system_CheckPath(path, RT_FILE_PATH_TYPE_DIR)) {
		rt_error_message_WriteLastVariadic(RT_NULL, path, _R(" is not a directory: "), (rt_char*)RT_NULL);
		goto the_end;
	}

	if (!rt_runtime_heap_create(&runtimeHeap)) {
		rt_error_message_write_last(_R("Runtime heap creation failed: "));
		goto the_end;
	}

	if (!rt_sortable_array_create((void**)&lpFileInfos, &runtimeHeap.heap, 0, sizeof(FILE_INFO), &CompareFileInfos, RT_NULL)) {
		rt_error_message_write_last(_R("Array creation failed: "));
		goto close_heap;
	}

	if (!rt_file_path_browse(path, &BrowseProc, RT_FALSE, RT_FALSE, &lpFileInfos)) {
		result = 1;
		goto close_array;
	}

	unArraySize = rt_array_get_size(lpFileInfos);

	ZzConvertUnsignedIntegerToString(unArraySize, lpConversionBuffer);

	written = 0;
	rt_char_CopyString(_R("Files count: "),				 &message[written], RT_FILE_PATH_SIZE + 200 - written, &output_size); written += output_size;
	rt_char_CopyString(lpConversionBuffer,					&message[written], RT_FILE_PATH_SIZE + 200 - written, &output_size); written += output_size;
	rt_char_CopyStringWithSize(_R("\n"), 1,				 &message[written], RT_FILE_PATH_SIZE + 200 - written, &output_size); written += output_size;
	rt_console_write_string_with_size(message, written);

	rt_sortable_array_sort(lpFileInfos);

	for (i = 0; i < unArraySize; i++) {
		/* Compute the new file name. */
		ZzConvertUnsignedIntegerToString((i + 1) * 5, destination_file_name);

		written = 0;
		rt_char_left_pad(destination_file_name, rt_char_GetCStringSize(destination_file_name), _R('0'), 5, &destination_file_name[written], 20 - written, &output_size); written += output_size;
		rt_char_CopyString(_R(".jpg"),																												 &destination_file_name[written], 20 - written, &output_size); written += output_size;

		/* Display a log message. */
		written = 0;
		rt_char_CopyString(lpFileInfos[i].lpFileName,		 &message[written], 200 - written, &output_size); written += output_size;
		rt_char_CopyString(_R(" => "),										 &message[written], 200 - written, &output_size); written += output_size;
		rt_char_CopyString(lpFileInfos[i].lpOriginalDate, &message[written], 200 - written, &output_size); written += output_size;
		rt_char_CopyString(_R(" => "),										 &message[written], 200 - written, &output_size); written += output_size;
		rt_char_CopyString(destination_file_name,									&message[written], 200 - written, &output_size); written += output_size;
		rt_char_CopyString(_R("\n"),											 &message[written], 200 - written, &output_size); written += output_size;
		rt_console_write_string_with_size(message, written);

		/* Compute old file path. */
		written = 0;
		rt_char_CopyString(path,													&lpOldFilePath[written], RT_FILE_PATH_SIZE - written, &output_size); written += output_size;
		rt_char_CopyString(RT_FILE_PATH_SEPARATOR_STRING, &lpOldFilePath[written], RT_FILE_PATH_SIZE - written, &output_size); written += output_size;
		rt_char_CopyString(lpFileInfos[i].lpFileName,			&lpOldFilePath[written], RT_FILE_PATH_SIZE - written, &output_size); written += output_size;

		/* Compute new file path. */
		written = 0;
		rt_char_CopyString(path,													&destination_file_path[written], RT_FILE_PATH_SIZE - written, &output_size); written += output_size;
		rt_char_CopyString(RT_FILE_PATH_SEPARATOR_STRING, &destination_file_path[written], RT_FILE_PATH_SIZE - written, &output_size); written += output_size;
		rt_char_CopyString(destination_file_name,									 &destination_file_path[written], RT_FILE_PATH_SIZE - written, &output_size); written += output_size;

		if (!rt_file_system_move_file(lpOldFilePath, destination_file_path)) {
			rt_error_message_write_last(_R("Failed to rename file: "));
			goto close_array;
		}
	}
	result = 0;

close_array:
	if (!rt_array_free((void**)&lpFileInfos)) {
		rt_error_message_write_last(_R("Failed to close array: "));
		result = 1;
	}
close_heap:
	if (!runtimeHeap.heap->close(&runtimeHeap)) {
		rt_error_message_write_last(_R("Failed to close runtime heap: "));
		result = 1;
	}

the_end:
	return result;
}

rt_un16 RT_CALL RtMain(rt_n32 argc, rt_char *argv[])
{
	RT_ARRAY zzFirstArgument;
	rt_un32 result;

	if (argc != 2) {
		result = DisplayHelp(1);
		goto the_end;
	} else {
		rt_char_CreateString(&zzFirstArgument, argv[1]);
		if (rt_char_StringEqualsCString(&zzFirstArgument, _R("/?")) ||
				rt_char_StringEqualsCString(&zzFirstArgument, _R("-h")) ||
				rt_char_StringEqualsCString(&zzFirstArgument, _R("--help")))
		{
			result = DisplayHelp(0);
			goto the_end;
		}
	}

	result = Perform(argv[1]);

the_end:
	return result;
}

