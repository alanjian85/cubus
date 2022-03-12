#ifndef CEPHALON_GRASS_H_
#define CEPHALON_GRASS_H_

namespace cephalon {
    class Grass : public Block {
    public:
        virtual std::uint32_t getColor() const override {
            return 0xff50c87e;
        }
    };
}

#endif // CEPHALON_DIRT_H_