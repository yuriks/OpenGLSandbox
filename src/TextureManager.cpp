#include "TextureManager.hpp"

#include "GL3/gl3w.h"
#include "stb_image.h"
#include "gl/Texture.hpp"

namespace hw {
namespace rsrc {

namespace {

static void setTextureParameters(Texture::FilterMode filter_mode, Texture::RepeatMode repeat_mode) {
	GLint gl_min_filter, gl_mag_filter, gl_repeat_mode;

	switch (filter_mode) {
	case Texture::FilterMode::NEAREST:
		gl_min_filter = gl_mag_filter = GL_NEAREST; break;
	case Texture::FilterMode::LINEAR:
		gl_min_filter = gl_mag_filter = GL_LINEAR; break;
	case Texture::FilterMode::MIPMAP_LINEAR:
		gl_min_filter = GL_LINEAR_MIPMAP_LINEAR;
		gl_mag_filter = GL_LINEAR; break;
	}

	switch (repeat_mode) {
	case Texture::RepeatMode::WRAP:
		gl_repeat_mode = GL_REPEAT; break;
	case Texture::RepeatMode::CLAMP:
		gl_repeat_mode = GL_CLAMP_TO_EDGE; break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_repeat_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_repeat_mode);
}

}

TextureHandle TextureManager::loadTexture(const std::string& name) {
	// Check if texture has already been loaded.
	auto existing = resource_names.find(name);
	if (existing != resource_names.end()) {
		return TextureHandle(this, existing->second);
	} else {
		std::string texture_filename;
		Texture::FilterMode filter_mode = Texture::FilterMode::LINEAR;
		Texture::RepeatMode repeat_mode = Texture::RepeatMode::WRAP;
		int channels = 4;

		// TODO placeholder entries
		if (name == "placeholder") {
			texture_filename = "data/placeholder.png";
			channels = 3;
		} else if (name == "gems/ruby") {
			texture_filename = "data/ruby.png";
		}

		if (texture_filename.empty()) {
			return TextureHandle();
		} else {
			gl::Texture new_tex;
			glGenTextures(1, &new_tex.name);

			glBindTexture(GL_TEXTURE_2D, new_tex.name);

			setTextureParameters(filter_mode, repeat_mode);

			assert(channels >= 1 && channels <= 4);
			static const GLint internal_formats[4] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};
			GLint gl_format = internal_formats[channels-1];

			int width, height, comp;
			unsigned char* data = stbi_load(texture_filename.c_str(), &width, &height, &comp, channels);
			if (data == nullptr) {
				return TextureHandle();
			}

			glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, data);

			size_t tex_index = first_free_resource;
			assert(tex_index != -1);
			first_free_resource = resources[tex_index].next_free;

			refcounts[tex_index] = 0;
			Texture* tex = new(resources[tex_index].data) Texture(std::move(new_tex), name, width, height);

			resource_names.emplace(name, tex_index);
			return TextureHandle(this, tex_index);
		}
	}
}

} // namespace rsrc
} // namespace hw
