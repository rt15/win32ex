#include "cbmenu.h"

rt_s RT_CALL CbManageMenu(CB_MENU_CALLBACK lpHeaderCallback, CB_MENU_ITEM *lpMenuItems, rt_un unItemCounts, void *context)
{
	RT_ARRAY rtConversionBuffer;
	rt_char lpConversionBuffer[64];
	RT_ARRAY rtMessage;
	rt_char message[256];
	rt_char read_bytes;
	rt_n i;
	rt_s ret;

	while (RT_TRUE) {
		if (!lpHeaderCallback(context)) goto error;

		rt_array_Create(&rtMessage, message, sizeof(rt_char), 256);
		rt_array_Create(&rtConversionBuffer, lpConversionBuffer, sizeof(rt_char), 64);

		for (i = 0; i < unItemCounts; i++) {
			if (!rt_char_append_n(i + 1, &rtConversionBuffer)) goto error;

			if (!rt_char_AppendCString(&rtMessage, lpMenuItems[i].lpTitle)) goto error;
			if (!rt_char_AppendCString(&rtMessage, _R(": "))) goto error;
			if (!rt_array_Append(&rtMessage, &rtConversionBuffer)) goto error;
			if (!rt_char_AppendCString(&rtMessage, _R("\n"))) goto error;
		}
		if (!rt_char_AppendCString(&rtMessage, _R("\nExit: 0\n"))) goto error;
		rt_console_write_string(&rtMessage);

		read_bytes = rt_console_read_char();
		if (read_bytes == _R('0')) {
			break;
		} else if (read_bytes >= _R('1') && read_bytes <= _R('9')) {
			read_bytes = read_bytes - _R('0');
			if (read_bytes <= unItemCounts) {
				if (!lpMenuItems[read_bytes - 1].lpMenuCallback(context)) goto error;
			}
		}
	}

	ret = RT_OK;
free:
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}
