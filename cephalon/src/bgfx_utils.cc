#include "bgfx_utils.h"

#include <fstream>
#include <vector>

#include <bx/bx.h>
#include <bx/string.h>

bgfx::ShaderHandle cephalon::loadShader(const char* name) {
    char filePath[512];
    const char* shaderPath = "???";

	switch (bgfx::getRendererType() )
	{
	    case bgfx::RendererType::Noop:
	    case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
	    case bgfx::RendererType::Direct3D11:
	    case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
	    case bgfx::RendererType::Agc:
	    case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
	    case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
	    case bgfx::RendererType::Nvn:        shaderPath = "shaders/nvn/";   break;
	    case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
	    case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
	    case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
	    case bgfx::RendererType::WebGPU:     shaderPath = "shaders/spirv/"; break;
	}

	bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
	bx::strCat(filePath, BX_COUNTOF(filePath), name);
	bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

    std::ifstream file(filePath, std::ios::ate | std::ios::binary);
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> data(size);
    file.read(data.data(), size);
    const bgfx::Memory* memory = bgfx::copy(data.data(), size);
    memory->data[size - 1] = '\0';

	bgfx::ShaderHandle handle = bgfx::createShader(memory);
	bgfx::setName(handle, name);
    
    return handle;
}

bgfx::ProgramHandle cephalon::loadProgram(const char* vs, const char* fs) {
    auto vsh = loadShader(vs);
    auto fsh = loadShader(fs);
    return bgfx::createProgram(vsh, fsh, true);
}