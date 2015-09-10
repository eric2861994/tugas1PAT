#ifndef FILE_SERVICE_MH
#define FILE_SERVICE_MH

#include "file_mt.h"

#include <map>
#include <string>
#include <vector>


class FileService {
public:
	FileService(const std::string baseDir, const int timeout);
	void start();
	void get(const std::string& filename, const char* result, int& status);
	int getReloadTimeout();

	void _reloadFiles();

	static const int FILE_FOUND = 0, FILE_NOT_FOUND = 1;

private:
	std::vector<std::string> discoverFiles();

	std::map<std::string, File_mt> files;
	const int reloadTimeout;
	const std::string baseDirectory;
};

#endif
