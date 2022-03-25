#ifndef CEPHALON_BLOCKS_DIRT_H_
#define CEPHALON_BLOCKS_DIRT_H_

namespace cephalon {
    class Dirt : public Block {
    public:
        void init(Atlas& atlas) {
            image_ = LoadImage("textures/dirt.dds", static_cast<bimg::TextureFormat::Enum>(Atlas::kFormat));
            region_ = atlas.add(image_);
        }

        virtual Region getRegion() const override {
            return region_;;
        }
    private:
        bimg::ImageContainer* image_;
        Region region_;
    };
}

#endif // CEPHALON_BLOCKS_DIRT_H_