#include <RtWin32Ex.h>
#include "ZzTools.h"

#define ZZ_PORT_NUMBER 42334



/**
 * Thread that create a server socket and send "Hello, world!" to the first connecting socket.
 */
RT_UN32 RT_CALL ZzServerSocketThreadCallback(void* lpParameter)
{
  RT_SOCKET rtSocket;
  RT_SOCKET rtAcceptedSocket;
  RT_SOCKET_ADDRESS rtSocketAddress;
  RT_B bSocketCreated;
  RT_B bAcceptedSocketCreated;
  RT_EVENT* lpEvent;
  RT_CHAR8* lpMsg;
  RT_UN32 nResult;

  bSocketCreated = RT_FALSE;
  bAcceptedSocketCreated = RT_FALSE;

  lpEvent = (RT_EVENT*)lpParameter;

  if (!RtCreateSocket(&rtSocket, RT_SOCKET_ADDRESS_FAMILY_IPV4, RT_SOCKET_TYPE_STREAM, RT_SOCKET_PROTOCOL_TCP, RT_TRUE, RT_TRUE)) goto handle_error;
  bSocketCreated = RT_TRUE;

  if (!RtBindSocket(&rtSocket, ZZ_PORT_NUMBER)) goto handle_error;
  if (!RtListenFromSocket(&rtSocket)) goto handle_error;

  if (!RtSignalEvent(lpEvent)) goto handle_error;

  if (!RtAcceptSocketConnection(&rtSocket, &rtAcceptedSocket, &rtSocketAddress)) goto handle_error;
  bAcceptedSocketCreated = RT_TRUE;

  lpMsg = "Hello, world!";
  if (RtSendThroughSocket(&rtAcceptedSocket, lpMsg, RtGetString8Size(lpMsg) + 1, 0) == RT_TYPE_MAX_UN) goto handle_error;

  if (!RtShutdownSocket(&rtAcceptedSocket, RT_SOCKET_SHUTDOWN_BOTH)) goto handle_error;

  nResult = RT_TRUE;
free_resources:
  if (bAcceptedSocketCreated)
  {
    bAcceptedSocketCreated = RT_FALSE;
    if (!RtFreeSocket(&rtAcceptedSocket) && nResult) goto handle_error;
  }
  if (bSocketCreated)
  {
    bSocketCreated = RT_FALSE;
    if (!RtFreeSocket(&rtSocket) && nResult) goto handle_error;
  }
  return nResult;

handle_error:
  ZzWriteLastErrorMessage(_R("Error in server socket thread: "));
  /* Ensure that main thread will not wait for ever. */
  RtSignalEvent(lpEvent);
  nResult = RT_FALSE;
  goto free_resources;
}

/**
 * Tests socket assuming RtInitializeSockets/RtCleanUpSockets are called.
 */
RT_B RT_CALL TtTestSockets()
{
  RT_EVENT rtEvent;
  RT_B bEventCreated;
  RT_THREAD rtThread;
  RT_B bThreadCreated;
  RT_B bSocketCreated;
  RT_CHAR8 lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_SOCKET rtSocket;
  RT_UN unReceived;
  RT_ADDRESS_IPV4 rtAddress;
  RT_SOCKET_ADDRESS_IPV4 rtSocketAddress;
  RT_B bResult;

  bEventCreated = RT_FALSE;
  bThreadCreated = RT_FALSE;
  bSocketCreated = RT_FALSE;

  if (!RtCreateEvent(&rtEvent)) goto handle_error;
  bEventCreated = RT_TRUE;

  if (!RtCreateThread(&rtThread, &ZzServerSocketThreadCallback, &rtEvent)) goto handle_error;
  bThreadCreated = RT_TRUE;

  if (!RtWaitForEvent(&rtEvent)) goto handle_error;

  /* Let some time for the server socket to accept connections. */
  RtSleep(10);

  if (!RtCreateSocket(&rtSocket, RT_SOCKET_ADDRESS_FAMILY_IPV4, RT_SOCKET_TYPE_STREAM, RT_SOCKET_PROTOCOL_TCP, RT_TRUE, RT_TRUE)) goto handle_error;
  bSocketCreated = RT_TRUE;

  RtCreateIpv4LoopbackAddress(&rtAddress);
  RtCreateIpv4SocketAddress(&rtSocketAddress, &rtAddress, ZZ_PORT_NUMBER);

  if (!RtConnectSocketWithAddress(&rtSocket, (RT_SOCKET_ADDRESS*)&rtSocketAddress)) goto handle_error;

  unReceived = RtReceiveFromSocket(&rtSocket, lpBuffer, RT_CHAR_BIG_STRING_SIZE, 0);
  if (unReceived == RT_TYPE_MAX_UN) goto handle_error;
  if (unReceived != RtGetString8Size("Hello, world!") + 1) goto handle_error;
  if (RtCompareString8s(lpBuffer, "Hello, world!")) goto handle_error;
  if (!RtShutdownSocket(&rtSocket, RT_SOCKET_SHUTDOWN_BOTH)) goto handle_error;

  if (!RtJoinAndCheckThread(&rtThread)) goto handle_error;

  bResult = RT_TRUE;
free_resources:
  if (bSocketCreated)
  {
    bSocketCreated = RT_FALSE;
    if (!RtFreeSocket(&rtSocket) && bResult) goto handle_error;
  }
  if (bThreadCreated)
  {
    bThreadCreated = RT_FALSE;
    if (!RtFreeThread(&rtThread) && bResult) goto handle_error;
  }
  if (bEventCreated)
  {
    bEventCreated = RT_FALSE;
    if (!RtFreeEvent(&rtEvent) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_UN16 RT_CALL TtTestSocket()
{
  RT_UN16 unResult;

  unResult = 1;

  if (!RtInitializeSockets()) goto the_end;

  if (!TtTestSockets()) goto clean_up_sockets;

  unResult = 0;

clean_up_sockets:
  if (!RtCleanUpSockets()) unResult = 1;
the_end:
  return unResult;
}
