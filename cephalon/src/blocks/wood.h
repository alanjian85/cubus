#ifndef CEPHALON_BLOCKS_WOOD_H_
#define CEPHALON_BLOCKS_WOOD_H_

namespace cephalon {
    class Wood : public Block {
    public:
        Wood()
            : Block("wood")
        {
            
        }

        void init() {
            layer_ = loadTile("wood");
            side_layer_ = loadTile("wood_side");
        }

        virtual std::uint16_t getRightLayer() const override {
            return side_layer_;
        }

        virtual std::uint16_t getLeftLayer() const override {
            return side_layer_;
        }

        virtual std::uint16_t getTopLayer() const override {
            return layer_;
        }

        virtual std::uint16_t getBottomLayer() const override {
            return layer_;
        }

        virtual std::uint16_t getBackLayer() const override {
            return side_layer_;
        }

        virtual std::uint16_t getFrontLayer() const override {
            return side_layer_;
        }
    private:
        std::uint16_t layer_;
        std::uint16_t side_layer_;
    };
}

#endif // CEPHALON_BLOCKS_WOOD_H_