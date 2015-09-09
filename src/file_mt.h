#ifndef FILE_MT_H
#define FILE_MT_H

#include <string>

/**
 * File with smart reload, will only reload if the file changes.
 */
class File_mt {
public:
	File_mt(const std::string& filename_);
	void reload();

private:
	const std::string filename;

	std::string content;
	time_t prevLoad;
};

#endif
