#ifndef _LEVEL4_H_
#define _LEVEL4_H_  

#include "level3.h"

class Level4 : public Level3 {
    public:
        Level4(std::ifstream *file,
               int spawnX,
               int spawnY,
               int blockCount,
               char nextType = '\0',
               bool random = true);
        void updateNextChar() override;
        std::shared_ptr<Block> getNext() override; 
        std::shared_ptr<Block> requestBlock(char type) const override;
};
#endif
