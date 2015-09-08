#include "property.h"

#include <iostream>

const int DEFAULT_BUFFER_SIZE = 128 * 1024; // 128 KiB

Property::Property(std::string filename) {
	//// load and parse property file
	// open a property file
	// TODO possible error here
	std::ifstream propertyFile(filename);

	// process each line of property file
	char charBuffer[DEFAULT_BUFFER_SIZE];
	// TODO process cases where one buffer is not enough (low priority)
	propertyFile.getLine(charBuffer, DEFAULT_BUFFER_SIZE);
}
