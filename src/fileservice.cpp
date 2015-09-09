#include "fileservice.h"

char* FileService::get(const std::string& filename) {
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
