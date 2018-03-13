#ifndef _LEVEL0_H_
#define _LEVEL0_H_

#include "level.h"

class Level0: public Level {
public:
    Level0(std::ifstream *file,
           int spawnX,
 	   int spawnY,
	   int blockCount, 
	   char nexType ='\0',
	   bool random = true);
    void updateNextChar() override;
    std::shared_ptr<Block> getNext() override;
    std::shared_ptr<Block> requestBlock(char type) const override; 
};
#endif
