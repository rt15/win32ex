#ifndef CB_GRADE_H
#define CB_GRADE_H

#include <RtWin32Ex.h>

#define CB_GRADE_LABEL_SIZE 8

typedef struct _CB_GRADE
{
  RT_CHAR lpLabel[CB_GRADE_LABEL_SIZE];
  RT_UN32 unPoints;
}
CB_GRADE;

RT_UN16 RT_CALL CbGrades(void* lpContext);

#endif /* CB_GRADE_H */
