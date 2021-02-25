#include "cbroute.h"

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

rt_un16 RT_CALL CbAddRoute(void *context)
{
	CB_COMPETITION *lpCompetition;
	CB_ROUTE *lpRoute;
	rt_char lpName[256];
	rt_n read_bytes;
	rt_un32 result;

	while (RT_TRUE) {
		RtWriteStringToConsole(_R("\nEnter a name or nothing to quit:\n"));
		read_bytes = RtReadLineFromConsole(lpName, 256);

		/* Error. */
		if (read_bytes == -1) {
			ret = 1;
			break;
		}

		/* Exit. */
		if (read_bytes == 0) {
			ret = 0;
			break;
		}

		/* Too long name. */
		if (read_bytes >= CB_ROUTE_NAME_SIZE) {
			RtWriteStringToConsole(_R("Too long name.\n"));
		} else {
			lpCompetition = (CB_COMPETITION*)context;
			if (!RtNewArrayItem((void**)&lpCompetition->lpRoutes, (void**)&lpRoute)) {
				ret = 1;
				break;
			}
			RtCopyStringWithSize(lpRoute->lpName, CB_ROUTE_NAME_SIZE, lpName, read_bytes);
		}
	}
	return ret;
}

rt_un16 RT_CALL CbDeleteRoute(void *context)
{
	CB_COMPETITION *lpCompetition;
	rt_n nRoutesCount;
	rt_char buffer[16];
	rt_char lpIndex[256];
	rt_n nIndex;
	rt_n read_bytes;
	rt_n i;
	rt_un32 result;

	lpCompetition = (CB_COMPETITION*)context;

	while (RT_TRUE) {
		RtWriteStringToConsole(_R("\nEnter an index or nothing to quit\n"));

		CbWriteSeparator();
		nRoutesCount = RtGetArraySize(lpCompetition->lpRoutes);
		for (i = 0; i < nRoutesCount; i++) {
			RtConvertNToString(i + 1, buffer, 16);
			RtWriteStringsToConsole(4, buffer, _R(": "), lpCompetition->lpRoutes[i].lpName, _R("\n"));
		}
		CbWriteSeparator();

		read_bytes = RtReadLineFromConsole(lpIndex, 256);

		/* Error. */
		if (read_bytes == -1) {
			ret = 1;
			break;
		}

		/* Exit. */
		if (read_bytes == 0) {
			ret = 0;
			break;
		}

		/* Parse index. */
		if (RtParseNFromString(lpIndex, &nIndex)) {
			if ((nIndex >= 1) && (nIndex <= nRoutesCount)) {
				if (!RtDeleteArrayItemIndex((void**)&lpCompetition->lpRoutes, nIndex - 1)) {
					ret = 1;
					break;
				}
			}
		}
	}

	return ret;
}

CB_MENU_ITEM cb_lpRoutesMenuItems[2] = {{_R("Add"), &CbAddRoute}, {_R("Delete"), &CbDeleteRoute}};


rt_un16 RT_CALL CbRoutesHeaderCallback(void *context)
{
	CB_COMPETITION *lpCompetition;
	rt_n nRoutesCount;
	rt_n i;

	CbWriteMenuTitle(_R("Routes"));
	lpCompetition = (CB_COMPETITION*)context;

	CbWriteSeparator();
	nRoutesCount = RtGetArraySize(lpCompetition->lpRoutes);
	for (i = 0; i < nRoutesCount; i++) {
		RtWriteStringsToConsole(2, lpCompetition->lpRoutes[i].lpName, _R("\n"));
	}
	CbWriteSeparator();

	return 0;
}

rt_un16 RT_CALL CbRoutes(void *context)
{
	rt_un32 result;

	ret = CbManageMenu(&CbRoutesHeaderCallback, cb_lpRoutesMenuItems, 2, context);

	return ret;
}
