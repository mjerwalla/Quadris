#include "blockgenerator.h"

using namespace std; 

// Constructor for BlockGenerator
BlockGenerator::BlockGenerator(int spawnX,
                               int spawnY,
                               int levelNum,
                               string file): spawnX{spawnX},
                                             spawnY{spawnY},
                                             levelNum{levelNum},
                                             startLevelNum{levelNum},
                                             in(file),
                                             startFile(file) {
                                                reset();
                                            }

// levelUp(int) increments the level of the game
int BlockGenerator::levelUp(int n) {
    char c = currLevel->nextType();
    for (int i = 0; i < n; i ++) {
        if (levelNum == 0) {
            currLevel = make_shared<Level1> (&in, spawnX, spawnY, 0, c, true); 
            levelNum+=1;
        } else if (levelNum == 1) {
            currLevel = make_shared<Level2> (&in, spawnX, spawnY, 0, c, true); 
            levelNum+=1;
        } else if (levelNum == 2) {
            currLevel = make_shared<Level3> (&in, spawnX, spawnY, 0, c, true); 
            levelNum+=1;
        } else if (levelNum == 3) {
            currLevel = make_shared<Level4> (&in, spawnX, spawnY, 0, c, true); 
            levelNum+=1;
        }
    }
    return levelNum;
}

// levelDown(int) decrements the level of the game
int BlockGenerator::levelDown(int n) {
    char c = currLevel->nextType();
    for (int i = 0; i < n; i ++) {
        if (levelNum == 4) {
            currLevel = make_shared<Level3> (&in, spawnX, spawnY, 0, c, true); 
            levelNum-=1;
        } else if (levelNum == 3) {
            currLevel = make_shared<Level2> (&in, spawnX, spawnY, 0, c, true); 
            levelNum-=1;
        } else if (levelNum == 2) {
            currLevel = make_shared<Level1> (&in, spawnX, spawnY, 0, c, true);  
            levelNum-=1;
        } else if (levelNum == 1) {
            resetFile(startFile);
            currLevel = make_shared<Level0> (&in, spawnX, spawnY, 0, c, true);  
            levelNum-=1;
        }
    }
    return levelNum;
}

// nextType() returns the next type of character
char BlockGenerator::nextType() const {
    char type = currLevel->nextType();
    return type; 
}

// getNext() returns a Block of the next type of character
std::shared_ptr<Block> BlockGenerator::getNext() {
    shared_ptr<Block> newBlock = currLevel->getNext(); 
    for (const auto &p : observerList) {
        newBlock->attachObserver(*p);
    }

    return newBlock;
}

// requestBlock(char) requests a Block of type char
std::shared_ptr<Block> BlockGenerator::requestBlock(char type) {
    shared_ptr<Block> newBlock = currLevel->requestBlock(type);
    for (const auto &p : observerList) {
        newBlock->attachObserver(*p);
    }
    
    return newBlock;
}

// resetBlockCount() reset the Block Count
void BlockGenerator::resetBlockCount() {
    currLevel->resetBlockCount();
}

// getLevel() returns the current Level of the game
int BlockGenerator::getLevel() const {
    return levelNum; 
}

// random() randomizes the level
void BlockGenerator::random() {
    currLevel->setRandom(true); 
}

// resetFile(string) resets the fstream object to point to the 
// start of the file
void BlockGenerator::resetFile(string file) {
    in.clear();
    in.close();
    in.open(file);
}

// noRandom(string) triggers Level3 and Level4 to accept input from "file" 
// instead of randomizing it
void BlockGenerator::noRandom(string file) {
    resetFile(file);
    currLevel->setInStream(&in);
    currLevel->setRandom(false);
}

// addBlockObserver(Observer) adds observers to a vectors of Observers that
// will be added to each block 
void BlockGenerator::addBlockObserver(Observer &who) {
    observerList.emplace_back(&who); 
}

// reset() resets the game
void BlockGenerator::reset() {
    resetFile(startFile);
    levelNum = startLevelNum;
    if (levelNum <= 0) {
        currLevel = make_shared<Level0> (&in, spawnX, spawnY, 0, '\0', true); 
    } else if (levelNum == 1) {
        currLevel = make_shared<Level1> (&in, spawnX, spawnY, 0, '\0', true); 
    } else if (levelNum == 2) {
        currLevel = make_shared<Level2> (&in, spawnX, spawnY, 0, '\0', true); 
    } else if (levelNum == 3) {
        currLevel = make_shared<Level3> (&in, spawnX, spawnY, 0, '\0', true); 
    } else {
        currLevel = make_shared<Level4> (&in, spawnX, spawnY, 0, '\0', true); 
    }   
}
