#ifndef RT_OUTPUT_STREAM_H
#define RT_OUTPUT_STREAM_H

#include "layer000/RtWin32ExTypes.h"

typedef struct _RT_OUTPUT_STREAM RT_OUTPUT_STREAM;

typedef RT_B (RT_CALL *RT_OUTPUT_STREAM_WRITE)(RT_OUTPUT_STREAM* lpOutputStream, RT_CHAR8* lpData, RT_UN unBytesToWrite);

struct _RT_OUTPUT_STREAM
{
  RT_OUTPUT_STREAM_WRITE lpWrite;
};

#endif /* RT_OUTPUT_STREAM_H */
