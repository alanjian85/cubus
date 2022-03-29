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
            region_ = loadTile("stone");
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
        Region region_;
    };
}

#endif // CEPHALON_BLOCKS_STONE_H_