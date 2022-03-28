#ifndef CEPHALON_BLOCKS_WOOD_H_
#define CEPHALON_BLOCKS_WOOD_H_

namespace cephalon {
    class Wood : public Block {
    public:
        Wood()
            : Block("wood")
        {
            
        }

        void init(Atlas& atlas) {
            image_ = LoadImage("blocks/wood", static_cast<bimg::TextureFormat::Enum>(Atlas::kFormat));
            region_ = atlas.add(image_);
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
        static inline bimg::ImageContainer* image_;
        static inline Region region_;
    };
}

#endif // CEPHALON_BLOCKS_WOOD_H_