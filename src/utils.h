#ifndef UTILS_MH
#define UTILS_MH

#include <string>
#include <vector>

/**
 * Find all positions of subString in text.
 */
std::vector<std::size_t> findPositions(const std::string& text, const std::string& subString);

/**
 * Split text with delimiter.
 */
std::vector<std::string> splitString(const std::string& text, const std::string& delimiter);

#endif
