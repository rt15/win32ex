#ifndef ZZ_APP_CONTEXT_H
#define ZZ_APP_CONTEXT_H

#include <RtWin32Ex.h>
#include <RtWin32ExGui.h>

#include "ZzResources.h"

typedef struct _ZZ_APP_CONTEXT
{
  RT_H hInstance;
  RT_UN32 unWindowExitCode;

  RT_N nVerticalSplitterX;                  /* Position of the vertical splitter. */
  RT_N nSplitterCursorOffset;               /* Used when the a splitter is in use to store the offset where the user clicked. */
  RT_B bUsingVerticalSplitter;              /* Track usage of vertical splitter. */

  RT_H hStandardCursor;
  RT_H hWestEastCursor;

  RT_H hFont;

  RT_H hMainWindow;
  RT_H hStatusBar;
  RT_H hLeftTab;
  RT_H hVerticalSplitter;
  RT_H hListBox;
  RT_H hAddButton;
  RT_H hDeleteButton;
}
ZZ_APP_CONTEXT;

RT_B RT_CALL ZzCreateAppContext(ZZ_APP_CONTEXT* lpAppContext);

RT_B RT_CALL ZzFreeAppContext(ZZ_APP_CONTEXT* lpAppContext);

#endif /* ZZ_APP_CONTEXT_H */
