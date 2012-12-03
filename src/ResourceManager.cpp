#include "ResourceManager.hpp"

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

ResourceManager::ResourceManager()
	: first_free_texture(0)
{
	for (size_t i = 0; i < textures.size()-1; ++i) {
		textures[i].next_free = i+1;
	}
	textures.back().next_free = -1;
}

ResourceManager::~ResourceManager() {
	collectGarbage();
	assert(texture_names.empty());
}

TextureHandle ResourceManager::loadTexture(const std::string& name) {
	// Check if texture has already been loaded.
	auto existing = texture_names.find(name);
	if (existing != texture_names.end()) {
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

			size_t tex_index = first_free_texture;
			assert(tex_index != -1);
			first_free_texture = textures[tex_index].next_free;

			textures[tex_index].refcount = 0;
			Texture* tex = new(textures[tex_index].texture_data) Texture(std::move(new_tex), name, width, height);

			texture_names.emplace(name, tex_index);
			return TextureHandle(this, tex_index);
		}
	}
}

void ResourceManager::collectGarbage() {
	for (auto i = texture_names.begin(), end = texture_names.end(); i != end;) {
		size_t index = i->second;
		TextureNode& node = textures[index];
		assert(node.refcount >= 0);

		if (node.refcount == 0) {
			Texture* tex = reinterpret_cast<Texture*>(node.texture_data);
			tex->~Texture();

			node.next_free = first_free_texture;
			first_free_texture = index;

			texture_names.erase(i++);
		} else {
			++i;
		}
	}
}

} // namespace rsrc
} // namespace hw
