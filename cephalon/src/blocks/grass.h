#ifndef CEPHALON_BLOCKS_GRASS_H_
#define CEPHALON_BLOCKS_GRASS_H_

namespace cephalon {
    class Grass : public Block {
    public:
        Grass()
            : Block("grass")
        {

        }

        void init() {
            side_layer_ = loadTile("grass_side");
            top_layer_ = loadTile("grass_top");
            bottom_layer_ = loadTile("grass_bottom");
        }

        virtual std::uint16_t getRightLayer() const override {
            return side_layer_;
        }

        virtual std::uint16_t getLeftLayer() const override {
            return side_layer_;
        }

        virtual std::uint16_t getTopLayer() const override {
            return top_layer_;
        }

        virtual std::uint16_t getBottomLayer() const override {
            return bottom_layer_;
        }

        virtual std::uint16_t getBackLayer() const override {
            return side_layer_;
        }

        virtual std::uint16_t getFrontLayer() const override {
            return side_layer_;
        }
    private:
        std::uint16_t side_layer_;
        std::uint16_t top_layer_;
        std::uint16_t bottom_layer_;
    };
}

#endif // CEPHALON_BLOCKS_GRASS_H_