#pragma once

#include <boost/noncopyable.hpp>
#include "Shader.hpp"
#include "ResourceManager.hpp"

namespace hw {
namespace rsrc {

typedef ResourceHandle<Shader> ShaderHandle;

class ShaderManager : public ResourceManager<Shader> {
public:
	ShaderHandle loadShader(const std::string& name);
};

} // namespace rsrc
} // namespace hw
