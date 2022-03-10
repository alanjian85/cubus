#ifndef CEPHALON_DIRT_H_
#define CEPHALON_DIRT_H_

namespace cephalon {
    class Dirt : public Block {
    public:
        virtual std::uint32_t getColor() const override {
            return 0xff396583;
        }
    };
}

#endif // CEPHALON_DIRT_H_