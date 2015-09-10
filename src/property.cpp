#include "property.h"


Property::Property(const std::string& filename) {
	// open a property file
	std::ifstream propertyFile(filename);

	// process each line of property file
	do {
		std::string line;
		std::getline(propertyFile, line);

		// process the line to map of property and value
		std::string key, value;
		int error;
		parseLine(line, key, value, error);
		if (error == 0) {
			set(key, value);
		}

	} while (propertyFile.good());
}


std::string Property::get(const std::string& key) {
	std::map<std::string, std::string>::iterator iterator = values.find(key);

	if (iterator != values.end()) {
		return iterator->second;

	} else {
		return "<" + key + " not found>";
	}
}


/**
 * Parse a line from property file.
 */
void Property::parseLine(const std::string& line, std::string& key, std::string& value, int& error) {
	// split against ' = '
	const std::vector<std::string> result = splitString(line, " = ");

	// key is the first element, value is the second element
	if (result.size() == 2) {
		key = result[0];
		value = result[1];
		error = 0;

	} else {
		key = "";
		value = "";
		error = 1;
	}
}


void Property::set(const std::string& key, const std::string& value) {
	values[key] = value;
}
