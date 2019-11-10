#include "cbmenu.h"

RT_UN16 RT_CALL CbManageMenu(CB_MENU_CALLBACK lpHeaderCallback, CB_MENU_ITEM* lpMenuItems, RT_UN unItemCounts, void* lpContext)
{
  RT_CHAR nRead;
  RT_UN unOutputSize;
  RT_N nI;

  RT_CHAR lpBuffer[16];

  RT_UN32 unResult;

  while (RT_TRUE)
  {
    unResult = lpHeaderCallback(lpContext);
    if (unResult)
    {
      break;
    }
    for (nI = 0; nI < unItemCounts; nI++)
    {
      RtChar_ConvertIntegerToString(nI + 1, lpBuffer, 16, &unOutputSize);
      RtConsole_WriteStringsOrErrors(RT_TRUE, lpMenuItems[nI].lpTitle, _R(": "), lpBuffer, _R("\n"), (RT_CHAR*)RT_NULL);
    }
    RtConsole_WriteString(_R("\nExit: 0\n"));

    nRead = RtConsole_ReadChar();
    if (nRead == _R('0'))
    {
      unResult = 0;
      break;
    }
    else if (nRead >= _R('1') && nRead <= _R('9'))
    {
      nRead = nRead - _R('0');
      if (nRead <= unItemCounts)
      {
        unResult = lpMenuItems[nRead - 1].lpMenuCallback(lpContext);
        if (unResult)
        {
          break;
        }
      }
    }
  }
  return unResult;
}
