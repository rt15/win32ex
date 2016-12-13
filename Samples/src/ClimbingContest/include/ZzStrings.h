#ifndef ZZ_STRINGS_H
#define ZZ_STRINGS_H

#include <RtWin32Ex.h>

#define ZZ_STRINGS_APPLICATION_NAME    0
#define ZZ_STRINGS_CLIMBERS            1
#define ZZ_STRINGS_ROUTES              2
#define ZZ_STRINGS_GRADES              3
#define ZZ_STRINGS_ADD                 4
#define ZZ_STRINGS_DELETE              5

RT_CHAR* RT_CALL ZzGetString(RT_UN unId);

#endif /* ZZ_STRINGS_H */
