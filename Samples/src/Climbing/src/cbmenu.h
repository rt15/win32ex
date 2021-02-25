#ifndef CB_MENU_H
#define CB_MENU_H

#include <rpr.h>

typedef rt_b (RT_CALL *CB_MENU_CALLBACK)(void *context);

typedef struct _CB_MENU_ITEM {
	rt_char *lpTitle;
	CB_MENU_CALLBACK lpMenuCallback;
}
CB_MENU_ITEM;

/**
 * @param nItemCounts Must be strictly less than 10.
 */
rt_s RT_CALL CbManageMenu(CB_MENU_CALLBACK lpHeaderCallback, CB_MENU_ITEM *lpMenuItems, rt_un unItemCounts, void *context);

#endif /* CB_MENU_H */
