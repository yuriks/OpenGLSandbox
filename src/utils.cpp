#include "utils.hpp"

#include <fstream>

namespace hw {

std::string loadTextFile(const std::string& filename) {
	std::string ret;
	
	std::ifstream f(filename);
	if (f) {
		f.seekg(0, std::ios::end);
		ret.resize(static_cast<size_t>(f.tellg()));
		f.seekg(0);
		f.read(&ret[0], ret.size());
	}

	return ret;
}

} // namespace hw
