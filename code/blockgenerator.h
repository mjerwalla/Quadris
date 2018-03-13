#ifndef _BLOCKGENERATOR_H_
#define _BLOCKGENERATOR_H_

#include "block.h"
#include "level.h"
#include "level0.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "observer.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

class BlockGenerator { 
    int spawnX, spawnY, levelNum, startLevelNum;
    std::ifstream in; 
    string startFile;
    std::shared_ptr<Level> currLevel;  
    vector<Observer*> observerList; 

public: 
    BlockGenerator(int spawnX,
                   int spawnY,
                   int levelNum,
                   std::string file);

    std::shared_ptr<Block> getNext();
    std::shared_ptr<Block> requestBlock(char type); 
    char nextType() const;
    int getLevel() const; 
    
    int levelUp(int n);
    int levelDown(int n);
    void random(); 
    void noRandom(std::string file);

    void resetFile(std::string file);
    void resetBlockCount();
    void reset();
    
    void addBlockObserver(Observer &who);

};
#endif /* _BLOCKGENERATOR_H_ */
