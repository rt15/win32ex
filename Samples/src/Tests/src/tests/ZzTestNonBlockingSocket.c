#include <rpr.h>

#include "ZzTools.h"

#define ZZ_PORT_NUMBER 42334

/**
 * Thread that create a server socket and send "Hello, world!" to the first connecting socket.
 */
rt_un32 RT_CALL ZzServerNonBlockingSocketThreadCallback(void *parameter)
{
	struct rt_socket zzSocket;
	struct rt_socket zzAcceptedSocket;
	struct rt_socket_address zzSocketAddress;
	rt_b bSocketCreated;
	rt_b bAcceptedSocketCreated;
	struct rt_event *event;
	rt_char8 *message;
	rt_un32 result;

	bSocketCreated = RT_FALSE;
	bAcceptedSocketCreated = RT_FALSE;

	event = (struct rt_event*)parameter;

	if (!rt_socket_create(&zzSocket, RT_SOCKET_ADDRESS_FAMILY_IPV4, RT_SOCKET_TYPE_STREAM, RT_SOCKET_PROTOCOL_TCP, RT_TRUE, RT_FALSE)) goto error;
	bSocketCreated = RT_TRUE;

	if (!rt_socket_set_boolean_option(&zzSocket, RT_SOCKET_PROTOCOL_SOCKET, RT_SOCKET_OPTION_REUSEADDR, RT_TRUE)) goto error;

	if (!rt_socket_bind(&zzSocket, ZZ_PORT_NUMBER)) goto error;
	if (!rt_socket_listen(&zzSocket)) goto error;

	if (!rt_event_signal(event)) goto error;

	if (!rt_socket_accept_connection(&zzSocket, &zzAcceptedSocket, &zzSocketAddress)) goto error;
	bAcceptedSocketCreated = RT_TRUE;

	message = "Hello";
	if (rt_socket_send(&zzAcceptedSocket, message, rt_char8_get_size(message), 0) == RT_TYPE_MAX_UN) goto error;

	/* Uncomment next line to separate responses. */
	/* rt_sleep_sleep(1000); */

	message = ", world!";
	if (rt_socket_send(&zzAcceptedSocket, message, rt_char8_get_size(message) + 1, 0) == RT_TYPE_MAX_UN) goto error;

	if (!rt_socket_shutdown(&zzAcceptedSocket, RT_SOCKET_SHUTDOWN_BOTH)) goto error;

	result = RT_OK;
free:
	if (bAcceptedSocketCreated) {
		bAcceptedSocketCreated = RT_FALSE;
		if (!rt_socket_free(&zzAcceptedSocket) && result) goto error;
	}
	if (bSocketCreated) {
		bSocketCreated = RT_FALSE;
		if (!rt_socket_free(&zzSocket) && result) goto error;
	}
	return result;

error:
	rt_error_message_write_last(_R("Error in server socket thread: "));
	/* Ensure that main thread will not wait for ever. */
	rt_event_signal(event);
	result = RT_FAILED;
	goto free;
}

/**
 * Tests socket assuming rt_socket_initialize/rt_socket_cleanup are called.
 */
rt_s RT_CALL ZzTestNonBlockingSockets()
{
	struct rt_event event;
	rt_b event_created;
	struct rt_thread thread;
	rt_b thread_created;
	rt_b bSocketCreated;
	rt_char8 buffer[RT_CHAR_BIG_STRING_SIZE];
	struct rt_socket zzSocket;
	rt_un received;
	struct rt_address_ipv4 zzAddress;
	struct rt_socket_address_ipv4 zzSocketAddress;
	rt_s ret;

	event_created = RT_FALSE;
	thread_created = RT_FALSE;
	bSocketCreated = RT_FALSE;

	if (!rt_event_create(&event)) goto error;
	event_created = RT_TRUE;

	if (!rt_thread_create(&thread, &ZzServerNonBlockingSocketThreadCallback, &event)) goto error;
	thread_created = RT_TRUE;

	if (!rt_event_wait_for(&event)) goto error;

	/* Let some time for the server socket to accept connections. */
	rt_sleep_sleep(10);

	if (!rt_socket_create(&zzSocket, RT_SOCKET_ADDRESS_FAMILY_IPV4, RT_SOCKET_TYPE_STREAM, RT_SOCKET_PROTOCOL_TCP, RT_FALSE, RT_FALSE)) goto error;
	bSocketCreated = RT_TRUE;

	rt_socket_address_create_ipv4_loopback_address(&zzAddress);
	rt_socket_address_create_ipv4(&zzSocketAddress, &zzAddress, ZZ_PORT_NUMBER);

	if (!rt_socket_connect_with_socket_address(&zzSocket, (struct rt_socket_address*)&zzSocketAddress)) {
		if (rt_error_would_block()) {
			rt_console_WriteCString(_R("Would block"));
		} else {
			goto error;
		}
	}

	received = rt_socket_receive_all(&zzSocket, buffer, RT_CHAR_BIG_STRING_SIZE);
	if (received == RT_TYPE_MAX_UN) goto error;
	if (received != rt_char8_get_size("Hello, world!") + 1) goto error;
	if (rt_char8_CompareStrings(buffer, "Hello, world!")) goto error;
	if (!rt_socket_shutdown(&zzSocket, RT_SOCKET_SHUTDOWN_BOTH)) goto error;

	if (!rt_thread_join_and_check(&thread)) goto error;

	ret = RT_OK;
free:
	if (bSocketCreated) {
		bSocketCreated = RT_FALSE;
		if (!rt_socket_free(&zzSocket) && ret)
			goto error;
	}
	if (thread_created) {
		thread_created = RT_FALSE;
		if (!rt_thread_free(&thread) && ret)
			goto error;
	}
	if (event_created) {
		event_created = RT_FALSE;
		if (!rt_event_free(&event) && ret)
			goto error;
	}
	return ret;

error:
	rt_error_message_write_last(_R("Non-blocking socket error: "));
	ret = RT_FAILED;
	goto free;
}

rt_s RT_CALL ZzTestNonBlockingSocket()
{
	rt_b bSocketsInitialized;
	rt_s ret;

	bSocketsInitialized = RT_FALSE;

	if (!rt_socket_initialize()) goto error;
	bSocketsInitialized = RT_TRUE;

	if (!ZzTestNonBlockingSockets()) goto error;

	ret = RT_OK;
free:
	if (bSocketsInitialized) {
		bSocketsInitialized = RT_FALSE;
		if (!rt_socket_cleanup() && ret)
			goto error;
	}
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}
