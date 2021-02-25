#ifndef CB_ROUTE_H
#define CB_ROUTE_H

#include <rpr.h>

#define CB_ROUTE_NAME_SIZE 64

typedef struct _CB_ROUTE {
	rt_char lpName[CB_ROUTE_NAME_SIZE];
}
CB_ROUTE;

rt_un16 RT_CALL CbRoutes(void *context);

#endif /* CB_ROUTE_H */
