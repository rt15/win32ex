#ifndef RT_INPUT_STREAM_H
#define RT_INPUT_STREAM_H

#include "layer000/RtWin32ExTypes.h"

typedef struct _RT_INPUT_STREAM RT_INPUT_STREAM;

typedef RT_B (RT_CALL *RT_INPUT_STREAM_READ)(RT_INPUT_STREAM* lpInputStream, RT_CHAR8* lpBuffer, RT_UN unBytesToRead, RT_UN* lpBytesRead);

struct _RT_INPUT_STREAM
{
  RT_INPUT_STREAM_READ lpRead;
};

#endif /* RT_INPUT_STREAM_H */
