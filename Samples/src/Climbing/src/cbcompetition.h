#ifndef CB_COMPETITION_H
#define CB_COMPETITION_H

#include <RtWin32Ex.h>

#define CB_TABLES_COUNT 3

#define CB_CLIMBERS_INDEX 0
#define CB_ROUTES_INDEX 1
#define CB_GRADES_INDEX 2

#define CB_NAME_SIZE 64

typedef struct _CB_OBJECT
{
  RT_CHAR lpName[CB_NAME_SIZE];
}
CB_OBJECT;

typedef struct _CB_CLIMBER
{
  CB_OBJECT header;
}
CB_CLIMBER;

typedef struct _CB_CLIMBER_ITEM
{
  CB_CLIMBER climber;
  RT_N nIndexReference;
}
CB_CLIMBER_ITEM;

typedef struct _CB_ROUTE
{
  CB_OBJECT header;
}
CB_ROUTE;

typedef struct _CB_ROUTE_ITEM
{
  CB_ROUTE route;
  RT_N nIndexReference;
}
CB_ROUTE_ITEM;


typedef struct _CB_GRADE
{
  CB_OBJECT header;
  RT_UN32 unPoints;
}
CB_GRADE;

typedef struct _CB_GRADE_ITEM
{
  CB_GRADE grade;
  RT_N nIndexReference;
}
CB_GRADE_ITEM;

typedef struct _CB_COMPETITION
{
  RT_TABLE lpTables[CB_TABLES_COUNT];
}
CB_COMPETITION;

#endif /* CB_COMPETITION_H */
