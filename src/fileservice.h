#ifndef FILE_SERVICE_MH
#define FILE_SERVICE_MH

#include "file_mt.h"
#include "utils.h"

#include <dirent.h>
#include <event.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>


/**
 * Memory File Service. Load all files in base directory to memory.
 * This service will try to reload all files when timeout occured.
 */
class FileService {
public:
	/**
	 * Instantiate File Service with base directory and time to timeout.
	 * @baseDir base directory for this file service
	 * @timeout time to timeout in seconds
	 */
	FileService(const std::string baseDir, const int timeout);
	~FileService();

	/**
	 * Start File Service.
	 */
	void start();

	/**
	 * Get data for file in base directory.
	 */
	void get(const std::string& filename, const char** result, int& status);

	int getReloadTimeout();

	static const int FILE_FOUND = 0, FILE_NOT_FOUND = 1;

	friend void update(int, short int, void*);

private:
	std::vector<std::string> discoverFiles();
	void reloadFiles();

	std::map<std::string, File_mt*> files; // TODO remove file?
	const int reloadTimeout;
	const std::string baseDirectory;
};

#endif
