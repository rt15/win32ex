#include "cbmenu.h"

RT_B RT_CALL CbManageMenu(CB_MENU_CALLBACK lpHeaderCallback, CB_MENU_ITEM* lpMenuItems, RT_UN unItemCounts, void* lpContext)
{
  RT_ARRAY rtConversionBuffer;
  RT_CHAR lpConversionBuffer[64];
  RT_ARRAY rtMessage;
  RT_CHAR lpMessage[256];
  RT_CHAR nRead;
  RT_N nI;
  RT_B bResult;

  while (RT_TRUE)
  {
    if (!lpHeaderCallback(lpContext)) goto handle_error;

    RtArray_Create(&rtMessage, lpMessage, sizeof(RT_CHAR), 256);
    RtArray_Create(&rtConversionBuffer, lpConversionBuffer, sizeof(RT_CHAR), 64);

    for (nI = 0; nI < unItemCounts; nI++)
    {
      if (!RtChar_ConvertIntegerToString(nI + 1, &rtConversionBuffer)) goto handle_error;

      if (!RtChar_AppendCString(&rtMessage, lpMenuItems[nI].lpTitle)) goto handle_error;
      if (!RtChar_AppendCString(&rtMessage, _R(": "))) goto handle_error;
      if (!RtArray_Append(&rtMessage, &rtConversionBuffer)) goto handle_error;
      if (!RtChar_AppendCString(&rtMessage, _R("\n"))) goto handle_error;
    }
    if (!RtChar_AppendCString(&rtMessage, _R("\nExit: 0\n"))) goto handle_error;
    RtConsole_WriteString(&rtMessage);

    nRead = RtConsole_ReadChar();
    if (nRead == _R('0'))
    {
      break;
    }
    else if (nRead >= _R('1') && nRead <= _R('9'))
    {
      nRead = nRead - _R('0');
      if (nRead <= unItemCounts)
      {
        if (!lpMenuItems[nRead - 1].lpMenuCallback(lpContext)) goto handle_error;
      }
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
