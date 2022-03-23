#ifndef CEPHALON_BLOCKS_STONE_H_
#define CEPHALON_BLOCKS_STONE_H_

namespace cephalon {
    class Stone : public Block {
    public:
        virtual glm::vec2 getTexCoord() const override {
            return glm::vec2(0.5f, 0.0f);
        }
    };
}

#endif // CEPHALON_BLOCKS_STONE_H_