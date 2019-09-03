#ifndef RT_SOCKET_H
#define RT_SOCKET_H

#include "layer000/RtWin32ExTypes.h"
#include "layer002/RtIoDevice.h"

typedef struct _RT_SOCKET
{
#ifdef RT_DEFINE_WINDOWS
  RT_UN unSocket;
#else
  RT_N32 nSocket;
#endif
  RT_UN unAddressFamily;
  RT_B bBlocking;
}
RT_SOCKET;

/**
 * IP v4 address (in_addr).
 */
typedef struct _RT_ADDRESS_IPV4 {
  union {
    struct {
      RT_UCHAR8 uc1;
      RT_UCHAR8 uc2;
      RT_UCHAR8 uc3;
      RT_UCHAR8 uc4;
    } rtB;
    struct {
      RT_UN16 un1;
      RT_UN16 un2;
    } rtW;
    RT_UN32 unAddress;
  } rtUnion;
} RT_ADDRESS_IPV4;

/**
 * IP v6 address (in_addr6).
 */
typedef struct _RT_ADDRESS_IPV6 {
  union {
    RT_UCHAR8 lp8[16];
    RT_UN16 lp16[8];
    RT_UN32 lp32[4];
  } rtUnion;
} RT_ADDRESS_IPV6;

/**
 * Parent socket address structure (sockaddr).<br>
 * Underlying structure can be an RT_SOCKET_ADDRESS_IPV4 or an RT_SOCKET_ADDRESS_FAMILY_IPV6.
 *
 * <p>
 * Unlike sockaddr which has the size of RT_SOCKET_ADDRESS_IPV4 (16 bytes), RT_SOCKET_ADDRESS has the size of RT_SOCKET_ADDRESS_IPV6 (28 bytes).
 * </p>
 */
typedef struct _RT_SOCKET_ADDRESS {
  RT_UN16 unAddressFamily;
  RT_CHAR8 cData[26];
}
RT_SOCKET_ADDRESS;

/**
 * RT_SOCKET_ADDRESS for IP v4 (sockaddr_in).
 */
typedef struct _RT_SOCKET_ADDRESS_IPV4 {
  RT_UN16 unAddressFamily;
  RT_UN16 unPort;
  RT_ADDRESS_IPV4 rtAddress;
  RT_CHAR8 cZero[8];
}
RT_SOCKET_ADDRESS_IPV4;

/**
 * RT_SOCKET_ADDRESS for IP v6 (sockaddr_in6).
 */
typedef struct _RT_SOCKET_ADDRESS_IPV6 {
  RT_UN16 unAddressFamily;
  RT_UN16 unPort;
  RT_UN32 unFlowInfo;
  RT_ADDRESS_IPV6 rtAddress;
  RT_UN32 unScopeId;
}
RT_SOCKET_ADDRESS_IPV6;

/* Socket address family (AF_APPLETALK and AF_IPX are the only supported by Windows and Linux). */
/* Used as <tt>unAddressFamily</tt> parameter of <tt>RtSocket_Create</tt>. */
/* Values are correct on both Windows and Linux using macros. */
/* IP v4. */
#define RT_SOCKET_ADDRESS_FAMILY_IPV4 2
/* IP v6. */
#ifdef RT_DEFINE_WINDOWS
  #define RT_SOCKET_ADDRESS_FAMILY_IPV6 23
#else
  #define RT_SOCKET_ADDRESS_FAMILY_IPV6 10
#endif

/* Transport protocol layer type, mostly tcp/udp. */
/* Used as <tt>unType</tt> parameter of <tt>RtSocket_Create</tt>. */
/* Values are correct on both Windows and Linux. */
/* Mostly TCP. */
#define RT_SOCKET_TYPE_STREAM 1
/* Mostly UDP. */
#define RT_SOCKET_TYPE_DGRAM 2
#define RT_SOCKET_TYPE_RAW 3
#define RT_SOCKET_TYPE_RDM 4
#define RT_SOCKET_TYPE_SEQPACKET 5

/* Allows to precise protocol. */
/* Used as optional <tt>unType</tt> parameter of <tt>RtSocket_Create</tt>. */
/* Values are correct on both Windows and Linux (Except for RT_SOCKET_PROTOCOL_SOCKET). */
/* Dummy protocol for TCP.             */
#define RT_SOCKET_PROTOCOL_IP 0
/* Internet Control Message Protocol.  */
#define RT_SOCKET_PROTOCOL_ICMP 1
/* Internet Group Management Protocol. */
#define RT_SOCKET_PROTOCOL_IGMP 2
/* TCP Transmission Control Protocol.  */
#define RT_SOCKET_PROTOCOL_TCP 6
/* PUP protocol.                       */
#define RT_SOCKET_PROTOCOL_PUP 12
/* UDP, User Datagram Protocol.        */
#define RT_SOCKET_PROTOCOL_UDP 17
/* XNS IDP protocol.                   */
#define RT_SOCKET_PROTOCOL_IDP 22
/* Raw IP packets.                     */
#define RT_SOCKET_PROTOCOL_RAW 255
/* IPv6 Hop-by-Hop options.            */
#define RT_SOCKET_PROTOCOL_HOPOPTS 0
/* IPv6-in-IPv4 tunneling.             */
#define RT_SOCKET_PROTOCOL_IPV6 41
/* IPv6 routing header                 */
#define RT_SOCKET_PROTOCOL_ROUTING 43
/* IPv6 fragmentation header.          */
#define RT_SOCKET_PROTOCOL_FRAGMENT 44
/* encapsulating security payload.     */
#define RT_SOCKET_PROTOCOL_ESP 50
/* Authentication Header protocol.     */
#define RT_SOCKET_PROTOCOL_AH 51
/* ICMPv6.                             */
#define RT_SOCKET_PROTOCOL_ICMPV6 58
/* IPv6 no next header.                */
#define RT_SOCKET_PROTOCOL_NONE 59
/* IPv6 Destination options.           */
#define RT_SOCKET_PROTOCOL_DSTOPTS 60
/* Classical socket protocol level (SOL_SOCKET) used only for RtSocket_SetBooleanOption/RtSocket_SetOption. */
#define RT_SOCKET_PROTOCOL_SOCKET 65535

/* Boolean socket option used in RtSocket_SetBooleanOption/RtSocket_SetOption. */
/* Values are correct on Windows only. */
/* Allows the socket to be bound to an address that is already in use. Can be use to quickly reuse a port in TIME_WAIT state. */
#define RT_SOCKET_OPTION_REUSEADDR 4

/* Sending and receiving flags. */
/* Values are correct on Windows only. */
/* Process out-of-band data. */
#define RT_SOCKET_MESSAGE_OUT_OF_BAND 1
/* Peek at incoming message. When receiving data only. */
#define RT_SOCKET_MESSAGE_PEEK 2
/* Send without using routing tables. When sending data only. */
#define RT_SOCKET_MESSAGE_DO_NOT_ROUTE 4
/* Do not complete until buffer is completely filled or remote socket is closed. When receiving data only. */
#define RT_SOCKET_MESSAGE_WAIT_ALL 8

/* RtSocket_Shutdown flags. */
/* Values are correct on both Windows and Linux. */
/* Shutdown receive operations. */
#define RT_SOCKET_SHUTDOWN_RECEIVE 0
/* Shutdown send operations. */
#define RT_SOCKET_SHUTDOWN_SEND 1
/* Shutdown both send and receive operations. */
#define RT_SOCKET_SHUTDOWN_BOTH 2

/**
 * Must be called before any other socket functions.<br />
 * Call WSAStartup under Windows and does nothing under unix.
 *
 * <p>
 * Call RtSocket_CleanUp after a successful call to RtSocket_Initialize when you finished using socket functions.
 * </p>
 *
 * @return RT_FALSE and set last error in case of issue.
 */
RT_B RT_API RtSocket_Initialize();

/**
 * Create a socket.
 *
 * <p>
 * If <tt>unProtocol</tt> is zero, the protocol is deduced from <tt>unAddressFamily</tt> and <tt>unType</tt>.<br>
 * <tt>RT_SOCKET_ADDRESS_FAMILY_IPV4<tt> and <tt>RT_SOCKET_TYPE_STREAM</tt> mostly means TCP.
 * <tt>RT_SOCKET_ADDRESS_FAMILY_IPV6<tt> and <tt>RT_SOCKET_TYPE_DGRAM</tt> mostly means UDP.
 * </p>
 *
 * <p>
 * Under Windows, an inheritable handle can be used by a process created with CreateProcess and bInheritHandles TRUE.<br>
 * Under Linux, a non-inheritable file descriptor is closed when execl/execlp/execle/execv/execvp/execvpe is used.<br>
 * It is not closed by a fork.
 * </p>
 *
 * @param unAddressFamily RT_SOCKET_ADDRESS_FAMILY_XXX value, mostly IP v4 or IP v6.
 * @param unType Transport layer type, mostly TCP or UDP.
 * @param bInheritable Whether the socket could be used in a child process. Beware of handles/file descriptors leaks.
 * @param unProtocol Often zero but can be a RT_SOCKET_PROTOCOL_XXX value.
 */
RT_B RT_API RtSocket_Create(RT_SOCKET* lpSocket, RT_UN unAddressFamily, RT_UN unType, RT_UN unProtocol, RT_B bBlocking, RT_B bInheritable);

/**
 * Set a socket boolean option.
 *
 * @param unProtocolLevel Level of the option of the protocol. A classical value is RT_SOCKET_PROTOCOL_SOCKET.
 * @param unOption The option to set like RT_SOCKET_OPTION_REUSEADDR.
 * @param bValue The value to set.
 */
RT_B RT_API RtSocket_SetBooleanOption(RT_SOCKET* lpSocket, RT_UN unProtocolLevel, RT_UN unOption, RT_B bValue);

/**
 * Set a socket option.
 *
 * <p>
 * For boolean values, <tt>RtSocket_SetBooleanOption</tt> is a bit simpler to use.
 * </p>
 *
 * @param unProtocolLevel Level of the option of the protocol. A classical value is RT_SOCKET_PROTOCOL_SOCKET.
 * @param unOption The option to set like RT_SOCKET_OPTION_REUSEADDR.
 * @param lpValue Pointer to the new option value.
 * @param unValueSize Size of the value pointed by <tt>lpValue</tt>.
 */
RT_B RT_API RtSocket_SetOption(RT_SOCKET* lpSocket, RT_UN unProtocolLevel, RT_UN unOption, void* lpValue, RT_UN unValueSize);

void RT_API RtSocket_CreateIpv4LoopbackAddress(RT_ADDRESS_IPV4* lpAddress);

void RT_API RtSocket_CreateIpv4Address(RT_SOCKET_ADDRESS_IPV4* lpSocketAddress, RT_ADDRESS_IPV4* lpAddress, RT_UN unPort);

RT_B RT_API RtSocket_Connect(RT_SOCKET* lpSocket, RT_CHAR* lpHostName, RT_UN unPort);

/**
 * <p>
 * In the case of a non-blocking socket, most of the time <tt>RT_FAILURE</tt> is returned.<br>
 * You then have to use <tt>RtError_WouldBlock</tt> to determine if the error is a failure or if the connection is in progress.<br>
 * You can use <tt>RtSocket_WaitForNonBlockingConnection</tt> to wait for the connection to be writable.
 * </p>
 */
RT_B RT_API RtSocket_ConnectWithAddress(RT_SOCKET* lpSocket, RT_SOCKET_ADDRESS* lpSocketAddress);

/**
 * Can be used after <tt>RtSocket_ConnectWithAddress</tt> in the case of a non-blocking socket.
 */
RT_B RT_API RtSocket_WaitForNonBlockingConnection(RT_SOCKET* lpSocket);

RT_B RT_API RtSocket_Bind(RT_SOCKET* lpSocket, RT_UN unPort);

RT_B RT_API RtSocket_Listen(RT_SOCKET* lpSocket);

/**
 * @param nBacklog Defined as int on both Windows and Linux.
 */
RT_B RT_API RtSocket_ListenWithBackLog(RT_SOCKET* lpSocket, RT_N32 nBacklog);

/**
 * @param lpSocketAddress If not null, will be returned with the address of the connecting entity.
 */
RT_B RT_API RtSocket_AcceptConnection(RT_SOCKET* lpSocket, RT_SOCKET* lpAcceptedSocket, RT_SOCKET_ADDRESS* lpSocketAddress);

/**
 *
 * @param nFlags Combination of RT_SOCKET_MESSAGE_XXXX flags.
 * @return Number of bytes sent, RT_TYPE_MAX_UN in case of issue.
 */
RT_UN RT_API RtSocket_Send(RT_SOCKET* lpSocket, void* lpData, RT_UN unDataSize, RT_UN unFlags);

/**
 *
 * @param unFlags Combination of RT_SOCKET_MESSAGE_XXXX flags. RT_SOCKET_MESSAGE_WAIT_ALL is a classical one.
 * @return Number of bytes received, RT_TYPE_MAX_UN in case of issue.
 */
RT_UN RT_API RtSocket_Receive(RT_SOCKET* lpSocket, void* lpBuffer, RT_UN unBufferSize, RT_UN unFlags);

/**
 * RtSocket_Receive can receive incomplete response and RT_SOCKET_MESSAGE_WAIT_ALL can be blocking if the remote host does not close the connection.<br>
 * This function is a workaround for this.<br>
 * It ensures that either the buffer is full either the connection is closed.
 */
RT_UN RT_API RtSocket_ReceiveAll(RT_SOCKET* lpSocket, void* lpBuffer, RT_UN unBufferSize);

/**
 * @param unFlags One of RT_SOCKET_SHUTDOWN_XXXX, mostly RT_SOCKET_SHUTDOWN_BOTH.
 */
RT_B RT_API RtSocket_Shutdown(RT_SOCKET* lpSocket, RT_UN unFlag);

RT_B RT_API RtSocket_Free(RT_SOCKET* lpSocket);

/**
 * Must be called after a successful call to RtSocket_Initialize and after all socket functions calls.
 * Call WSACleanup under Windows and does nothing under unix.
 *
 * @return RT_FALSE and set last error in case of issue.
 */
RT_B RT_API RtSocket_CleanUp();

#endif /* RT_SOCKET_H */
