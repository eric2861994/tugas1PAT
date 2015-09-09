#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <event.h>
#include <evhttp.h>

#define RELOAD_TIMEOUT 5
#define DEFAULT_FILE "html/Sleeping.html"


char *filedata;
time_t lasttime = 0;
char filename[80];
int counter = 0;


void read_file() {
	struct stat buf;
	stat(filename, &buf);

	if (buf.st_mtime > lasttime) {
		if (counter++)
			fprintf(stderr, "Reloading file: %s", filename);
		else
			fprintf(stderr, "Loading file: %s", filename);

		FILE *f = fopen(filename, "rb");
		if (f == NULL) {
			fprintf(stderr, "Couldn't open file\n");
			exit(1);
		}

		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		char *data = (char *)malloc(size + 1);

		fseek(f, 0, SEEK_SET);
		fread(data, sizeof(char), size, f);
		fclose(f);

		filedata = (char *)malloc(size + 1);
		strcpy(filedata, data);

		fprintf(stderr, " (%d bytes)\n", size);
		lasttime = buf.st_mtime;
	}
}


void load_file() {
	read_file();

	struct timeval tv;
	tv.tv_sec = RELOAD_TIMEOUT;
	tv.tv_usec = 0;

	struct event *loadfile_event = malloc(sizeof(struct event));
	evtimer_set(loadfile_event, load_file, loadfile_event);
	evtimer_add(loadfile_event, &tv);
}


void generic_request_handler(struct evhttp_request *req, void *arg) {
	struct evbuffer *evb = evbuffer_new();
	evbuffer_add_printf(evb, "%s",filedata);

	evhttp_send_reply(req, HTTP_OK, "Client", evb);
	evbuffer_free(evb);
}


int main(int argc, char *argv[]) {
	if (argc > 1) {
		strcpy(filename, argv[1]);
		printf("Using %s\n", filename);

	} else {
		strcpy(filename, DEFAULT_FILE);
	}

	event_init();

	load_file();

	char *http_addr = "127.0.0.1";
	short http_port = 8081;
	struct evhttp *http_server = evhttp_start(http_addr, http_port);

	evhttp_set_gencb(http_server, generic_request_handler, NULL);
	fprintf(stderr, "Server started on port %d\n", http_port);

	event_dispatch();
}
