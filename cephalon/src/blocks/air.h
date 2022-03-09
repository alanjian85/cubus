#ifndef CEPHALON_AIR_H_
#define CEPHALON_AIR_H_

namespace cephalon {
    class Air : public Block {
    public:
        virtual bool isAir() const override { 
            return false;
        }
    };
}

#endif // CEPHALON_AIR_H_