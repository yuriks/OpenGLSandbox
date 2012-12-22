#pragma once

#include <string>
#include <memory>
#include <array>
#include <cassert>
#include <atomic>
#include <unordered_map>
#include <utility>

#include <GL3/gl3.h>
#include <boost/noncopyable.hpp>
#include "gl/Shader.hpp"

namespace hw {
namespace rsrc {

class Shader : private boost::noncopyable {
public:
	static const int MAX_RESOURCES = 16;

	const gl::Shader& getGLShader() const { return opengl_shader; }

	Shader(Shader&& o)
		: opengl_shader(std::move(o.opengl_shader)), shader_name(std::move(o.shader_name))
	{ }

private:
	Shader(gl::Shader&& opengl_shader, std::string name)
		: opengl_shader(std::move(opengl_shader)), shader_name(std::move(name))
	{ }

	gl::Shader opengl_shader;
	std::string shader_name;

	friend class ShaderManager;
};

} // namespace rsrc
} // namespace hw
