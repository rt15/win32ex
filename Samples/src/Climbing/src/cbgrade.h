#ifndef CB_GRADE_H
#define CB_GRADE_H

#include <rpr.h>

#define CB_GRADE_LABEL_SIZE 8

typedef struct _CB_GRADE {
	rt_char lpLabel[CB_GRADE_LABEL_SIZE];
	rt_un32 unPoints;
}
CB_GRADE;

rt_un16 RT_CALL CbGrades(void *context);

#endif /* CB_GRADE_H */
