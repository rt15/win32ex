#ifndef CB_ROUTE_H
#define CB_ROUTE_H

#include <RtWin32Ex.h>

#define CB_ROUTE_NAME_SIZE 64

typedef struct _CB_ROUTE
{
  RT_CHAR lpName[CB_ROUTE_NAME_SIZE];
}
CB_ROUTE;

RT_UN16 RT_CALL CbRoutes(void* lpContext);

#endif /* CB_ROUTE_H */
