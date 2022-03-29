#ifndef CEPHALON_BLOCKS_DIRT_H_
#define CEPHALON_BLOCKS_DIRT_H_

namespace cephalon {
    class Dirt : public Block {
    public:
        Dirt()
            : Block("dirt")
        {

        }

        void init() {
            region_ = loadTile("dirt");
        }

        virtual Region getRightRegion() const override {
            return region_;
        }

        virtual Region getLeftRegion() const override {
            return region_;
        }

        virtual Region getTopRegion() const override {
            return region_;
        }

        virtual Region getBottomRegion() const override {
            return region_;
        }

        virtual Region getBackRegion() const override {
            return region_;
        }

        virtual Region getFrontRegion() const override {
            return region_;
        }
    private:
        bimg::ImageContainer* image_;
        Region region_;
    };
}

#endif // CEPHALON_BLOCKS_DIRT_H_