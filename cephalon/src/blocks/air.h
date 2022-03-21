#ifndef CEPHALON_AIR_H_
#define CEPHALON_AIR_H_

namespace cephalon {
    class Air : public Block {
    public:
        virtual bool isAir() const override { 
            return true;
        }

        virtual glm::vec2 getTexCoord() const override {
            return glm::vec2(0.0f, 0.0f);
        }
    };
}

#endif // CEPHALON_AIR_H_