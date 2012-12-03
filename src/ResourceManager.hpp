#pragma once

#include <boost/noncopyable.hpp>
#include "Texture.hpp"

namespace hw {
namespace rsrc {

class ResourceManager : private boost::noncopyable {
public:
	ResourceManager();
	~ResourceManager();

	TextureHandle loadTexture(const std::string& name);

	// Frees resources with no more references.
	void collectGarbage();

	void increaseTextureRefcount(int index) {
		++textures[index].refcount;
	}

	void decreaseTextureRefcount(int index) {
		--textures[index].refcount;
	}

	Texture& getTexture(size_t index) {
		return *reinterpret_cast<Texture*>(textures[index].texture_data);
	}

private:
	static const int MAX_TEXTURES = 64;

	struct TextureNode {
		union {
			char texture_data[sizeof(Texture)];
			size_t next_free;
		};
		int refcount;
	};

	std::unordered_map<std::string, size_t> texture_names;
	std::array<TextureNode, MAX_TEXTURES> textures;
	size_t first_free_texture;
};

class TextureHandle {
public:
	TextureHandle()
		: manager(nullptr)
	{
	}

	TextureHandle(const TextureHandle& o)
		: index(o.index), manager(o.manager)
	{
		manager->increaseTextureRefcount(index);
	}

	TextureHandle(TextureHandle&& o)
		: index(o.index), manager(o.manager)
	{
		o.manager = nullptr;
	}

	TextureHandle& operator =(const TextureHandle& o) {
		o.manager->increaseTextureRefcount(o.index);
		if (manager)
			manager->decreaseTextureRefcount(index);

		index = o.index;
		manager = o.manager;
	}

	TextureHandle& operator =(TextureHandle&& o) {
		if (manager)
			manager->decreaseTextureRefcount(index);

		index = o.index;
		manager = o.manager;
		o.manager = nullptr;
	}

	~TextureHandle() {
		if (manager)
			manager->decreaseTextureRefcount(index);
	}

	Texture& operator *() {
		return manager->getTexture(index);
	}

	Texture* operator ->() {
		return &(operator*());
	}

private:
	TextureHandle(ResourceManager* manager, size_t index)
		: index(index), manager(manager)
	{
		manager->increaseTextureRefcount(index);
	}

	size_t index;
	ResourceManager* manager;

	friend class ResourceManager;
};

} // namespace rsrc
} // namespace hw
