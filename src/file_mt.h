#ifndef FILE_MT_H
#define FILE_MT_H

#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

/**
 * On Memory File with smart reload, will only reload if the file changes.
 */
class File_mt {
public:
	File_mt(const std::string& filename_);
	~File_mt();

	/**
	 * Reload a file if it is modified. It will also load the file if it haven't
	 * been loaded before.
	 */
	void reload();

	/**
	 * Return read only content as char buffer.
	 */
	const char* getContent();
	std::size_t getContentLength();

private:
	const std::string filename;

	const char* content;
	std::size_t contentLength;
	time_t prevLoad;
};

#endif
