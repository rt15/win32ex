#ifndef CB_MENU_H
#define CB_MENU_H

#include <RtWin32Ex.h>

typedef RT_UN16 (RT_CALL* CB_MENU_CALLBACK)(void* lpContext);

typedef struct _CB_MENU_ITEM
{
  RT_CHAR* lpTitle;
  CB_MENU_CALLBACK lpMenuCallback;
}
CB_MENU_ITEM;

/**
 * @param nItemCounts Must be strictly less than 10.
 */
RT_UN16 RT_CALL CbManageMenu(CB_MENU_CALLBACK lpHeaderCallback, CB_MENU_ITEM* lpMenuItems, RT_UN unItemCounts, void* lpContext);

#endif /* CB_MENU_H */
