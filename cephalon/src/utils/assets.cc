#include "assets.h"

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include <bx/bx.h>
#include <bx/error.h>
#include <bx/string.h>
#include <spdlog/spdlog.h>

bgfx::ShaderHandle cephalon::LoadShader(const std::string& name) {
    std::string path;

	switch (bgfx::getRendererType() )
	{
	    case bgfx::RendererType::Noop:
//	    case bgfx::RendererType::Direct3D9:  path += "assets/shaders/dx9/";   break;
	    case bgfx::RendererType::Direct3D11:
	    case bgfx::RendererType::Direct3D12: path += "assets/shaders/dx11/";  break;
	    case bgfx::RendererType::Agc:
	    case bgfx::RendererType::Gnm:        path += "assets/shaders/pssl/";  break;
	    case bgfx::RendererType::Metal:      path += "assets/shaders/metal/"; break;
	    case bgfx::RendererType::Nvn:        path += "assets/shaders/nvn/";   break;
	    case bgfx::RendererType::OpenGL:     path += "assets/shaders/glsl/";  break;
	    case bgfx::RendererType::OpenGLES:   path += "assets/shaders/essl/";  break;
	    case bgfx::RendererType::Vulkan:     path += "assets/shaders/spirv/"; break;
//	    case bgfx::RendererType::WebGPU:     path += "assets/shaders/spirv/"; break;
	}

    path += name;
    path += ".bin";

    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        spdlog::error("Couldn't open shader file {}", path);
        return BGFX_INVALID_HANDLE;
    }
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> data(size);
    file.read(data.data(), size);
    const bgfx::Memory* memory = bgfx::copy(data.data(), size);
    memory->data[size - 1] = '\0';

	bgfx::ShaderHandle handle = bgfx::createShader(memory);
	bgfx::setName(handle, name.c_str());
    
    if (!bgfx::isValid(handle)) {
        spdlog::error("Error creating shader {}", name);
    } else {
        spdlog::info("Shader {} loaded successfully", name);
    }
    return handle;
}

bgfx::ProgramHandle cephalon::LoadProgram(const std::string& vs, const std::string& fs) {
    bgfx::ShaderHandle vsh = LoadShader(vs);
    bgfx::ShaderHandle fsh = LoadShader(fs);
    bgfx::ProgramHandle handle = bgfx::createProgram(vsh, fsh, true);
    if (!bgfx::isValid(handle))
        spdlog::error("Error creating program of shader {} and {}", vs, fs);
    else
        spdlog::info("Program of shader {} and {} loaded successfully", vs, fs);
    return handle;
}

bimg::ImageContainer* cephalon::LoadImage(const std::string& name) {
    std::string path = "assets/textures/";
    path += name;
    path += ".dds";

    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        spdlog::error("Couldn't open image file {}", path);
        return nullptr;
    }
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> data(size);
    file.read(data.data(), size);
    bx::DefaultAllocator allocator;
    bx::Error error;
    bimg::ImageContainer* image = bimg::imageParseDds(&allocator, data.data(), data.size(), &error);
    if (!error.isOk()) {
        auto message = error.getMessage();
        spdlog::error("Error parsing image: {}", std::string_view(message.getPtr(), message.getLength()));
    } else {
        spdlog::info("Image {} loaded successfully", name);
    }
    return image;
}
