#ifndef CEPHALON_CONFIG_H_
#define CEPHALON_CONFIG_H_

namespace cephalon {
    class Config {
    public:
        static constexpr int kViewDistance = 5;
        static constexpr int kDestroyDistance = 5;
        static constexpr int kChunkLoadLimit = 4;
        static constexpr int kChunkRebuildLimit = 8;
    };
}

#endif // CEPHALON_CONFIG_H_