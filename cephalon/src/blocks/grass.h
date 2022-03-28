#ifndef CEPHALON_BLOCKS_GRASS_H_
#define CEPHALON_BLOCKS_GRASS_H_

namespace cephalon {
    class Grass : public Block {
    public:
        Grass()
            : Block("grass")
        {

        }

        void init(Atlas& atlas) {
            side_image_ = LoadImage("blocks/grass_side", static_cast<bimg::TextureFormat::Enum>(Atlas::kFormat));
            top_image_ = LoadImage("blocks/grass_top", static_cast<bimg::TextureFormat::Enum>(Atlas::kFormat));
            bottom_image_ = LoadImage("blocks/grass_bottom", static_cast<bimg::TextureFormat::Enum>(Atlas::kFormat));
            side_region_ = atlas.add(side_image_);
            top_region_ = atlas.add(top_image_);
            bottom_region_ = atlas.add(bottom_image_);
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
        static inline bimg::ImageContainer* side_image_;
        static inline bimg::ImageContainer* top_image_;
        static inline bimg::ImageContainer* bottom_image_;
        static inline Region side_region_;
        static inline Region top_region_;
        static inline Region bottom_region_;
    };
}

#endif // CEPHALON_BLOCKS_GRASS_H_