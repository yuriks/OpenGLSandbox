#include "ShaderManager.hpp"

#include "GL3/gl3w.h"
#include "gl/Shader.hpp"
#include "utils.hpp"

#include "engine_objects.hpp"

#include <boost/algorithm/string/predicate.hpp>
#include <iostream>

namespace hw {
namespace rsrc {

ShaderHandle ShaderManager::loadShader(const std::string& name) {
	// Check if shader has already been loaded.
	ShaderHandle existing = getLoadedResource(name);
	if (existing.valid())
		return existing;

	std::string shader_filename = "data/" + name;
	GLenum shader_type;

	if (boost::ends_with(name, ".vert")) {
		shader_type = GL_VERTEX_SHADER;
	} else if (boost::ends_with(name, ".frag")) {
		shader_type = GL_FRAGMENT_SHADER;
	} else {
		return ShaderHandle();
	}

	gl::Shader new_shader(glCreateShader(shader_type));

	std::string shader_source = loadTextFile(shader_filename);
	const char* shader_source_cstr = shader_source.c_str();
	glShaderSource(new_shader.name, 1, &shader_source_cstr, nullptr);
	glCompileShader(new_shader.name);

	GLint compile_result;
	glGetShaderiv(new_shader.name, GL_COMPILE_STATUS, &compile_result);

	if (compile_result == GL_FALSE) {
		GLint log_size;
		glGetShaderiv(new_shader.name, GL_INFO_LOG_LENGTH, &log_size);

		std::vector<char> log_buf(log_size);
		glGetShaderInfoLog(new_shader.name, log_buf.size(), nullptr, log_buf.data());

		// TODO change to logging
		std::cerr << "Error compiling " << name << ":\n";
		std::cerr << log_buf.data() << std::flush;

		return ShaderHandle();
	}

	return constructResource(name, Shader(std::move(new_shader), name));
}

} // namespace rsrc
} // namespace hw
