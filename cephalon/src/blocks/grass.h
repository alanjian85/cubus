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
            side_region_ = loadTile("grass_side");
            top_region_ = loadTile("grass_top");
            bottom_region_ = loadTile("grass_bottom");
        }

        virtual Region getRightRegion() const override {
            return side_region_;;
        }

        virtual Region getLeftRegion() const override {
            return side_region_;;
        }

        virtual Region getTopRegion() const override {
            return top_region_;;
        }

        virtual Region getBottomRegion() const override {
            return bottom_region_;;
        }

        virtual Region getBackRegion() const override {
            return side_region_;;
        }

        virtual Region getFrontRegion() const override {
            return side_region_;;
        }
    private:
        Region side_region_;
        Region top_region_;
        Region bottom_region_;
    };
}

#endif // CEPHALON_BLOCKS_GRASS_H_