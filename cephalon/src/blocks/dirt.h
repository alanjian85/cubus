#ifndef CEPHALON_DIRT_H_
#define CEPHALON_DIRT_H_

namespace cephalon {
    class Dirt : public Block {
    public:
        virtual glm::vec2 getTexCoord() const override {
            return glm::vec2(0.0f, 0.0f);
        }
    };
}

#endif // CEPHALON_DIRT_H_