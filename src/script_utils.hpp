#pragma once

#include "squirrel/squirrel.h"
#include "utils.hpp"
#include <cassert>
#include <string>
#include <vector>

namespace hw {

// Check SQuirrel: Asserts call succeeded.
inline void CSQ(SQRESULT result) {
	assert(SQ_SUCCEEDED(result));
}

class ScriptEngine;
inline ScriptEngine* getEngine(HSQUIRRELVM vm) {
	return static_cast<ScriptEngine*>(sq_getforeignptr(vm));
}

inline SQRESULT loadScript(HSQUIRRELVM vm, const std::string& filename) {
	std::string script_contents = loadTextFile(filename);
	if (script_contents.empty())
		return SQ_ERROR;
	return sq_compilebuffer(vm, script_contents.c_str(), script_contents.size(), filename.c_str(), true);
}

inline void assertParamsNum(HSQUIRRELVM vm, SQInteger num) {
	assert(sq_gettop(vm) == num);
}

inline void assertParamsType(HSQUIRRELVM vm, SQInteger i, SQObjectType type) {
	assert(sq_gettype(vm, i) == type);
}

inline void assertParams(HSQUIRRELVM vm) {
	assertParamsNum(vm, 0);
}

inline void assertParams(HSQUIRRELVM vm, SQObjectType a) {
	assertParamsNum(vm, 1);
	assertParamsType(vm, 1, a);
}

inline void assertParams(HSQUIRRELVM vm, SQObjectType a, SQObjectType b) {
	assertParamsNum(vm, 2);
	assertParamsType(vm, 1, a);
	assertParamsType(vm, 2, b);
}

/** Recursively traverse tables and load the object indicated by path from the
 * table at the top of the stack. */
void loadVariablePath(HSQUIRRELVM vm, const std::vector<std::string>& path);
/** Shorthand: path is a string with parts of the path separated by '^'. */
void loadVariablePath(HSQUIRRELVM vm, const std::string& path);

inline SQRESULT getKey(HSQUIRRELVM vm, const char* key) {
	sq_pushstring(vm, key, -1);
	return sq_get(vm, -2);
}

} // namespace hw
