#include <rpr.h>
#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>

rt_un16 RT_CALL RtMain(rt_n32 argc, rt_char *argv[])
{
	RT_ARRAY zzMessage;
	rt_un16 result;

	if (rt_console_write_string(rt_char_CreateString(&zzMessage, _R("Hello, world!\n")))) {
		result = 0;
	} else {
		result = 1;
	}
	return result;
}
