#pragma once

#include <boost/noncopyable.hpp>
#include <string>

struct SQVM;

namespace hw {

class ScriptEngine : private boost::noncopyable {
public:
	ScriptEngine();
	~ScriptEngine();

	void runScript(const std::string& filename, bool require = false);

	SQVM* vm;

private:
	void initScriptFunctions();
};

} // namespace hw
