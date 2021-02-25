#include "zzStrings.h"

rt_char *zz_lpStrings[] = {
	/* ZZ_STRINGS_APPLICATION_NAME						 */ _R("Climbing Contest"),
	/* ZZ_STRINGS_CLIMBERS										 */ _R("Climbers"),
	/* ZZ_STRINGS_ROUTES											 */ _R("Routes"),
	/* ZZ_STRINGS_GRADES											 */ _R("Grades"),
	/* ZZ_STRINGS_ADD													*/ _R("Add"),
	/* ZZ_STRINGS_DELETE											 */ _R("Delete")
	};

rt_char *RT_CALL ZzGetString(rt_un unId)
{
	return zz_lpStrings[unId];
}
