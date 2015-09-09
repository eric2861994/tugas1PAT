#include "fileservice.h"

std::string FileService::get(const std::string& filename) {
	// search filename in files map
	// get the content
}

void FileService::start() {
	update(nullptr);
}


/**
 * Periodically call reloadFiles, must have void* as argument because it is
 * used as callback function for libevent.
 */
void FileService::update(void*) {
	reloadFiles();

	timeval tv;
	tv.tv_sec = reloadTimeout;
	tv.tv_usec = 0;

	// we cannot use smart pointer here because the library do not expect it.
	event* loadfiles_event
	evtimer_new(loadfile_event, update, nullptr);
	evtimer_add(loadfile_event, &tv);
}

void reloadFiles() {
	// for each files in filemap

	// reload file
}


// put this to File_mc class
void reloadFile(File_mc) {
	stat buf;
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
