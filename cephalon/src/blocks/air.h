#ifndef CEPHALON_BLOCKS_AIR_H_
#define CEPHALON_BLOCKS_AIR_H_

namespace cephalon {
    class Air : public Block {
    public:
        Air() 
            : Block("air")
        {

        }

        virtual bool isAir() const override { 
            return true;
        }

        virtual std::uint16_t getRightLayer() const override {
            return 0;
        }

        virtual std::uint16_t getLeftLayer() const override {
            return 0;
        }

        virtual std::uint16_t getTopLayer() const override {
            return 0;
        }

        virtual std::uint16_t getBottomLayer() const override {
            return 0;
        }

        virtual std::uint16_t getBackLayer() const override {
            return 0;
        }

        virtual std::uint16_t getFrontLayer() const override {
            return 0;
        }
    };
}

#endif // CEPHALON_BLOCKS_AIR_H_