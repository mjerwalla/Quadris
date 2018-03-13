#ifndef _LEVEL2_H_
#define _LEVEL2_H_ 

#include "level.h"

class Level2: public Level {
    public:
        Level2(std::ifstream *file,
               int spawnX,
               int spawnY,
               int blockCount,
               char nexType = '\0',
               bool random = true);
        void updateNextChar() override;
        std::shared_ptr<Block> getNext() override;
        std::shared_ptr<Block> requestBlock(char type) const override; 
};
#endif
