#include "layer007/RtSocket.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer003/RtMemory.h"
#include "layer005/RtSystemInfo.h"
#include "layer006/RtFile.h"


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
  bResult = RT_SUCCESS;

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
  bResult = RT_FAILURE;
  goto free_resources;
#endif
}

RT_B RT_API RtCreateSocket(RT_SOCKET* lpSocket, RT_UN unAddressFamily, RT_UN unType, RT_UN unProtocol, RT_B bBlocking, RT_B bInheritable)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unFlags;
  RT_B bFlagNoHandleInherit;
  RT_FILE rtSocket;
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

  if (!bInheritable)
  {
    /* Flag required only to set handle non-inheritable.  */
    /* Socket handles are inheritable by default. */

    /* WSA_FLAG_NO_HANDLE_INHERIT is supported on Windows 7 with SP1, Windows Server 2008 R2 with SP1, and later. */
    if (!RtIsOsVersionGreaterOrEqualTo(6, 1, 1, &bFlagNoHandleInherit)) goto handle_error;
    if (bFlagNoHandleInherit)
    {
      unFlags |= 0x80; /* WSA_FLAG_NO_HANDLE_INHERIT */
    }
  }

  /* WSA_FLAG_NO_HANDLE_INHERIT flag is in early versions of Windows only. */
  lpSocket->unSocket = (RT_UN)WSASocket((int)unAddressFamily, (int)unType, (int)unProtocol, RT_NULL, 0, unFlags);
  if (lpSocket->unSocket == INVALID_SOCKET) goto handle_error;

  if (!bInheritable && !bFlagNoHandleInherit)
  {
    /* WSA_FLAG_NO_HANDLE_INHERIT is not supported. */
    /* We manually set it to non-inheritable. */
    /* There is a race condidition here, the handle could leak if CreateProcess is called in parallel. */
    rtSocket.hFile = (RT_H)lpSocket->unSocket;
    if (!RtSetFileInheritable(&rtSocket, RT_FALSE))
    {
      RtFreeSocket(lpSocket);
      goto handle_error;
    }
  }

#else /* NOT RT_DEFINE_WINDOWS */

  /* Add needed flags with bitwise OR in socket type. */
  unActualType = unType;
  if (!bBlocking)
  {
    unActualType |= SOCK_NONBLOCK;
  }
  if (!bInheritable)
  {
    unActualType |= SOCK_CLOEXEC;
  }

  lpSocket->nSocket = socket(unAddressFamily, unActualType, unProtocol);
  if (lpSocket->nSocket == -1) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

void RT_API RtCreateIpv4LoopbackAddress(RT_ADDRESS_IPV4* lpAddress)
{
  lpAddress->rtUnion.unAddress = htonl(INADDR_LOOPBACK);
}

void RT_API RtCreateIpv4SocketAddress(RT_SOCKET_ADDRESS_IPV4* lpSocketAddress, RT_ADDRESS_IPV4* lpAddress, RT_UN unPort)
{
  RT_MEMORY_ZERO(lpSocketAddress, sizeof(RT_SOCKET_ADDRESS_IPV4));
  lpSocketAddress->unAddressFamily = RT_SOCKET_ADDRESS_FAMILY_IPV4;
  lpSocketAddress->unPort = htons((RT_UN16)unPort);
  RT_MEMORY_COPY(lpAddress, &lpSocketAddress->rtAddress, sizeof(RT_ADDRESS_IPV4));
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
      RT_MEMORY_ZERO(&rtIpv4SocketAddress, sizeof(RT_SOCKET_ADDRESS_IPV4));
      rtIpv4SocketAddress.unPort = htons((RT_UN16)unPort);
      RT_MEMORY_COPY(lpAddress, &rtIpv4SocketAddress.rtAddress, sizeof(RT_ADDRESS_IPV4));
      lpSocketAddress = (RT_SOCKET_ADDRESS*)&rtIpv4SocketAddress;
      break;
    case RT_SOCKET_ADDRESS_FAMILY_IPV6:
      RT_MEMORY_ZERO(&rtIpv6SocketAddress, sizeof(RT_SOCKET_ADDRESS_IPV6));
      rtIpv4SocketAddress.unPort = htons((RT_UN16)unPort);
      RT_MEMORY_COPY(lpAddress, &rtIpv6SocketAddress.rtAddress, sizeof(RT_ADDRESS_IPV6));
      lpSocketAddress = (RT_SOCKET_ADDRESS*)&rtIpv6SocketAddress;
      break;
    default:
      RtSetLastError(RT_ERROR_FUNCTION_FAILED);
      goto handle_error;
  }
  lpSocketAddress->unAddressFamily = (RT_UN16)unAddressFamily;

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
  bResult = RT_FAILURE;
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

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtBindSocket(RT_SOCKET* lpSocket, RT_UN unPort)
{
  struct sockaddr_in rtIpv4SocketAddress;
  struct sockaddr_in6 rtIpv6SocketAddress;
  struct sockaddr* lpSocketAddress;
  RT_N32 nAddressSize;
  RT_B bResult;

  switch (lpSocket->unAddressFamily)
  {
    case RT_SOCKET_ADDRESS_FAMILY_IPV4:
      RT_MEMORY_ZERO(&rtIpv4SocketAddress, sizeof(rtIpv4SocketAddress));
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
      RT_MEMORY_ZERO(&rtIpv6SocketAddress, sizeof(rtIpv6SocketAddress));
      rtIpv6SocketAddress.sin6_port = htons((RT_UN16)unPort);
      rtIpv6SocketAddress.sin6_addr = in6addr_any;
      nAddressSize = sizeof(rtIpv6SocketAddress);
      lpSocketAddress = (struct sockaddr*)&rtIpv6SocketAddress;
      break;
    default:
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
  }
  lpSocketAddress->sa_family = (RT_UN16)lpSocket->unAddressFamily;

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
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtListenFromSocket(RT_SOCKET* lpSocket)
{
  return RtListenFromSocketWithBackLog(lpSocket, SOMAXCONN);
}

RT_B RT_API RtListenFromSocketWithBackLog(RT_SOCKET* lpSocket, RT_N32 nBacklog)
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
RT_UN RT_CALL RtComputeSocketMessageFlags(RT_UN unFlags)
{
  RT_UN unResult;

  unResult = 0;
  if (unFlags & RT_SOCKET_MESSAGE_OUT_OF_BAND) unResult |= MSG_OOB;
  if (unFlags & RT_SOCKET_MESSAGE_PEEK) unResult |= MSG_PEEK;
  if (unFlags & RT_SOCKET_MESSAGE_DO_NOT_ROUTE) unResult |= MSG_DONTROUTE;
  if (unFlags & RT_SOCKET_MESSAGE_WAIT_ALL) unResult |= MSG_WAITALL;

  return unResult;
}
#endif

RT_UN RT_API RtSendThroughSocket(RT_SOCKET* lpSocket, void* lpData, RT_UN unDataSize, RT_UN unFlags)
{
  RT_UN unActualFlags;
  int nSendResult;
  RT_UN unResult;

#ifdef RT_DEFINE_WINDOWS
  unActualFlags = unFlags;
#else
  unActualFlags = RtComputeSocketMessageFlags(unFlags);
#endif

#ifdef RT_DEFINE_WINDOWS
  /* Returns SOCKET_ERROR (-1) and set WSAGetLastError in case of issue. */
  nSendResult = send(lpSocket->unSocket, lpData, (int)unDataSize, (int)unActualFlags);
#else
  /* Returns -1 in case of issue and set errno. */
  nSendResult = send(lpSocket->nSocket, lpData, unDataSize, unActualFlags);
#endif

  if (nSendResult < 0)
  {
    unResult = RT_TYPE_MAX_UN;
  }
  else
  {
    unResult = nSendResult;
  }
  return unResult;
}

RT_UN RT_API RtReceiveFromSocket(RT_SOCKET* lpSocket, void* lpBuffer, RT_UN unBufferSize, RT_UN unFlags)
{
  RT_UN unActualFlags;
  int nRecvResult;
  RT_UN unResult;

#ifdef RT_DEFINE_WINDOWS
  unActualFlags = unFlags;
#else
  unActualFlags = RtComputeSocketMessageFlags(unFlags);
#endif

#ifdef RT_DEFINE_WINDOWS
  /* Returns SOCKET_ERROR (-1) and set WSAGetLastError in case of issue. */
  nRecvResult = recv(lpSocket->unSocket, lpBuffer, (int)unBufferSize, (int)unActualFlags);
#else
  /* Returns -1 in case of issue and set errno. */
  nRecvResult = recv(lpSocket->nSocket, lpBuffer, unBufferSize, unActualFlags);
#endif

  if (nRecvResult < 0)
  {
    unResult = RT_TYPE_MAX_UN;
  }
  else
  {
    unResult = nRecvResult;
  }
  return unResult;
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
    RT_MEMORY_ZERO(lpSocketAddress, sizeof(RT_SOCKET_ADDRESS));
    lpActualSocketAddress = lpSocketAddress;
  }
  else
  {
    lpActualSocketAddress = &rtSocketAddress;
  }

  nSocketAddressSize = sizeof(RT_SOCKET_ADDRESS);

#ifdef RT_DEFINE_WINDOWS
  lpAcceptedSocket->unSocket = (RT_UN)accept(lpSocket->unSocket, (struct sockaddr*)lpActualSocketAddress, &nSocketAddressSize);
  if (lpAcceptedSocket->unSocket == INVALID_SOCKET) goto handle_error;
#else
  lpAcceptedSocket->nSocket = accept(lpSocket->nSocket, (struct sockaddr*)lpActualSocketAddress, &nSocketAddressSize);
  if (lpAcceptedSocket->nSocket == -1) goto handle_error;
#endif

  lpAcceptedSocket->unAddressFamily = lpActualSocketAddress->unAddressFamily;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtShutdownSocket(RT_SOCKET* lpSocket, RT_UN unFlag)
{
  RT_B bResult;

  /* On success shutdown returns zero, -1 and set last error otherwise. */
#ifdef RT_DEFINE_WINDOWS
  bResult = !shutdown(lpSocket->unSocket, (int)unFlag);
#else
  bResult = !shutdown(lpSocket->nSocket, unFlag);
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
  bResult = RT_SUCCESS;
#endif
  return bResult;
}
