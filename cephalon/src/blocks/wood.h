#ifndef CEPHALON_WOOD_H_
#define CEPHALON_WOOD_H_

namespace cephalon {
    class Wood : public Block {
    public:
        virtual std::uint32_t getColor() const override {
            return 0xff72a4ca;
        }
    };
}

#endif // CEPHALON_STONE_H_