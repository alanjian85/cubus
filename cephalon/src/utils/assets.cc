#include "assets.h"

#include <fstream>
#include <string_view>
#include <vector>

#include <bx/bx.h>
#include <bx/error.h>
#include <bx/string.h>
#include <spdlog/spdlog.h>

bgfx::ShaderHandle cephalon::LoadShader(const char* name) {
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
    if (!file.is_open()) {
        spdlog::error("Could not open {} shader file", name);
        return BGFX_INVALID_HANDLE;
    }
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> data(size);
    file.read(data.data(), size);
    const bgfx::Memory* memory = bgfx::copy(data.data(), size);
    memory->data[size - 1] = '\0';

	bgfx::ShaderHandle handle = bgfx::createShader(memory);
	bgfx::setName(handle, name);
    
    if (!bgfx::isValid(handle))
        spdlog::error("Error creating shader {}", name);
    return handle;
}

bgfx::ProgramHandle cephalon::LoadProgram(const char* vs, const char* fs) {
    bgfx::ShaderHandle vsh = LoadShader(vs);
    bgfx::ShaderHandle fsh = LoadShader(fs);
    bgfx::ProgramHandle handle = bgfx::createProgram(vsh, fsh, true);
    if (!bgfx::isValid(handle))
        spdlog::error("Error creating program for shader {} and {}", vs, fs);
    return handle;
}

bimg::ImageContainer* cephalon::LoadImage(const char* path, bimg::TextureFormat::Enum dst_format) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
        spdlog::error("Could not open image file at {}", path);
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> data(size);
    file.read(data.data(), size);
    bx::DefaultAllocator allocator;
    bx::Error error;
    bimg::ImageContainer* image = bimg::imageParse(&allocator, data.data(), data.size(), dst_format, &error);
    if (!error.isOk()) {
        auto message = error.getMessage();
        spdlog::error("Error parsing image: {}", std::string_view(message.getPtr(), message.getLength()));
    }
    return image;
}

bgfx::TextureHandle cephalon::LoadTexture(const char* path, std::uint64_t flags) {
    bimg::ImageContainer* image = LoadImage(path);
    return bgfx::createTexture2D(
        image->m_width,
        image->m_height,
        1 < image->m_numMips,
        image->m_numLayers, 
        bgfx::TextureFormat::Enum(image->m_format),
        flags,
        bgfx::makeRef(image->m_data, image->m_size)
    );
}