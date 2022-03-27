#ifndef CEPHALON_BLOCKS_SAND_H_
#define CEPHALON_BLOCKS_SAND_H_

namespace cephalon {
    class Sand : public Block {
    public:
        void init(Atlas& atlas) {
            image_ = LoadImage("textures/blocks/sand.dds", static_cast<bimg::TextureFormat::Enum>(Atlas::kFormat));
            region_ = atlas.add(image_);
        }

        virtual Region getRegion() const override {
            return region_;;
        }

        virtual const char* getName() const override {
            return "sand";
        }
    private:
        static inline bimg::ImageContainer* image_;
        static inline Region region_;
    };
}

#endif // CEPHALON_BLOCKS_SAND_H_