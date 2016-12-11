#include "zzstrings.h"

RT_CHAR* zz_lpStrings[] = {
  /* ZZ_STRINGS_APPLICATION_NAME             */ _R("Climbing Contest"),
  /* ZZ_STRINGS_CLIMBERS                     */ _R("Climbers"),
  /* ZZ_STRINGS_ROUTES                       */ _R("Routes"),
  /* ZZ_STRINGS_GRADES                       */ _R("Grades")
  };

RT_CHAR* RT_CALL ZzGetString(RT_UN unId)
{
  return zz_lpStrings[unId];
}
