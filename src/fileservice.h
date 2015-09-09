#ifndef FILE_SERVICE_MH
#define FILE_SERVICE_MH

#include <string>
#include <map>


class FileService {
public:
	FileService();
	void start();
	void get(const std::string& filename, const char* result, int& status);

private:
	void update(void*);
	reload_files();

	std::map<std::string, File_mc> files;
};

#endif
