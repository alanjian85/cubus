#ifndef CEPHALON_BLOCKS_STONE_H_
#define CEPHALON_BLOCKS_STONE_H_

namespace cephalon {
    class Stone : public Block {
    public:
        Stone()
            : Block("stone")
        {

        }

        void init() {
            layer_ = loadTile("stone");
        }

        virtual std::uint16_t getRightLayer() const override {
            return layer_;
        }

        virtual std::uint16_t getLeftLayer() const override {
            return layer_;
        }

        virtual std::uint16_t getTopLayer() const override {
            return layer_;
        }

        virtual std::uint16_t getBottomLayer() const override {
            return layer_;
        }

        virtual std::uint16_t getBackLayer() const override {
            return layer_;
        }

        virtual std::uint16_t getFrontLayer() const override {
            return layer_;
        }
    private:
        std::uint16_t layer_;
    };
}

#endif // CEPHALON_BLOCKS_STONE_H_