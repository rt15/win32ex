#include "cbgrade.h"

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

rt_un16 RT_CALL CbAddGrade(void *context)
{
	CB_COMPETITION *lpCompetition;
	CB_GRADE *lpGrade;
	rt_char lpLabel[256];
	rt_n read_bytes;
	rt_un32 result;

	while (RT_TRUE) {
		RtWriteStringToConsole(_R("\nEnter a label or nothing to quit:\n"));
		read_bytes = RtReadLineFromConsole(lpLabel, 256);

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

		/* Too long label. */
		if (read_bytes >= CB_GRADE_LABEL_SIZE) {
			RtWriteStringToConsole(_R("Too long label.\n"));
		} else {
			lpCompetition = (CB_COMPETITION*)context;
			if (!RtNewArrayItem((void**)&lpCompetition->lpGrades, (void**)&lpGrade)) {
				ret = 1;
				break;
			}
			RtCopyStringWithSize(lpGrade->lpLabel, CB_GRADE_LABEL_SIZE, lpLabel, read_bytes);
		}
	}
	return ret;
}

rt_un16 RT_CALL CbDeleteGrade(void *context)
{
	CB_COMPETITION *lpCompetition;
	rt_n nGradesCount;
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
		nGradesCount = RtGetArraySize(lpCompetition->lpGrades);
		for (i = 0; i < nGradesCount; i++) {
			RtConvertNToString(i + 1, buffer, 16);
			RtWriteStringsToConsole(4, buffer, _R(": "), lpCompetition->lpGrades[i].lpLabel, _R("\n"));
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
			if ((nIndex >= 1) && (nIndex <= nGradesCount)) {
				if (!RtDeleteArrayItemIndex((void**)&lpCompetition->lpGrades, nIndex - 1)) {
					ret = 1;
					break;
				}
			}
		}
	}

	return ret;
}

CB_MENU_ITEM cb_lpGradesMenuItems[2] = {{_R("Add"), &CbAddGrade}, {_R("Delete"), &CbDeleteGrade}};


rt_un16 RT_CALL CbGradesHeaderCallback(void *context)
{
	CB_COMPETITION *lpCompetition;
	rt_n nGradesCount;
	rt_n i;

	CbWriteMenuTitle(_R("Grades"));
	lpCompetition = (CB_COMPETITION*)context;

	CbWriteSeparator();
	nGradesCount = RtGetArraySize(lpCompetition->lpGrades);
	for (i = 0; i < nGradesCount; i++) {
		RtWriteStringsToConsole(2, lpCompetition->lpGrades[i].lpLabel, _R("\n"));
	}
	CbWriteSeparator();

	return 0;
}

rt_un16 RT_CALL CbGrades(void *context)
{
	rt_un32 result;

	ret = CbManageMenu(&CbGradesHeaderCallback, cb_lpGradesMenuItems, 2, context);

	return ret;
}
