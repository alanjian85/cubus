#ifndef CEPHALON_STONE_H_
#define CEPHALON_STONE_H_

namespace cephalon {
    class Stone : public Block {
    public:
        virtual std::uint32_t getColor() const override {
            return 0xff8d8c88;
        }
    };
}

#endif // CEPHALON_STONE_H_