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
            region_ = loadTile("wood");
            side_region_ = loadTile("wood_side");
        }

        virtual Region getRightRegion() const override {
            return side_region_;
        }

        virtual Region getLeftRegion() const override {
            return side_region_;
        }
        
        virtual Region getTopRegion() const override {
            return region_;
        }
        
        virtual Region getBottomRegion() const override {
            return region_;
        }
        
        virtual Region getBackRegion() const override {
            return side_region_;
        }
        
        virtual Region getFrontRegion() const override {
            return side_region_;
        }
    private:
        static inline bimg::ImageContainer* image_;
        static inline bimg::ImageContainer* side_image_;
        static inline Region region_;
        static inline Region side_region_;
    };
}

#endif // CEPHALON_BLOCKS_WOOD_H_