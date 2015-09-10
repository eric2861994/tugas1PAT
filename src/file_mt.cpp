#include "file_mt.h"

#include <fstream>
#include <iostream>
#include <sys/stat.h>


File_mt::File_mt(const std::string& filename_) :
	filename(filename_), prevLoad(0), content(nullptr) {}


File_mt::~File_mt() {
	if (content != nullptr) {
		delete[] content;
	}
}


void File_mt::reload() {
	struct stat buf;
	stat(filename.c_str(), &buf);

	// if file is modified
	if (buf.st_mtime > prevLoad) {
		std::ifstream f(filename, std::ifstream::binary);

		// TODO file may be unavailable if deleted.
		if (f.fail()) {
			fprintf(stderr, "Couldn't open file\n");
			exit(1);
		}

		// get length of file
		f.seekg(0, f.end);
		std::size_t length = f.tellg();
		f.seekg(0, f.beg);

		// read file
		char* buffer = new char[length];
		f.read(buffer, length);

		f.close();

		// TODO investigate whether this can cause problem in destruction of content
		if (content != nullptr) {
			std::cout << "Reloaded: " << filename << " (" << length << " bytes)";
			delete[] content;

		} else {
			std::cout << "Loaded: " << filename << " (" << length << " bytes)";
		}
		std::cout << std::endl;
		content = buffer;
		prevLoad = buf.st_mtime;
	}
}


const char* File_mt::getContent() {
	return content;
}
