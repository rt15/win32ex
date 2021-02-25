#include "layer006/rt_socket.h"


/**
 * @file
 *
 * <p>
 * Under Windows currently WSAGetLastError is the same as GetLastError.
 * </p>
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

#ifdef RT_DEFINE_LINUX

/**
 * Translate win32ex socket message flags into Linux flags.
 */
static rt_un rt_socket_get_message_flags(rt_un flags)
{
	rt_un result;

	result = 0;
	if (flags & RT_SOCKET_MESSAGE_OUT_OF_BAND) result |= MSG_OOB;
	if (flags & RT_SOCKET_MESSAGE_PEEK) result |= MSG_PEEK;
	if (flags & RT_SOCKET_MESSAGE_DO_NOT_ROUTE) result |= MSG_DONTROUTE;
	if (flags & RT_SOCKET_MESSAGE_WAIT_ALL) result |= MSG_WAITALL;

	return result;
}
#endif

rt_un rt_socket_send(struct rt_socket *socket, void *data, rt_un data_size, rt_un flags)
{
	rt_un actual_flags;
	int send_result;
	rt_un result;

#ifdef RT_DEFINE_WINDOWS
	actual_flags = flags;
#else
	actual_flags = rt_socket_get_message_flags(flags);
#endif

#ifdef RT_DEFINE_WINDOWS
	/* Returns SOCKET_ERROR (-1) and set WSAGetLastError in case of issue. */
	send_result = send(socket->socket_handle, data, (int)data_size, (int)actual_flags);
#else
	/* Returns -1 in case of issue and set errno. */
	send_result = send(socket->socket_file_descriptor, data, data_size, actual_flags);
#endif

	if (send_result < 0) {
		result = RT_TYPE_MAX_UN;
	} else {
		result = send_result;
	}
	return result;
}

rt_un rt_socket_receive(struct rt_socket *socket, void *buffer, rt_un buffer_capacity, rt_un flags)
{
	rt_un actual_flags;
	int recv_result;
	rt_un result;

#ifdef RT_DEFINE_WINDOWS
	actual_flags = flags;
#else
	actual_flags = rt_socket_get_message_flags(flags);
#endif

#ifdef RT_DEFINE_WINDOWS
	/* Returns SOCKET_ERROR (-1) and set WSAGetLastError in case of issue. */
	recv_result = recv(socket->socket_handle, buffer, (int)buffer_capacity, (int)actual_flags);
#else
	/* Returns -1 in case of issue and set errno. */
	recv_result = recv(socket->socket_file_descriptor, buffer, buffer_capacity, actual_flags);
#endif

	if (recv_result < 0) {
		result = RT_TYPE_MAX_UN;
	} else {
		result = recv_result;
	}
	return result;
}

rt_un rt_socket_receive_all(struct rt_socket *socket, void *buffer, rt_un buffer_capacity)
{
	rt_un result;
	rt_un received;

	result = 0;

	while (RT_TRUE) {
		received = rt_socket_receive(socket, &((rt_char8*)buffer)[result], buffer_capacity - result, 0);
		if (received == RT_TYPE_MAX_UN)
			goto error;

		if (!received) {
			/* Nothing more to read, return. */
			break;
		}

		result += received;

		if (result == buffer_capacity) {
			/* Buffer is full, return. */
			break;
		}
	}

free:
	return result;

error:
	result = RT_TYPE_MAX_UN;
	goto free;
}

rt_s rt_socket_accept_connection(struct rt_socket *socket, struct rt_socket *accepted_socket, struct rt_socket_address *socket_address)
{
#ifdef RT_DEFINE_WINDOWS
	int socket_address_size;
#else
	socklen_t socket_address_size;
#endif
	struct rt_socket_address socket_address;
	struct rt_socket_address *actual_socket_address;
	rt_s ret;

	if (socket_address) {
		RT_MEMORY_ZERO(socket_address, sizeof(struct rt_socket_address));
		actual_socket_address = socket_address;
	} else {
		actual_socket_address = &socket_address;
	}

	socket_address_size = sizeof(struct rt_socket_address);

#ifdef RT_DEFINE_WINDOWS
	accepted_socket->socket_handle = (rt_un)accept(socket->socket_handle, (struct sockaddr*)actual_socket_address, &socket_address_size);
	if (accepted_socket->socket_handle == INVALID_SOCKET)
		goto error;
#else
	accepted_socket->socket_file_descriptor = accept(socket->socket_file_descriptor, (struct sockaddr*)actual_socket_address, &socket_address_size);
	if (accepted_socket->socket_file_descriptor == -1)
		goto error;
#endif

	accepted_socket->address_family = actual_socket_address->address_family;

	ret = RT_OK;
free:
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

rt_s rt_socket_shutdown(struct rt_socket *socket, rt_un flag)
{
	rt_s ret;

	/* On success shutdown returns zero, -1 and set last error otherwise. */
#ifdef RT_DEFINE_WINDOWS
	ret = !shutdown(socket->socket_handle, (int)flag);
#else
	ret = !shutdown(socket->socket_file_descriptor, flag);
#endif

	return ret;
}
