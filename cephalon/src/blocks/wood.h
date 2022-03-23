#ifndef CEPHALON_BLOCKS_WOOD_H_
#define CEPHALON_BLOCKS_WOOD_H_

namespace cephalon {
    class Wood : public Block {
    public:
        virtual glm::vec2 getTexCoord() const override {
            return glm::vec2(0.75f, 0.0f);
        }
    };
}

#endif // CEPHALON_BLOCKS_WOOD_H_