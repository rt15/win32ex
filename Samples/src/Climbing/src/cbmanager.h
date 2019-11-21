#ifndef CB_MANAGER_H
#define CB_MANAGER_H

#include <RtWin32Ex.h>

#include "cbcompetition.h"

typedef struct _CB_MANAGER_CONTEXT
{
  CB_COMPETITION* lpCompetition;
  RT_N nClass;
  RT_CHAR* lpClassName;
}
CB_MANAGER_CONTEXT;

RT_B RT_CALL CbManageEntities(CB_MANAGER_CONTEXT* lpContext);

#endif /* CB_MANAGER_H */
