#pragma once

#include <boost/noncopyable.hpp>
#include "Texture.hpp"
#include "ResourceManager.hpp"

namespace hw {
namespace rsrc {

typedef ResourceHandle<Texture> TextureHandle;

class TextureManager : public ResourceManager<Texture> {
public:
	TextureManager();

	TextureHandle loadTexture(const std::string& name);
};

} // namespace rsrc
} // namespace hw
