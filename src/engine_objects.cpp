#include "engine_objects.hpp"

#include "TextureManager.hpp"
#include "ScriptCore.hpp"

namespace hw {
namespace engine {

std::unique_ptr<rsrc::TextureManager> texture_manager;
std::unique_ptr<ScriptEngine> script_engine;

void initialize_engine_objects() {
	script_engine.reset(new ScriptEngine);
	texture_manager.reset(new rsrc::TextureManager);

	script_engine->runScript("data/main.nut", true);
}

void teardown_engine_objects() {
	texture_manager.reset(nullptr);
	script_engine.reset(nullptr);
}

} // namespace engine
} // namespace hw
