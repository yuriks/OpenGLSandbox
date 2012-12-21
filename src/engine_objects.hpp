#pragma once

#include <memory>

namespace hw {

class ScriptEngine;
namespace rsrc {
	class TextureManager;
}

namespace engine {

extern std::unique_ptr<rsrc::TextureManager> texture_manager;
extern std::unique_ptr<ScriptEngine> script_engine;

void initialize_engine_objects();
void teardown_engine_objects();

} // namespace engine
} // namespace hw
