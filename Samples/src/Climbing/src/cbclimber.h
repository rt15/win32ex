#ifndef CB_CLIMBER_H
#define CB_CLIMBER_H

#include <rpr.h>

#define CB_CLIMBER_NAME_SIZE 64

typedef struct _CB_CLIMBER {
	rt_char lpName[CB_CLIMBER_NAME_SIZE];
}
CB_CLIMBER;

rt_un16 RT_CALL CbClimbers(void *context);

#endif /* CB_CLIMBER_H */
