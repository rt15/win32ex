#ifndef RT_WIN32_EX_H
#define RT_WIN32_EX_H

/**
 * @file
 * This special header must be included by win32ex API consumers.<br>
 * It includes all headers of the library except some special headers.
 *
 * <p>
 * It has no associated .c file.
 * </p>
 */

#include "layer000/RtWin32ExTypes.h"

#include "layer001/RtAtomic.h"
#include "layer001/RtComparison.h"
#include "layer001/RtInputStream.h"
#include "layer001/RtMemory.h"
#include "layer001/RtOutputStream.h"
/* #include "layer001/RtWin32ExOsHeaders.h", internal use only. */

#include "layer002/RtBinarySearch.h"
#include "layer002/RtChrono.h"
#include "layer002/RtCriticalsection.h"
#include "layer002/RtError.h"
#include "layer002/RtEvent.h"
#include "layer002/RtIoDevice.h"
#include "layer002/RtQuickSort.h"
#include "layer002/RtSleep.h"
#include "layer002/RtVirtualMemory.h"

#include "layer003/RtChar.h"
#include "layer003/RtChar8.h"
#include "layer003/RtEnvVar.h"
#include "layer003/RtFastInitialization.h"
#include "layer003/RtFile.h"
#include "layer003/RtHeap.h"
#include "layer003/RtInitialization.h"
#include "layer003/RtPipe.h"
#include "layer003/RtThread.h"

#include "layer004/RtArray.h"
#include "layer004/RtBase64.h"
#include "layer004/RtCommandLineArgs.h"
#include "layer004/RtEncoding.h"
#include "layer004/RtFileSystem.h"
#include "layer004/RtPageHeap.h"
#include "layer004/RtRandom.h"
#include "layer004/RtStaticHeap.h"
#include "layer004/RtSystemInfo.h"
#include "layer004/RtUrl.h"

#include "layer005/RtConsole.h"
#include "layer005/RtEnvVars.h"
#include "layer005/RtLinkedList.h"
#include "layer005/RtList.h"
#include "layer005/RtRuntimeHeap.h"
#include "layer005/RtSortableArray.h"

#include "layer006/RtErrorMessage.h"
#include "layer006/RtSocket.h"
#include "layer006/RtSmallFile.h"
#include "layer006/RtTableMetadata.h"
#include "layer006/RtTempFile.h"

#include "layer007/RtProcess.h"
#include "layer007/RtProperties.h"
#include "layer007/RtTable.h"

#endif /* RT_WIN32_EX_H */
