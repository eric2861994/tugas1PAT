#include <event.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define SERVER_PORT 8081

/**
 * structure for a client.
 */
struct client {
	int fd;
	struct bufferevent *buf_ev;
};

/**
 * set file mode by using fcntl.
 */
int setnonblock(int fd) {
	int flags = fcntl(fd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
}

/**
 * Reply to request, Called when receive a request.
 */
void buf_read_callback(struct bufferevent *incoming, void *arg) {
	printf("buf_read_callback called\n");
	fflush(stdout);
	char *req = evbuffer_readline(incoming->input);
	// TODO does this mean request is partially received?
	if (req == NULL) {
		printf("buf_read_callback: req is NULL\n");
		fflush(stdout);
		return;
	}

	struct evbuffer *evreturn = evbuffer_new();
	evbuffer_add_printf(evreturn,"You said %s\n", req);
	bufferevent_write_buffer(incoming, evreturn);
	evbuffer_free(evreturn);
	// logic, evreturn cannot be freed before executing bufferevent_write_buffer
	printf("after bufferevent_write_buffer()\n");
	fflush(stdout);

	free(req);
}


// TODO when is this called?
void buf_write_callback(struct bufferevent *bev, void *arg) {
	printf("buf_write_callback called\n");
	fflush(stdout);
}

/**
 * Close client. This is called when there is an error with client socket.
 */
void buf_error_callback(struct bufferevent *bev, short what, void *arg) {
	printf("buf_error_callback called\n");
	fflush(stdout);

	struct client *client = (struct client *)arg;
	bufferevent_free(client->buf_ev);
	close(client->fd);
	free(client);
}

/**
 * This function is called when socket receive event. It is meant to handle
 * socket creation for new connections.
 */
void accept_callback(int fd, short ev, void *arg) {
	// accept client
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd < 0) {
		warn("Client: accept() failed");
		return;
	}

	// set client socket to non-blocking mode
	setnonblock(client_fd);

	// set up client structure
	struct client *client;
	// sizeof(*client) = sizeof(struct client)
	client = calloc(1, sizeof(*client));
	if (client == NULL)
		err(1, "malloc failed");

	client->fd = client_fd;
	client->buf_ev = bufferevent_new(client_fd, buf_read_callback,
		buf_write_callback, buf_error_callback, client);

	bufferevent_enable(client->buf_ev, EV_READ);
}

int main(int argc, char **argv) {
	event_init();

	// create passive socket
	int socketlisten = socket(AF_INET, SOCK_STREAM, 0);
	if (socketlisten < 0) {
		fprintf(stderr,"Failed to create listen socket");
		return 1;
	}

	struct sockaddr_in addresslisten;
	memset(&addresslisten, 0, sizeof(addresslisten));

	addresslisten.sin_family = AF_INET;
	addresslisten.sin_addr.s_addr = INADDR_ANY;
	addresslisten.sin_port = htons(SERVER_PORT);

	if (bind( socketlisten, (struct sockaddr *)&addresslisten,
		sizeof(addresslisten) ) < 0) {

		fprintf(stderr,"Failed to bind");
		return 1;
	}

	if (listen(socketlisten, 5) < 0) {
		fprintf(stderr,"Failed to listen to socket");
		return 1;
	}

	// set socket options
	int reuse = 1;
	setsockopt(socketlisten, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	setnonblock(socketlisten);

	// set callback function (for READ or PERSIST event) for socket
	struct event accept_event;
	event_set(&accept_event, socketlisten, EV_READ|EV_PERSIST,
		accept_callback, NULL);
	event_add(&accept_event, NULL);

	event_dispatch();
	close(socketlisten);

	return 0;
}
