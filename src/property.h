#ifndef PROPERTY_MH
#define PROPERTY_MH

#include <string>

/**
 * Property file loader.
 */
class Property {
public:
	Property(std::string filename);
	std::string get(std::string key);

private:
};

#endif
