#ifndef RT_SOCKET_H
#define RT_SOCKET_H

#include "layer000/rt_types.h"

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

/* rt_socket_shutdown flags. */
/* Values are correct on both Windows and Linux. */
/* Shutdown receive operations. */
#define RT_SOCKET_SHUTDOWN_RECEIVE 0
/* Shutdown send operations. */
#define RT_SOCKET_SHUTDOWN_SEND 1
/* Shutdown both send and receive operations. */
#define RT_SOCKET_SHUTDOWN_BOTH 2

/**
 * @param socket_address If not null, will be returned with the address of the connecting entity.
 */
rt_s rt_socket_accept_connection(struct rt_socket *socket, struct rt_socket *accepted_socket, struct rt_socket_address *socket_address);

/**
 *
 * @param flags Combination of RT_SOCKET_MESSAGE_XXXX flags.
 * @return Number of bytes sent, RT_TYPE_MAX_UN in case of issue.
 */
rt_un rt_socket_send(struct rt_socket *socket, void *data, rt_un data_size, rt_un flags);

/**
 *
 * @param flags Combination of RT_SOCKET_MESSAGE_XXXX flags. RT_SOCKET_MESSAGE_WAIT_ALL is a classical one.
 * @return Number of bytes received, RT_TYPE_MAX_UN in case of issue.
 */
rt_un rt_socket_receive(struct rt_socket *socket, void *buffer, rt_un buffer_capacity, rt_un flags);

/**
 * rt_socket_receive can receive incomplete response and RT_SOCKET_MESSAGE_WAIT_ALL can be blocking if the remote host does not close the connection.<br>
 * This function is a workaround for this.<br>
 * It ensures that either the buffer is full either the connection is closed.
 */
rt_un rt_socket_receive_all(struct rt_socket *socket, void *buffer, rt_un buffer_capacity);

/**
 * @param flags One of RT_SOCKET_SHUTDOWN_XXXX, mostly RT_SOCKET_SHUTDOWN_BOTH.
 */
rt_s rt_socket_shutdown(struct rt_socket *socket, rt_un flag);

#endif /* RT_SOCKET_H */
