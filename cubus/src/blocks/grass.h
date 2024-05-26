#pragma once

namespace cephalon {
    class Grass : public Block {
    public:
        Grass()
            : Block("grass")
        {

        }

        void init() {
            side_texture_ = loadTexture("grass_side");
            top_texture_ = loadTexture("grass_top");
            bottom_texture_ = loadTexture("grass_bottom");
        }

        virtual TextureInfo getRightTexture() const override {
            return side_texture_;
        }

        virtual TextureInfo getLeftTexture() const override {
            return side_texture_;
        }

        virtual TextureInfo getTopTexture() const override {
            return top_texture_;
        }

        virtual TextureInfo getBottomTexture() const override {
            return bottom_texture_;
        }

        virtual TextureInfo getBackTexture() const override {
            return side_texture_;
        }

        virtual TextureInfo getFrontTexture() const override {
            return side_texture_;
        }
    private:
        TextureInfo side_texture_;
        TextureInfo top_texture_;
        TextureInfo bottom_texture_;
    };
}
