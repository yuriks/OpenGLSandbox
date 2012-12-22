#include "script_utils.hpp"

#include <boost/algorithm/string/split.hpp>

namespace hw {

void loadVariablePath(HSQUIRRELVM vm, const std::vector<std::string>& path) {
	for (const std::string& i : path) {
		sq_pushstring(vm, i.c_str(), i.size());
		CSQ(sq_get(vm, -2));
	}

	HSQOBJECT h;
	CSQ(sq_getstackobj(vm, -1, &h));
	sq_pop(vm, path.size() - 1);
	sq_pushobject(vm, h);
}

void loadVariablePath(HSQUIRRELVM vm, const std::string& path) {
	std::vector<std::string> split_path;
	boost::split(split_path, path, [](char c) { return c == '^'; });
	loadVariablePath(vm, split_path);
}

}