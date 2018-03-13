#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "block.h"
#include "blocktypes.h"
#include <memory>
#include <fstream>
#include <iostream>
#include <cstdlib>

class Level {
protected: 
    std::ifstream *in;
    char nextTypeChar;
    int spawnX, spawnY; 
    int blockCount;
    bool random;
public:
    Level(std::ifstream *file, 
	  char nextType,
          int spawnX, 
	  int spwanY, 
	  int blockCount, 
	  bool random = true);
    char nextType(); 
    void resetBlockCount();
    void setRandom(bool newRandom);
    void setInStream(std::ifstream *file);
    virtual void updateNextChar() = 0;
    virtual std::shared_ptr<Block> getNext() =0;
    virtual std::shared_ptr<Block> requestBlock(char type) const =0;
};
#endif 
