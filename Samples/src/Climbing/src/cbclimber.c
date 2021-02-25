#include "cbclimber.h"

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

rt_un16 RT_CALL CbAddClimber(void *context)
{
	CB_COMPETITION *lpCompetition;
	CB_CLIMBER *lpClimber;
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
		if (read_bytes >= CB_CLIMBER_NAME_SIZE) {
			RtWriteStringToConsole(_R("Too long name.\n"));
		} else {
			lpCompetition = (CB_COMPETITION*)context;
			if (!RtNewArrayItem((void**)&lpCompetition->lpClimbers, (void**)&lpClimber)) {
				ret = 1;
				break;
			}
			RtCopyStringWithSize(lpClimber->lpName, CB_CLIMBER_NAME_SIZE, lpName, read_bytes);
		}
	}
	return ret;
}

rt_un16 RT_CALL CbDeleteClimber(void *context)
{
	CB_COMPETITION *lpCompetition;
	rt_n nClimbersCount;
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
		nClimbersCount = RtGetArraySize(lpCompetition->lpClimbers);
		for (i = 0; i < nClimbersCount; i++) {
			RtConvertNToString(i + 1, buffer, 16);
			RtWriteStringsToConsole(4, buffer, _R(": "), lpCompetition->lpClimbers[i].lpName, _R("\n"));
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
			if ((nIndex >= 1) && (nIndex <= nClimbersCount)) {
				if (!RtDeleteArrayItemIndex((void**)&lpCompetition->lpClimbers, nIndex - 1)) {
					ret = 1;
					break;
				}
			}
		}
	}

	return ret;
}

CB_MENU_ITEM cb_lpClimbersMenuItems[2] = {{_R("Add"), &CbAddClimber}, {_R("Delete"), &CbDeleteClimber}};


rt_un16 RT_CALL CbClimbersHeaderCallback(void *context)
{
	CB_COMPETITION *lpCompetition;
	rt_n nClimbersCount;
	rt_n i;

	CbWriteMenuTitle(_R("Climbers"));
	lpCompetition = (CB_COMPETITION*)context;

	CbWriteSeparator();
	nClimbersCount = RtGetArraySize(lpCompetition->lpClimbers);
	for (i = 0; i < nClimbersCount; i++) {
		RtWriteStringsToConsole(2, lpCompetition->lpClimbers[i].lpName, _R("\n"));
	}
	CbWriteSeparator();

	return 0;
}

rt_un16 RT_CALL CbClimbers(void *context)
{
	rt_un32 result;

	ret = CbManageMenu(&CbClimbersHeaderCallback, cb_lpClimbersMenuItems, 2, context);

	return ret;
}

