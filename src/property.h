#ifndef PROPERTY_MH
#define PROPERTY_MH

#include "utils.h"

#include <fstream>
#include <map>
#include <string>
#include <vector>


/**
 * Property file loader.
 */
class Property {
public:
	/**
	 * Construct from property file.
	 */
	Property(const std::string& filename);

	/**
	 * Get value for a key in this property.
	 */
	std::string get(const std::string& key);

private:
	void parseLine(const std::string& line, std::string& key, std::string& value, int& error);
	void set(const std::string& key, const std::string& value);

	std::map<std::string, std::string> values;
};

#endif
