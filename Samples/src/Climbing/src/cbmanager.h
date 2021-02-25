#ifndef CB_MANAGER_H
#define CB_MANAGER_H

#include <rpr.h>

#include "cbcompetition.h"

typedef struct _CB_MANAGER_CONTEXT {
	CB_COMPETITION *lpCompetition;
	rt_n nClass;
	rt_char *lpClassName;
}
CB_MANAGER_CONTEXT;

rt_s RT_CALL CbManageEntities(CB_MANAGER_CONTEXT *context);

#endif /* CB_MANAGER_H */
