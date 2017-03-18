#include "ZzTests.h"

#include "ZzTools.h"

#define ZZ_PORT_NUMBER 42334

/**
 * Thread that create a server socket and send "Hello, world!" to the first connecting socket.
 */
RT_UN32 RT_CALL ZzServerSocketThreadCallback(void* lpParameter)
{
  RT_SOCKET zzSocket;
  RT_SOCKET zzAcceptedSocket;
  RT_SOCKET_ADDRESS zzSocketAddress;
  RT_B bSocketCreated;
  RT_B bAcceptedSocketCreated;
  RT_EVENT* lpEvent;
  RT_CHAR8* lpMsg;
  RT_UN32 unResult;

  bSocketCreated = RT_FALSE;
  bAcceptedSocketCreated = RT_FALSE;

  lpEvent = (RT_EVENT*)lpParameter;

  if (!RtCreateSocket(&zzSocket, RT_SOCKET_ADDRESS_FAMILY_IPV4, RT_SOCKET_TYPE_STREAM, RT_SOCKET_PROTOCOL_TCP, RT_TRUE, RT_FALSE)) goto handle_error;
  bSocketCreated = RT_TRUE;

  if (!RtBindSocket(&zzSocket, ZZ_PORT_NUMBER)) goto handle_error;
  if (!RtListenFromSocket(&zzSocket)) goto handle_error;

  if (!RtSignalEvent(lpEvent)) goto handle_error;

  if (!RtAcceptSocketConnection(&zzSocket, &zzAcceptedSocket, &zzSocketAddress)) goto handle_error;
  bAcceptedSocketCreated = RT_TRUE;

  lpMsg = "Hello, world!";
  if (RtSendThroughSocket(&zzAcceptedSocket, lpMsg, RtGetString8Size(lpMsg) + 1, 0) == RT_TYPE_MAX_UN) goto handle_error;

  if (!RtShutdownSocket(&zzAcceptedSocket, RT_SOCKET_SHUTDOWN_BOTH)) goto handle_error;

  unResult = RT_SUCCESS;
free_resources:
  if (bAcceptedSocketCreated)
  {
    bAcceptedSocketCreated = RT_FALSE;
    if (!RtFreeSocket(&zzAcceptedSocket) && unResult) goto handle_error;
  }
  if (bSocketCreated)
  {
    bSocketCreated = RT_FALSE;
    if (!RtFreeSocket(&zzSocket) && unResult) goto handle_error;
  }
  return unResult;

handle_error:
  RtWriteLastErrorMessage(_R("Error in server socket thread: "));
  /* Ensure that main thread will not wait for ever. */
  RtSignalEvent(lpEvent);
  unResult = RT_FAILURE;
  goto free_resources;
}

/**
 * Tests socket assuming RtInitializeSockets/RtCleanUpSockets are called.
 */
RT_B RT_CALL ZzTestSockets()
{
  RT_EVENT zzEvent;
  RT_B bEventCreated;
  RT_THREAD zzThread;
  RT_B bThreadCreated;
  RT_B bSocketCreated;
  RT_CHAR8 lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_SOCKET zzSocket;
  RT_UN unReceived;
  RT_ADDRESS_IPV4 zzAddress;
  RT_SOCKET_ADDRESS_IPV4 zzSocketAddress;
  RT_B bResult;

  bEventCreated = RT_FALSE;
  bThreadCreated = RT_FALSE;
  bSocketCreated = RT_FALSE;

  if (!RtCreateEvent(&zzEvent)) goto handle_error;
  bEventCreated = RT_TRUE;

  if (!RtCreateThread(&zzThread, &ZzServerSocketThreadCallback, &zzEvent)) goto handle_error;
  bThreadCreated = RT_TRUE;

  if (!RtWaitForEvent(&zzEvent)) goto handle_error;

  /* Let some time for the server socket to accept connections. */
  RtSleep(10);

  if (!RtCreateSocket(&zzSocket, RT_SOCKET_ADDRESS_FAMILY_IPV4, RT_SOCKET_TYPE_STREAM, RT_SOCKET_PROTOCOL_TCP, RT_TRUE, RT_FALSE)) goto handle_error;
  bSocketCreated = RT_TRUE;

  RtCreateIpv4LoopbackAddress(&zzAddress);
  RtCreateIpv4SocketAddress(&zzSocketAddress, &zzAddress, ZZ_PORT_NUMBER);

  if (!RtConnectSocketWithAddress(&zzSocket, (RT_SOCKET_ADDRESS*)&zzSocketAddress)) goto handle_error;

  unReceived = RtReceiveFromSocket(&zzSocket, lpBuffer, RT_CHAR_BIG_STRING_SIZE, 0);
  if (unReceived == RT_TYPE_MAX_UN) goto handle_error;
  if (unReceived != RtGetString8Size("Hello, world!") + 1) goto handle_error;
  if (RtCompareString8s(lpBuffer, "Hello, world!")) goto handle_error;
  if (!RtShutdownSocket(&zzSocket, RT_SOCKET_SHUTDOWN_BOTH)) goto handle_error;

  if (!RtJoinAndCheckThread(&zzThread)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bSocketCreated)
  {
    bSocketCreated = RT_FALSE;
    if (!RtFreeSocket(&zzSocket) && bResult) goto handle_error;
  }
  if (bThreadCreated)
  {
    bThreadCreated = RT_FALSE;
    if (!RtFreeThread(&zzThread) && bResult) goto handle_error;
  }
  if (bEventCreated)
  {
    bEventCreated = RT_FALSE;
    if (!RtFreeEvent(&zzEvent) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestSocket()
{
  RT_B bSocketsInitialized;
  RT_B bResult;

  bSocketsInitialized = RT_FALSE;

  if (!RtInitializeSockets()) goto handle_error;
  bSocketsInitialized = RT_TRUE;

  if (!ZzTestSockets()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bSocketsInitialized)
  {
    bSocketsInitialized = RT_FALSE;
    if (!RtCleanUpSockets() && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
