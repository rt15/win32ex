#include "layer004/RtSocket.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer003/RtMemory.h"

/**
 * @file
 *
 * <p>
 * Under Windows currently WSAGetLastError is the same as GetLastError.
 * </p>
 *
 * <p>
 * Socket options candidates that could be used with <tt>setsockopt</tt>:
 * </p>
 * <ul>
 *   <li>SO_BROADCAST</li>
 *   <li>SO_DEBUG (Not implemented under Windows).</li>
 *   <li>SO_DONTROUTE (MSG_DONTROUTE of send/sendto can be used instead).</li>
 *   <li>SO_KEEPALIVE</li>
 *   <li>SO_LINGER</li>
 *   <li>SO_OOBINLINE</li>
 *   <li>SO_RCVBUF</li>
 *   <li>SO_RCVTIMEO</li>
 *   <li>SO_REUSEADDR</li>
 *   <li>SO_SNDBUF</li>
 *   <li>SO_SNDTIMEO</li>
 * </ul>
 *
 * <p>
 * Client socket:
 * </p>
 * <ol>
 *   <li>socket: Create the socket for given protocol.</li>
 *   <li>setsockopt: Set socket options.</li>
 *   <li>gethostbyname/gethostbyaddr: Retrieve a sockaddr_in structure from an host name or ip address.</li>
 *   <li>connect: Connect to given address (sockaddr) and port.</li>
 *   <li>send/recv for TCP or sendto/recvfrom for UDP.</li>
 *   <li>shutdown.</li>
 *   <li>closesocket.</li>
 * </ol>
 *
 * <p>
 * Server socket:
 * </p>
 * <ol>
 *   <li>socket: Create the socket for given protocol.</li>
 *   <li>setsockopt: Set socket options.</li>
 *   <li>bind: Associates a socket with a port (And optionally an IP address).</li>
 *   <li>listen: Start listening.</li>
 *   <li>accept: Accept an incoming connection an create associated (accepted) socket.</li>
 *   <li>send/recv for TCP or sendto/recvfrom for UDP on accepted socket.</li>
 *   <li>shutdown accepted socket only (There is no communication on the server socket).</li>
 *   <li>closesocket on both sockets.</li>
 * </ol>
 */

/**
 * ADDRINFOW/GetAddrInfoW/FreeAddrInfoW are not defined in MinGW32 as of 02.11.2016.
 */
#ifdef RT_DEFINE_WINDOWS

 typedef struct _RT_ADDRINFOW {
  int ai_flags;
  int ai_family;
  int ai_socktype;
  int ai_protocol;
  size_t ai_addrlen;
  PWSTR ai_canonname;
  struct sockaddr *ai_addr;
  struct _RT_ADDRINFOW *ai_next;
} RT_ADDRINFOW;


typedef int (RT_STDCALL *RT_GET_ADDR_INFO_W)(PCWSTR pNodeName, PCWSTR pServiceName, RT_ADDRINFOW *pHints, RT_ADDRINFOW** ppResult);

typedef void (RT_STDCALL *RT_FREE_ADDR_INFO_W)(RT_ADDRINFOW* pAddrInfo);

RT_GET_ADDR_INFO_W rt_lpGetAddrInfoW;
RT_FREE_ADDR_INFO_W rt_lpFreeAddrInfoW;

#endif

RT_B RT_API RtInitializeSockets()
 {
#ifdef RT_DEFINE_WINDOWS
  int nReturnedValue;
  WORD unVersionRequested;
  WSADATA rtWsaData;
  RT_B bWSAStartupCalled;
  HMODULE hWs2Module;
#endif
   RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  bWSAStartupCalled = RT_FALSE;

  /* Version 2.2 is supported by even Windows NT 4.0 and Windows 95 with the Windows Socket 2 update. */
  unVersionRequested = MAKEWORD(2, 2);
  nReturnedValue = WSAStartup(unVersionRequested, &rtWsaData);
  if (nReturnedValue)
  {
    /* WSAStartup returns error code and might not call WSASetLastError. */
    RtSetLastError(nReturnedValue);
    goto handle_error;
  }
  bWSAStartupCalled = RT_TRUE;

  /* Check returned version. WSAStartup return same or less than requested, never greater. */
  if (LOBYTE(rtWsaData.wVersion) != 2 || HIBYTE(rtWsaData.wVersion) != 2)
  {
      /* There has been a successful call to WSAStartup but no way to use 2.2 sockets. */
      RtSetLastError(RT_ERROR_FUNCTION_FAILED);
      goto handle_error;
  }

  /* We use GetModuleHandle as WSAStartup has been called. */
  hWs2Module = GetModuleHandle(_R("ws2_32"));
  if (!hWs2Module) goto handle_error;

  /* Workaround until GetAddrInfoW/FreeAddrInfoW are widely available in ws2tcpip.h of MinGW32. */
  rt_lpGetAddrInfoW = (RT_GET_ADDR_INFO_W)GetProcAddress(hWs2Module, "GetAddrInfoW");
  if (!rt_lpGetAddrInfoW) goto handle_error;
  rt_lpFreeAddrInfoW = (RT_FREE_ADDR_INFO_W)GetProcAddress(hWs2Module, "FreeAddrInfoW");
  if (!rt_lpFreeAddrInfoW) goto handle_error;

#endif
  bResult = RT_TRUE;

#ifdef RT_DEFINE_WINDOWS
free_resources:
#endif
  return bResult;

#ifdef RT_DEFINE_WINDOWS
handle_error:
  if (bWSAStartupCalled)
  {
    WSACleanup();
    bWSAStartupCalled = RT_FALSE;
  }
  bResult = RT_FALSE;
  goto free_resources;
#endif
}

RT_B RT_API RtCreateSocket(RT_SOCKET* lpSocket, RT_UN unAddressFamily, RT_UN unType, RT_UN unProtocol, RT_B bBlocking, RT_B bCloseOnExec)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unFlags;
#else
  RT_UN unActualType;
#endif
  RT_B bResult;

  lpSocket->unAddressFamily = unAddressFamily;
#ifdef RT_DEFINE_WINDOWS
  if (bBlocking)
  {
    unFlags = 0;
  }
  else
  {
    /* Mandatory for non-blocking sockets. */
    unFlags = WSA_FLAG_OVERLAPPED;
  }
  /* WSA_FLAG_NO_HANDLE_INHERIT flag is in early versions of Windows only. */
  lpSocket->unSocket = WSASocket(unAddressFamily, unType, unProtocol, RT_NULL, 0, unFlags);
  if (lpSocket->unSocket == INVALID_SOCKET) goto handle_error;

#else /* NOT RT_DEFINE_WINDOWS */

  /* Add needed flags with bitwise OR in socket type. */
  unActualType = unType;
  if (!bBlocking)
  {
    unActualType |= SOCK_NONBLOCK;
  }
  if (bCloseOnExec)
  {
    unActualType |= SOCK_CLOEXEC;
  }

  lpSocket->nSocket = socket(unAddressFamily, unActualType, unProtocol);
  if (lpSocket->nSocket == -1) goto handle_error;
#endif

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

void RT_API RtCreateIpv4LoopbackAddress(RT_ADDRESS_IPV4* lpAddress)
{
  lpAddress->rtUnion.unAddress = htonl(INADDR_LOOPBACK);
}

void RT_API RtCreateIpv4SocketAddress(RT_SOCKET_ADDRESS_IPV4* lpSocketAddress, RT_ADDRESS_IPV4* lpAddress, RT_UN unPort)
{
  RtZeroMemory(lpSocketAddress, sizeof(RT_SOCKET_ADDRESS_IPV4));
  lpSocketAddress->unAddressFamily = RT_SOCKET_ADDRESS_FAMILY_IPV4;
  lpSocketAddress->unPort = htons(unPort);
  RtCopyMemory(lpAddress, &lpSocketAddress->rtAddress, sizeof(RT_ADDRESS_IPV4));
}

RT_B RT_API RtConnectSocket(RT_SOCKET* lpSocket, RT_CHAR* lpHostName, RT_UN unPort)
{
  RT_SOCKET_ADDRESS_IPV4 rtIpv4SocketAddress;
  RT_SOCKET_ADDRESS_IPV6 rtIpv6SocketAddress;
  RT_SOCKET_ADDRESS* lpSocketAddress;
  RT_UN unAddressFamily;
  void* lpAddress;
#ifdef RT_DEFINE_WINDOWS
  int nReturnedValue;
  RT_ADDRINFOW* lpAddressInfo;
  RT_B bAddressInfoInitialized;
#else
  struct hostent* lpHost;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  bAddressInfoInitialized = RT_FALSE;

  nReturnedValue = rt_lpGetAddrInfoW(lpHostName, RT_NULL, RT_NULL, &lpAddressInfo);
  if (nReturnedValue)
  {
    /* GetAddrInfoW returns an error code in case of error, zero otherwise. */
    SetLastError(nReturnedValue);
    goto handle_error;
  }
  bAddressInfoInitialized = RT_TRUE;
  unAddressFamily = lpAddressInfo->ai_family;

  switch (unAddressFamily)
  {
    case RT_SOCKET_ADDRESS_FAMILY_IPV4:
      lpAddress = &((SOCKADDR_IN*)lpAddressInfo->ai_addr)->sin_addr;
      break;
    case RT_SOCKET_ADDRESS_FAMILY_IPV6:
      lpAddress = &((SOCKADDR_IN6*)lpAddressInfo->ai_addr)->sin6_addr;
      break;
    default:
      SetLastError(RT_ERROR_FUNCTION_FAILED);
      goto handle_error;
  }
#else
  lpHost = gethostbyname(lpHostName);
  if (!lpHost) goto handle_error;
  unAddressFamily = lpHost->h_addrtype;
  lpAddress = &lpHost->h_addr_list[0];
#endif

  /* At this point, unAddressFamily and lpAddress must be set. */
  switch (unAddressFamily)
  {
    case RT_SOCKET_ADDRESS_FAMILY_IPV4:
      RtZeroMemory(&rtIpv4SocketAddress, sizeof(RT_SOCKET_ADDRESS_IPV4));
      rtIpv4SocketAddress.unPort = htons((RT_UN16)unPort);
      RtCopyMemory(lpAddress, &rtIpv4SocketAddress.rtAddress, sizeof(RT_ADDRESS_IPV4));
      lpSocketAddress = (RT_SOCKET_ADDRESS*)&rtIpv4SocketAddress;
      break;
    case RT_SOCKET_ADDRESS_FAMILY_IPV6:
      RtZeroMemory(&rtIpv6SocketAddress, sizeof(RT_SOCKET_ADDRESS_IPV6));
      rtIpv4SocketAddress.unPort = htons((RT_UN16)unPort);
      RtCopyMemory(lpAddress, &rtIpv6SocketAddress.rtAddress, sizeof(RT_ADDRESS_IPV6));
      lpSocketAddress = (RT_SOCKET_ADDRESS*)&rtIpv6SocketAddress;
      break;
    default:
      RtSetLastError(RT_ERROR_FUNCTION_FAILED);
      goto handle_error;
  }
  lpSocketAddress->unAddressFamily = unAddressFamily;

  bResult = RtConnectSocketWithAddress(lpSocket, lpSocketAddress);
  goto free_resources;

free_resources:
#ifdef RT_DEFINE_WINDOWS
  if (bAddressInfoInitialized)
  {
    bAddressInfoInitialized = RT_FALSE;
    /* FreeAddrInfoW cannot fails. */
    rt_lpFreeAddrInfoW(lpAddressInfo);
  }
#endif
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_API RtConnectSocketWithAddress(RT_SOCKET* lpSocket, RT_SOCKET_ADDRESS* lpSocketAddress)
{
  int nSocketAddressSize;
  RT_B bResult;

  switch (lpSocketAddress->unAddressFamily)
  {
    case RT_SOCKET_ADDRESS_FAMILY_IPV4:
      nSocketAddressSize = sizeof(RT_SOCKET_ADDRESS_IPV4);
      break;
    case RT_SOCKET_ADDRESS_FAMILY_IPV6:
      nSocketAddressSize = sizeof(RT_SOCKET_ADDRESS_IPV6);
      break;
    default:
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
  }

#ifdef RT_DEFINE_WINDOWS
  if (connect(lpSocket->unSocket, (struct sockaddr*)lpSocketAddress, nSocketAddressSize))
  {
    /* TODO: Result is always non zero with non-blocking sockets under Windows. */
    goto handle_error;
  }
#else
  if (connect(lpSocket->nSocket, (struct sockaddr*)lpSocketAddress, nSocketAddressSize))
  {
    goto handle_error;
  }
#endif

free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_API RtBindSocket(RT_SOCKET* lpSocket, RT_UN unPort)
{
  struct sockaddr_in rtIpv4SocketAddress;
  struct sockaddr_in6 rtIpv6SocketAddress;
  struct sockaddr* lpSocketAddress;
  RT_N nAddressSize;
  RT_B bResult;

  switch (lpSocket->unAddressFamily)
  {
    case RT_SOCKET_ADDRESS_FAMILY_IPV4:
      RtZeroMemory(&rtIpv4SocketAddress, sizeof(rtIpv4SocketAddress));
      rtIpv4SocketAddress.sin_port = htons((RT_UN16)unPort);
#ifdef RT_DEFINE_WINDOWS
      rtIpv4SocketAddress.sin_addr.S_un.S_addr = INADDR_ANY;
#else
      rtIpv4SocketAddress.sin_addr.s_addr = INADDR_ANY;
#endif
      nAddressSize = sizeof(rtIpv4SocketAddress);
      lpSocketAddress = (struct sockaddr*)&rtIpv4SocketAddress;
      break;
    case RT_SOCKET_ADDRESS_FAMILY_IPV6:
      RtZeroMemory(&rtIpv6SocketAddress, sizeof(rtIpv6SocketAddress));
      rtIpv6SocketAddress.sin6_port = htons((RT_UN16)unPort);
      rtIpv6SocketAddress.sin6_addr = in6addr_any;
      nAddressSize = sizeof(rtIpv6SocketAddress);
      lpSocketAddress = (struct sockaddr*)&rtIpv6SocketAddress;
      break;
    default:
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
  }
  lpSocketAddress->sa_family = lpSocket->unAddressFamily;

#ifdef RT_DEFINE_WINDOWS
  /* On success bind returns zero, -1 and set last error otherwise. */
  bResult = !bind(lpSocket->unSocket, lpSocketAddress, nAddressSize);
#else
  /* On success bind returns zero, -1 and set last error otherwise. */
  bResult = !bind(lpSocket->nSocket, lpSocketAddress, nAddressSize);
#endif

free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_API RtListenFromSocket(RT_SOCKET* lpSocket)
{
  return RtListenFromSocketWithBackLog(lpSocket, SOMAXCONN);
}

RT_B RT_API RtListenFromSocketWithBackLog(RT_SOCKET* lpSocket, RT_N nBacklog)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* On success listen returns zero, -1 and set last error otherwise. */
  bResult = !listen(lpSocket->unSocket, nBacklog);
#else
  /* On success listen returns zero, -1 and set last error otherwise. */
  bResult = !listen(lpSocket->nSocket, nBacklog);
#endif

  return bResult;
}

#ifdef RT_DEFINE_LINUX

/**
 * Translate win32ex socket message flags into Linux flags.
 */
RT_N RT_CALL RtComputeSocketMessageFlags(RT_N nFlags)
{
  RT_N nResult;

  nResult = 0;
  if (nFlags & RT_SOCKET_MESSAGE_OUT_OF_BAND) nResult |= MSG_OOB;
  if (nFlags & RT_SOCKET_MESSAGE_PEEK) nResult |= MSG_PEEK;
  if (nFlags & RT_SOCKET_MESSAGE_DO_NOT_ROUTE) nResult |= MSG_DONTROUTE;
  if (nFlags & RT_SOCKET_MESSAGE_WAIT_ALL) nResult |= MSG_WAITALL;

  return nResult;
}
#endif

RT_N RT_API RtSendThroughSocket(RT_SOCKET* lpSocket, void* lpData, RT_N nDataSize, RT_N nFlags)
{
  RT_N nActualFlags;
  RT_N nResult;

#ifdef RT_DEFINE_WINDOWS
  nActualFlags = nFlags;
#else
  nActualFlags = RtComputeSocketMessageFlags(nFlags);
#endif

#ifdef RT_DEFINE_WINDOWS
  nResult = send(lpSocket->unSocket, lpData, nDataSize, nActualFlags);
#else
  nResult = send(lpSocket->nSocket, lpData, nDataSize, nActualFlags);
#endif

  return nResult;
}

RT_N RT_API RtReceiveFromSocket(RT_SOCKET* lpSocket, void* lpBuffer, RT_N nBufferSize, RT_N nFlags)
{
  RT_N nActualFlags;
  RT_N nResult;

#ifdef RT_DEFINE_WINDOWS
  nActualFlags = nFlags;
#else
  nActualFlags = RtComputeSocketMessageFlags(nFlags);
#endif

#ifdef RT_DEFINE_WINDOWS
  nResult = recv(lpSocket->unSocket, lpBuffer, nBufferSize, nActualFlags);
#else
  nResult = recv(lpSocket->nSocket, lpBuffer, nBufferSize, nActualFlags);
#endif

  return nResult;
}

RT_B RT_API RtAcceptSocketConnection(RT_SOCKET* lpSocket, RT_SOCKET* lpAcceptedSocket, RT_SOCKET_ADDRESS* lpSocketAddress)
{
#ifdef RT_DEFINE_WINDOWS
  int nSocketAddressSize;
#else
  socklen_t nSocketAddressSize;
#endif
  RT_SOCKET_ADDRESS rtSocketAddress;
  RT_SOCKET_ADDRESS* lpActualSocketAddress;
  RT_B bResult;

  if (lpSocketAddress)
  {
    RtZeroMemory(lpSocketAddress, sizeof(RT_SOCKET_ADDRESS));
    lpActualSocketAddress = lpSocketAddress;
  }
  else
  {
    lpActualSocketAddress = &rtSocketAddress;
  }

  nSocketAddressSize = sizeof(RT_SOCKET_ADDRESS);

#ifdef RT_DEFINE_WINDOWS
  lpAcceptedSocket->unSocket = accept(lpSocket->unSocket, (struct sockaddr*)lpActualSocketAddress, &nSocketAddressSize);
  if (lpAcceptedSocket->unSocket == INVALID_SOCKET) goto handle_error;
#else
  lpAcceptedSocket->nSocket = accept(lpSocket->nSocket, (struct sockaddr*)lpActualSocketAddress, &nSocketAddressSize);
  if (lpAcceptedSocket->nSocket == -1) goto handle_error;
#endif

  lpAcceptedSocket->unAddressFamily = lpActualSocketAddress->unAddressFamily;

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_API RtShutdownSocket(RT_SOCKET* lpSocket, RT_N nFlag)
{
  RT_B bResult;

  /* On success shutdown returns zero, -1 and set last error otherwise. */
#ifdef RT_DEFINE_WINDOWS
  bResult = !shutdown(lpSocket->unSocket, nFlag);
#else
  bResult = !shutdown(lpSocket->nSocket, nFlag);
#endif

  return bResult;
}

RT_B RT_API RtFreeSocket(RT_SOCKET* lpSocket)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* closesocket returns zero if the operation was successful, call WSASetLastError and returns SOCKET_ERROR otherwise. */
  bResult = !closesocket(lpSocket->unSocket);
#else
  /* close() returns zero on success. */
  bResult = !close(lpSocket->nSocket);
#endif
  return bResult;
}

RT_B RT_API RtCleanUpSockets()
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* WSACleanup returns zero if the operation was successful, call WSASetLastError and returns SOCKET_ERROR otherwise. */
  bResult = !WSACleanup();
#else
  bResult = RT_TRUE;
#endif
  return bResult;
}