#pragma once

namespace cephalon {
    class Wood : public Block {
    public:
        Wood()
            : Block("wood")
        {
            
        }

        void init() {
            texture_ = loadTexture("wood");
            side_texture_ = loadTexture("wood_side");
        }

        virtual TextureInfo getRightTexture() const override {
            return side_texture_;
        }

        virtual TextureInfo getLeftTexture() const override {
            return side_texture_;
        }

        virtual TextureInfo getTopTexture() const override {
            return texture_;
        }

        virtual TextureInfo getBottomTexture() const override {
            return texture_;
        }

        virtual TextureInfo getBackTexture() const override {
            return side_texture_;
        }

        virtual TextureInfo getFrontTexture() const override {
            return side_texture_;
        }
    private:
        TextureInfo texture_;
        TextureInfo side_texture_;
    };
}
