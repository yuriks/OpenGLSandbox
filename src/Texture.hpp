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
#include "gl/Texture.hpp"

namespace hw {
namespace rsrc {

class Texture : private boost::noncopyable {
public:
	static const int MAX_RESOURCES = 64;

	enum class FilterMode {
		NEAREST,
		LINEAR,
		MIPMAP_LINEAR
	};

	enum class RepeatMode {
		CLAMP,
		WRAP
	};

	//void setFilterMode(FilterMode filter_mode);
	//void setRepeatMode(RepeatMode repeat_mode);

	const gl::Texture& getGLTexture() const { return opengl_tex; }

	int getImageWidth() const { return image_width; }
	int getImageHeight() const { return image_height; }

private:
	Texture(gl::Texture&& opengl_tex, std::string name, int width, int height)
		: opengl_tex(std::move(opengl_tex)), image_name(std::move(name)),
		image_width(width), image_height(height)
	{ }

	gl::Texture opengl_tex;

	std::string image_name;
	int image_width, image_height;

	//FilterMode filter_mode;
	//RepeatMode repeat_mode;

	friend class TextureManager;
};

} // namespace rsrc
} // namespace hw
