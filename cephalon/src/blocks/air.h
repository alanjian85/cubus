#ifndef CEPHALON_AIR_H_
#define CEPHALON_AIR_H_

namespace cephalon {
    class Air : public Block {
    public:
        virtual bool isAir() const override { 
            return true;
        }

        virtual std::uint32_t getColor() const override {
            return 0x00000000;
        }
    };
}

#endif // CEPHALON_AIR_H_