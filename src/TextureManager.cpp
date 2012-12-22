#include "TextureManager.hpp"

#include "GL3/gl3w.h"
#include "stb_image.h"
#include "gl/Texture.hpp"

#include "engine_objects.hpp"
#include "ScriptCore.hpp"
#include "squirrel/squirrel.h"
#include "script_utils.hpp"

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

TextureManager::TextureManager() {
	HSQUIRRELVM vm = engine::script_engine->vm;

	sq_pushroottable(vm); // 1
	sq_pushstring(vm, "Resources", -1); // 2
	CSQ(sq_get(vm, -2)); // 2

	sq_pushstring(vm, "TextureInfo", -1); // 3
	sq_newtable(vm); // 4
	CSQ(sq_newslot(vm, -3, SQFalse)); // 2
	sq_poptop(vm); // 1

#define ADD_CONSTANT(name) \
		sq_pushstring(vm, PREFIX #name, -1); \
		sq_pushinteger(vm, static_cast<SQInteger>(ENUM_PREFIX name)); \
		CSQ(sq_newslot(vm, -3, SQFalse))

#define PREFIX "TEXFILTER_"
#define ENUM_PREFIX Texture::FilterMode::
	ADD_CONSTANT(NEAREST);
	ADD_CONSTANT(LINEAR);
	ADD_CONSTANT(MIPMAP_LINEAR);
#undef PREFIX
#undef ENUM_PREFIX

#define PREFIX "TEXREPEAT_"
#define ENUM_PREFIX Texture::RepeatMode::
	ADD_CONSTANT(CLAMP);
	ADD_CONSTANT(WRAP);
#undef PREFIX
#undef ENUM_PREFIX

#undef ADD_CONSTANT

	sq_poptop(vm); // 0
}

TextureHandle TextureManager::loadTexture(const std::string& name) {
	// Check if texture has already been loaded.
	TextureHandle existing = getLoadedResource(name);
	if (existing.valid())
		return existing;

	std::string texture_filename;
	Texture::FilterMode filter_mode = Texture::FilterMode::LINEAR;
	Texture::RepeatMode repeat_mode = Texture::RepeatMode::WRAP;
	int channels = 4;

	HSQUIRRELVM vm = ::hw::engine::script_engine->vm;
	sq_pushroottable(vm);
	loadVariablePath(vm, "Resources^TextureInfo^" + name);

	const SQChar* tex_filename_cstr;

	CSQ(getKey(vm, "filename"));
	sq_getstring(vm, -1, &tex_filename_cstr);
	sq_poptop(vm);
	texture_filename = tex_filename_cstr;

	if (SQ_SUCCEEDED(getKey(vm, "channels"))) {
		sq_getinteger(vm, -1, &channels);
		sq_poptop(vm);
	}

	if (SQ_SUCCEEDED(getKey(vm, "filter_mode"))) {
		SQInteger tmp;
		sq_getinteger(vm, -1, &tmp);
		sq_poptop(vm);

		filter_mode = Texture::FilterMode(tmp);
	}

	if (SQ_SUCCEEDED(getKey(vm, "repeat_mode"))) {
		SQInteger tmp;
		sq_getinteger(vm, -1, &tmp);
		sq_poptop(vm);

		repeat_mode = Texture::RepeatMode(tmp);
	}

	texture_filename = "data/" + texture_filename;

	if (texture_filename.empty()) {
		return TextureHandle();
	}

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

	return constructResource(name, Texture(std::move(new_tex), name, width, height));
}

} // namespace rsrc
} // namespace hw
