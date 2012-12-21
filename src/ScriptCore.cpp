#include "ScriptCore.hpp"

#include <cassert>
#include <iostream>

#include "squirrel/squirrel.h"
#include "script_utils.hpp"

namespace hw {

void compilerErrorHandler(HSQUIRRELVM vm, const SQChar* desc, const SQChar* source,
		SQInteger line, SQInteger column) {
	std::cerr << source << ':' << line << ": " << desc << std::endl;
}

ScriptEngine::ScriptEngine() {
	vm = sq_open(1024);
	sq_setforeignptr(vm, static_cast<SQUserPointer>(this));
	sq_setcompilererrorhandler(vm, compilerErrorHandler);

	initScriptFunctions();
}

ScriptEngine::~ScriptEngine() {
	sq_close(vm);
}

void ScriptEngine::runScript(const std::string& filename, bool require) {
	if (SQ_SUCCEEDED(loadScript(vm, filename))) {
		sq_pushroottable(vm);
		CSQ(sq_call(vm, 1, false, true));
		sq_poptop(vm);
	} else {
		if (require) {
			assert(false);
		} else {
			// TODO Log error
		}
	}
}

SQInteger squirrel_Script_include(HSQUIRRELVM vm) {
	assertParams(vm, OT_TABLE, OT_STRING);

	const SQChar* str;
	sq_getstring(vm, 2, &str);

	getEngine(vm)->runScript(std::string("data/") + str, true);

	return 0;
}

SQInteger squirrel_Script_debugPrint(HSQUIRRELVM vm) {
	assertParams(vm, OT_TABLE, OT_STRING);

	const SQChar* str;
	sq_getstring(vm, 2, &str);

	std::cerr << str << std::endl;

	return 0;
}

SQInteger squirrel_Script_setErrorHandler(HSQUIRRELVM vm) {
	assertParams(vm, OT_TABLE, OT_CLOSURE);

	sq_seterrorhandler(vm);

	return 0;
}

void ScriptEngine::initScriptFunctions() {
	sq_pushroottable(vm);

	sq_pushstring(vm, "Script", -1);
	sq_newtable(vm);

#define REGISTER_FUNCTION(name) \
	sq_pushstring(vm, #name, -1); \
	sq_newclosure(vm, squirrel_Script_##name, 0); \
	CSQ(sq_newslot(vm, -3, SQFalse))

	REGISTER_FUNCTION(include);
	REGISTER_FUNCTION(debugPrint);
	REGISTER_FUNCTION(setErrorHandler);
#undef REGISTER_FUNCTION

	CSQ(sq_newslot(vm, -3, SQFalse));

	sq_pushstring(vm, "Resources", -1);
	sq_newtable(vm);
	CSQ(sq_newslot(vm, -3, SQFalse));

	sq_poptop(vm);
}

} // namespace hw
