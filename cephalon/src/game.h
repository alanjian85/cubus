#ifndef CEPHALON_GAME_H_
#define CEPHALON_GAME_H_

#include "bgfx_utils.h"

namespace cephalon {
    class Game {
    public:
        void start() {
            float vertices[] = {
                -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
                 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f
            };

            layout_.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
            .end();
            vbh_ = bgfx::createVertexBuffer(bgfx::copy(vertices, sizeof(vertices)), layout_);

            program_ = LoadProgram("vs_shader", "fs_shader");
        }

        void update(float delta) {

        }

        void render() {
            bgfx::setVertexBuffer(0, vbh_);
            bgfx::submit(0, program_);
        }
    private:
        bgfx::VertexLayout layout_;
        bgfx::VertexBufferHandle vbh_;

        bgfx::ProgramHandle program_;
    };
}

#endif // CEPHALON_GAME_H_