#include "fileservice.h"


FileService::FileService(const std::string baseDir, const int timeout) :
	baseDirectory(baseDir), reloadTimeout(timeout) {}

FileService::~FileService() {
	for (auto it = files.begin(); it != files.end(); ++it) {
		delete it->second;
	}
}

void FileService::start() {
	update(0, 0, this);
}


void FileService::get(const std::string& serverPath, const char** result, int& status) {
	std::string filename = baseDirectory + serverPath;
	if (hasEnding(filename, "/")) {
		filename += "index.html";
	}

	auto it = files.find(filename);
	if (it != files.end()) {
		*result = it->second->getContent();
		status = FILE_FOUND;

	} else {
		*result = nullptr;
		status = FILE_NOT_FOUND;
	}
}


int FileService::getReloadTimeout() {
	return reloadTimeout;
}


void FileService::reloadFiles() {
	std::vector<std::string> dirFiles = discoverFiles();

	for (auto it = dirFiles.begin(); it != dirFiles.end(); ++it) {
		if (hasEnding(*it, ".html")) {
			auto iterator = files.find(*it);
			// if file already exist before, just reload it
			if (iterator != files.end()) {
				iterator->second->reload();

			} else {
				// add file to file service
				File_mt* file = new File_mt(*it);
				file->reload();
				files.insert(std::pair<std::string, File_mt*>(*it, file));
			}
		}
	}
}


std::vector<std::string> FileService::discoverFiles() {
	DIR *dir = opendir(baseDirectory.c_str());
	if (dir != nullptr) {
		std::vector<std::string> filenames;

		// get all the files and directories within directory
		dirent *ent = readdir(dir);
		while (ent != nullptr) {
			filenames.push_back(baseDirectory + "/" + ent->d_name);
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


/**
 * Periodically call reloadFiles, must have void* as argument because it is
 * used as callback function for libevent.
 */
void update(int, short int, void* fsInstance) {
	FileService* mThis = (FileService*) fsInstance;
	mThis->reloadFiles();

	timeval tv;
	tv.tv_sec = mThis->getReloadTimeout();
	tv.tv_usec = 0;

	// we cannot use smart pointer here because the library do not expect it.
	event *loadfile_event = (event *)malloc(sizeof(event));
	evtimer_set(loadfile_event, update, fsInstance);
	evtimer_add(loadfile_event, &tv);
}
