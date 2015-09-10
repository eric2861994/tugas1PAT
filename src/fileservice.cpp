#include "fileservice.h"


void update(int, short int, void* fsInstance);


FileService::FileService(const std::string baseDir, const int timeout) :
	baseDirectory(baseDir), reloadTimeout(timeout) {}


void FileService::start() {
	update(0, 0, this);
}


void FileService::get(const std::string& serverPath, const char** result, int& status) {
	const std::string filename = baseDirectory + serverPath;

	auto it = files.find(filename);
	if (it != files.end()) {
		*result = it->second->getContent();
		status = FILE_FOUND;

	} else {
		*result = nullptr;
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

int FileService::getReloadTimeout() {
	return reloadTimeout;
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
