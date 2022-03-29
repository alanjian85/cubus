#ifndef CEPHALON_BLOCKS_SAND_H_
#define CEPHALON_BLOCKS_SAND_H_

namespace cephalon {
    class Sand : public Block {
    public:
        Sand()
            : Block("sand")
        {
        
        }

        void init() {
            region_ = loadTile("sand");
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

#endif // CEPHALON_BLOCKS_SAND_H_