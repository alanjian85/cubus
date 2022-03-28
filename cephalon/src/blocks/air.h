#ifndef CEPHALON_BLOCKS_AIR_H_
#define CEPHALON_BLOCKS_AIR_H_

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

        virtual Region getRightRegion() const override {
            return Region(glm::ivec2(0), glm::ivec2(0));
        }

        virtual Region getLeftRegion() const override {
            return Region(glm::ivec2(0), glm::ivec2(0));
        }

        virtual Region getTopRegion() const override {
            return Region(glm::ivec2(0), glm::ivec2(0));
        }

        virtual Region getBottomRegion() const override {
            return Region(glm::ivec2(0), glm::ivec2(0));
        }

        virtual Region getBackRegion() const override {
            return Region(glm::ivec2(0), glm::ivec2(0));
        }

        virtual Region getFrontRegion() const override {
            return Region(glm::ivec2(0), glm::ivec2(0));
        }
    };
}

#endif // CEPHALON_BLOCKS_AIR_H_