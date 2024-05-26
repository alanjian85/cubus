#pragma once

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/error.h>
#include <bimg/encode.h>

namespace cephalon {
    class Callback : public bgfx::CallbackI {
	    virtual void fatal(const char* path, uint16_t line, bgfx::Fatal::Enum code, const char* str) override {

	    }

	    virtual void traceVargs(const char* path, uint16_t line, const char* format, va_list arg_list) override {

	    }

	    virtual void profilerBegin(const char* name, uint32_t abgr, const char* path, uint16_t line) override {

	    }

	    virtual void profilerBeginLiteral(const char* name, uint32_t abgr, const char* path, uint16_t line) override {

	    }

	    virtual void profilerEnd() override {

	    }

	    virtual uint32_t cacheReadSize(uint64_t id) override {
	    	return 0;
	    }

	    virtual bool cacheRead(uint64_t id, void* data, uint32_t size) override {
            return false;
	    }

	    virtual void cacheWrite(uint64_t id, const void* data, uint32_t size) override {
	    	
	    }

	    virtual void screenShot(const char* path, uint32_t width, uint32_t height, uint32_t pitch, const void* data, uint32_t size, bool yflip) override {
            bx::FileWriter writer;
	        if (bx::open(&writer, path, false))
	        {
	        	bimg::imageWritePng(&writer, width, height, pitch, data, bimg::TextureFormat::BGRA8, yflip);
	        	bx::close(&writer);
	        }
	    }

	    virtual void captureBegin(uint32_t width, uint32_t height, uint32_t pitch, bgfx::TextureFormat::Enum format, bool yflip) override {

	    }

	    virtual void captureEnd() override {

	    }

	    virtual void captureFrame(const void* data, uint32_t size) override {

	    }
    };
}
