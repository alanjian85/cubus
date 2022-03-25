#ifndef CEPHALON_BLOCKS_AIR_H_
#define CEPHALON_BLOCKS_AIR_H_

namespace cephalon {
    class Air : public Block {
    public:
        virtual bool isAir() const override { 
            return true;
        }

        virtual Region getRegion() const override {
            return Region(glm::ivec2(0), glm::ivec2(0));
        }
    };
}

#endif // CEPHALON_BLOCKS_AIR_H_