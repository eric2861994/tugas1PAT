#include "file_mt.h"

#include <sys/stat.h>
#include <fstream>

File_mt::File_mt(const std::string& filename_) :
	filename(filename_), prevLoad(0) {}

void File_mt::reload() {
	struct stat buf;
	stat(filename.c_str(), &buf);

	if (buf.st_mtime > prevLoad) {
		std::ifstream f(filename, std::ifstream::binary);

		// TODO ASSUMPTION file is always available.
		if (f.fail()) {
			fprintf(stderr, "Couldn't open file\n");
			exit(1);
		}

		// get length of file
		f.seekg(0, f.end);
		int length = f.tellg();
		f.seekg(0, f.beg);

		// read file
		char* buffer = new char[length + 1];
		buffer[length] = '\0';
		f.read(buffer, length);

		f.close();

		content = buffer;
		prevLoad = buf.st_mtime;
	}
}
