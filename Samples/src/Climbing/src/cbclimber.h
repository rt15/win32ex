#ifndef CB_CLIMBER_H
#define CB_CLIMBER_H

#include <RtWin32Ex.h>

#define CB_CLIMBER_NAME_SIZE 64

typedef struct _CB_CLIMBER
{
  RT_CHAR lpName[CB_CLIMBER_NAME_SIZE];
}
CB_CLIMBER;

RT_UN16 RT_CALL CbClimbers(void* lpContext);

#endif /* CB_CLIMBER_H */
