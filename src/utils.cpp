#include "utils.h"

std::vector<std::size_t> findPositions(const std::string& text, const std::string& subString) {
	std::vector<std::size_t> positions;

	std::size_t found = text.find(subString);
	while (found != std::string::npos) {
		positions.push_back(found);
		found = text.find(subString, found + 1);
	}

	return positions;
}


std::vector<std::string> splitString(const std::string& text, const std::string& delimiter) {
	// get delimeter positions
	const std::vector<std::size_t> positions = findPositions(text, delimiter);

	const std::size_t delimiterLength = delimiter.length();
	std::size_t start = 0;
	std::vector<std::string> splits;

	const std::size_t positionsLength = positions.size();
	for (std::size_t i = 0; i < positionsLength; ++i) {
		// push splitted segment
		const std::size_t length = positions[i] - start;
		splits.push_back(text.substr(start, length));

		start = positions[i] + delimiterLength;
	}

	// push last segment
	splits.push_back(text.substr(start));

	return splits;
}
