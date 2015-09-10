#include "fileservice.h"

#include <dirent.h>
#include <event.h>
#include <iostream>


void update(int, short int, void* fsInstance);


FileService::FileService(const std::string baseDir, const int timeout) :
	baseDirectory(baseDir), reloadTimeout(timeout) {}


void FileService::start() {
	update(0, 0, this);
}


void FileService::get(const std::string& filename, const char* result, int& status) {
	auto it = files.find(filename);
	if (it != files.end()) {
		result = it->second.getContent();
		status = FILE_FOUND;

	} else {
		result = nullptr;
		status = FILE_NOT_FOUND;
	}
}


/**
 * Periodically call reloadFiles, must have void* as argument because it is
 * used as callback function for libevent.
 */
void update(int, short int, void* fsInstance) {
	FileService* mThis = (FileService*) fsInstance;
	// TODO set this to friend
	mThis->_reloadFiles();

	timeval tv;
	tv.tv_sec = mThis->getReloadTimeout();
	tv.tv_usec = 0;

	// we cannot use smart pointer here because the library do not expect it.
	event *loadfile_event = (event*) malloc(sizeof(event));
	evtimer_set(loadfile_event, update, fsInstance);
	evtimer_add(loadfile_event, &tv);
}

void printVector(std::vector<std::string> v) {
	for (auto it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << '\n';
	}
	std::cout.flush();
}

void FileService::_reloadFiles() {
	std::vector<std::string> files = discoverFiles();

	std::cout << "reloading files:\n";
	printVector(files);
	// reload file for each files in filemap
	// for (auto it = files.begin(); it != files.end(); ++it) {
	// 	it->second.reload();
	// }
}

int FileService::getReloadTimeout() {
	return reloadTimeout;
}

std::vector<std::string> FileService::discoverFiles() {
	DIR *dir = opendir(baseDirectory.c_str());
	dirent *ent;
	if (dir != nullptr) {
		std::vector<std::string> filenames;
		// get all the files and directories within directory
		dirent *ent = readdir(dir);
		while (ent != nullptr) {
			filenames.push_back(ent->d_name);
			ent = readdir(dir);
		}
		closedir(dir);

		return filenames;

	} else {
		// could not open directory
		perror ("");
		exit(1);
	}
}
