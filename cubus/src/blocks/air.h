#pragma once

namespace cephalon {
    class Air : public Block {
    public:
        Air() 
            : Block("air")
        {

        }

        virtual bool isAir() const override { 
            return true;
        }

        virtual TextureInfo getRightTexture() const override {
            return TextureInfo(0, glm::ivec2(0));
        }

        virtual TextureInfo getLeftTexture() const override {
            return TextureInfo(0, glm::ivec2(0));
        }

        virtual TextureInfo getTopTexture() const override {
            return TextureInfo(0, glm::ivec2(0));
        }

        virtual TextureInfo getBottomTexture() const override {
            return TextureInfo(0, glm::ivec2(0));
        }

        virtual TextureInfo getBackTexture() const override {
            return TextureInfo(0, glm::ivec2(0));
        }

        virtual TextureInfo getFrontTexture() const override {
            return TextureInfo(0, glm::ivec2(0));
        }
    };
}
