#define RT_DEFINE_USE_CRT

#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libexif/exif-data.h>

#define ORIGINAL_DATE_SIZE 64

typedef struct _FILE_INFO
{
  RT_CHAR lpFileName[RT_FILE_SYSTEM_MAX_FILE_NAME];
  RT_CHAR lpOriginalDate[ORIGINAL_DATE_SIZE];
}
FILE_INFO;

RT_UN16 WriteLastErrorMessage(RT_CHAR* lpLabel)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unWritten;

  unWritten = 0;
  RtCopyString(lpLabel, lpBuffer, RT_CHAR_BIG_STRING_SIZE, &unWritten);
  RtGetLastErrorMessage(&lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten);
  RtCopyStringWithSize(_R("\n"), 1, &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten);

  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  return 1;
}

RT_UN16 DisplayHelp(RT_UN32 unResult)
{
  RtWriteStringToConsole(_R("Rename jpg files by date.\nUsage:\nPicturesNamer directory\n"));
  return unResult;
}

/**
 * lpBuffer size should be at least ORIGINAL_DATE_SIZE.
 */
RT_N RetrieveOriginalDate(RT_CHAR* lpFile, RT_CHAR* lpBuffer)
{
  RT_CHAR8 lpPathBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR8 lpOriginalDateBuffer[ORIGINAL_DATE_SIZE];
  ExifData *exifData;
  ExifEntry *exifEntry;
  RT_N nResult;

  nResult = 0;

  if (RtEncodeWithBuffer(lpFile, -1, 0, lpPathBuffer, RT_FILE_SYSTEM_MAX_FILE_PATH) == -1)
  {
    WriteLastErrorMessage(_R("Failed to convert to multibytes: "));
    goto the_end;
  }

  exifData = exif_data_new_from_file(lpPathBuffer);
  if (exifData)
  {
    exifEntry = exif_data_get_entry(exifData, EXIF_TAG_DATE_TIME_ORIGINAL);
    if (exifEntry)
    {
      strncpy(lpOriginalDateBuffer, (RT_CHAR8*)exifEntry->data, ORIGINAL_DATE_SIZE);
    }
    else
    {
      /* TODO: Write error message and file path. */
    }
    exif_data_free(exifData);

    nResult = RtDecodeWithBuffer(lpOriginalDateBuffer, -1, 0, lpBuffer, ORIGINAL_DATE_SIZE);
  }
  else
  {
    /* TODO: Write error message and file path. */
  }

the_end:
  return nResult;
}

RT_B RT_CALL BrowseProc(RT_CHAR* lpPath, RT_UN unType, void* lpContext)
{
  RT_CHAR lpExtension[5];
  RT_UN unPathSize;
  RT_UN unWritten;
  FILE_INFO* lpFileInfo;
  RT_B bResult;

  if (unType == RT_FILE_SYSTEM_TYPE_FILE)
  {
    unPathSize = RtGetStringSize(lpPath);
    if (unPathSize > 4)
    {
      /* Extract extension. */
      RtCopyString(&lpPath[unPathSize - 4], lpExtension, 5, &unWritten);
      RtFastLowerString(lpExtension);
      if (!RtCompareStrings(_R(".jpg"), lpExtension))
      {
        if (!RtNewArrayItem((void**)lpContext, (void**)&lpFileInfo))
        {
          WriteLastErrorMessage(_R("New item failed: "));
          bResult = RT_FAILURE;
          goto the_end;
        }

        /* Fill file info. */
        RtExtractFileName(lpPath, unPathSize, lpFileInfo->lpFileName, RT_FILE_SYSTEM_MAX_FILE_NAME, &unWritten);
        RetrieveOriginalDate(lpPath, lpFileInfo->lpOriginalDate);
      }
    }
  }
  bResult = RT_SUCCESS;
the_end:
  return bResult;
}

RT_B RT_CALL CompareFileInfos(void* lpItem1, void* lpItem2, void* lpContext, RT_N* lpComparisonResult)
{
  FILE_INFO* lpFileInfo1;
  FILE_INFO* lpFileInfo2;

  lpFileInfo1 = (FILE_INFO*)lpItem1;
  lpFileInfo2 = (FILE_INFO*)lpItem2;

  *lpComparisonResult = RtCompareStrings(lpFileInfo1->lpOriginalDate, lpFileInfo2->lpOriginalDate);
  return RT_TRUE;
}

RT_UN16 Perform(RT_CHAR* lpPath)
{
  RT_CHAR lpNewFileName[20];
  RT_CHAR lpMessage[RT_FILE_SYSTEM_MAX_FILE_PATH + 200];
  RT_CHAR lpNewFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpOldFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  FILE_INFO* lpFileInfos;
  RtRuntimeHeap runtimeHeap;
  RT_UN32 unArraySize;
  RT_N nI;
  RT_UN32 unResult;

  unResult = 1;

  if (!RtCheckPath(lpPath, RT_FILE_SYSTEM_TYPE_DIRECTORY))
  {
    unWritten = 0;
    RtCopyString(lpPath, lpMessage, RT_FILE_SYSTEM_MAX_FILE_PATH + 200, &unWritten);
    RtCopyString(_R(" is not a directory: "), &lpMessage[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH + 200 - unWritten, &unWritten);
    unResult = WriteLastErrorMessage(lpMessage);
    goto the_end;
  }

  if (!RtRuntimeHeapCreate(&runtimeHeap))
  {
    unResult = WriteLastErrorMessage(_R("Runtime heap creation failed: "));
    goto the_end;
  }

  if (!RtCreateSortableArray((void**)&lpFileInfos, &runtimeHeap.lpHeap, 0, sizeof(FILE_INFO), &CompareFileInfos, RT_NULL))
  {
    unResult = WriteLastErrorMessage(_R("Array creation failed: "));
    goto close_heap;
  }

  if (!RtBrowsePath(lpPath, &BrowseProc, RT_FALSE, &lpFileInfos))
  {
    unResult = 1;
    goto close_array;
  }

  unArraySize = RtGetArraySize(lpFileInfos);

  unWritten = 0;
  RtCopyString(_R("Files count: "), lpMessage, RT_FILE_SYSTEM_MAX_FILE_PATH + 200, &unWritten);
  RtConvertIntegerToString(unArraySize, &lpMessage[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH + 200 - unWritten, &unWritten);
  RtCopyStringWithSize(_R("\n"), 1, &lpMessage[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH + 200 - unWritten, &unWritten);
  RtWriteStringToConsoleWithSize(lpMessage, unWritten);

  RtSortSortableArray(lpFileInfos);

  for (nI = 0; nI < unArraySize; nI++)
  {
    /* Compute the new file name. */
    unWritten = 0;
    RtConvertIntegerToString((nI + 1) * 5, lpNewFileName, 20, &unWritten);
    RtLeftPadStringWithSize(lpNewFileName, unWritten, _R('0'), 5, lpNewFileName, 20, &unWritten);
    RtCopyString(_R(".jpg"), &lpNewFileName[unWritten], 20 - unWritten, &unWritten);

    /* Display a log message. */
    unWritten = 0;
    RtCopyString(lpFileInfos[nI].lpFileName,      &lpMessage[unWritten], 200 - unWritten, &unWritten);
    RtCopyString(_R(" => "),                      &lpMessage[unWritten], 200 - unWritten, &unWritten);
    RtCopyString(lpFileInfos[nI].lpOriginalDate,  &lpMessage[unWritten], 200 - unWritten, &unWritten);
    RtCopyString(_R(" => "),                      &lpMessage[unWritten], 200 - unWritten, &unWritten);
    RtCopyString(lpNewFileName,                   &lpMessage[unWritten], 200 - unWritten, &unWritten);
    RtCopyString(_R("\n"),                        &lpMessage[unWritten], 200 - unWritten, &unWritten);
    RtWriteStringToConsoleWithSize(lpMessage, unWritten);

    /* Compute old file path. */
    unWritten = 0;
    RtCopyString(lpPath,                          &lpOldFilePath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);
    RtCopyString(RT_FILE_SYSTEM_SEPARATOR_STRING, &lpOldFilePath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);
    RtCopyString(lpFileInfos[nI].lpFileName,      &lpOldFilePath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);

    /* Compute new file path. */
    unWritten = 0;
    RtCopyString(lpPath,                          &lpNewFilePath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);
    RtCopyString(RT_FILE_SYSTEM_SEPARATOR_STRING, &lpNewFilePath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);
    RtCopyString(lpNewFileName,                   &lpNewFilePath[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten);

    if (!RtMoveFile(lpOldFilePath, lpNewFilePath))
    {
      WriteLastErrorMessage(_R("Failed to rename file: "));
      unResult = 1;
      goto close_array;
    }
  }
  unResult = 0;

close_array:
  if (!RtFreeArray((void**)&lpFileInfos))
  {
    WriteLastErrorMessage(_R("Failed to close array: "));
    unResult = 1;
  }
close_heap:
  if (!runtimeHeap.lpHeap->lpClose(&runtimeHeap))
  {
    WriteLastErrorMessage(_R("Failed to close runtime heap: "));
    unResult = 1;
  }

the_end:
  return unResult;
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_UN32 unResult;

  if (nArgC != 2)
  {
    unResult = DisplayHelp(1);
    goto the_end;
  }
  else
  {
    if ((!RtCompareStrings(_R("/?"), lpArgV[1])) ||
        (!RtCompareStrings(_R("-h"), lpArgV[1])) ||
        (!RtCompareStrings(_R("--help"), lpArgV[1])))
    {
      unResult = DisplayHelp(0);
      goto the_end;
    }
  }

  unResult = Perform(lpArgV[1]);

the_end:
  return unResult;
}

