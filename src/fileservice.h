#ifndef FILE_SERVICE_MH
#define FILE_SERVICE_MH

#include <string>



class FileService {
public:
	FileService();
	void start();
	std::string get(const std::string& filename);

private:
	void update(void*);
	reload_files();

	std::map<std::string, File_mc> files;
};

#endif
