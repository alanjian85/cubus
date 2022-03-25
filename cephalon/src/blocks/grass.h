#ifndef CEPHALON_BLOCKS_GRASS_H_
#define CEPHALON_BLOCKS_GRASS_H_

namespace cephalon {
    class Grass : public Block {
    public:
        void init(Atlas& atlas) {
            image_ = LoadImage("textures/blocks/grass.dds", static_cast<bimg::TextureFormat::Enum>(Atlas::kFormat));
            region_ = atlas.add(image_);
        }

        virtual Region getRegion() const override {
            return region_;;
        }
    private:
        static inline bimg::ImageContainer* image_;
        static inline Region region_;
    };
}

#endif // CEPHALON_BLOCKS_GRASS_H_