#include "level1.h"

using namespace std; 
using blocktypes::grid;
using blocktypes::colour; 

// Constructor for Level1
Level1::Level1(ifstream *file, int spawnX, int spawnY, int blockCount, char nexType, bool random):
                Level(file, nexType, spawnX, spawnY, blockCount, random) {
                    if (!nextTypeChar) {
                        updateNextChar();
                    }
                }

// updateNextChar() updates nextTypeChar given characters S and Z have a 
// 1/12 pobability to occur next while I,J,L,O,T have a 1/6 probabilty 
// each to occur next
void Level1::updateNextChar() {
    double randNum = rand() % 12;
    if (randNum == 0) {
        nextTypeChar = 'S'; 
    } else if (randNum == 1) {
        nextTypeChar = 'Z';
    } else if (randNum == 2 || randNum == 3) {
        nextTypeChar = 'I'; 
    } else if (randNum == 4 || randNum == 5) {
        nextTypeChar = 'J'; 
    } else if (randNum == 6 || randNum == 7) {
        nextTypeChar = 'L'; 
    } else if (randNum == 9 || randNum == 9) {
        nextTypeChar = 'O'; 
    } else {
        nextTypeChar = 'T'; 
    }
}


// getNext() returns a smart pointer of type Block which contains the the next
// Block of type nextCharType
shared_ptr<Block> Level1::getNext() {
    char newType = nextTypeChar;
    updateNextChar();
    blockCount+=1;
    return requestBlock(newType);
}

// requestBlock() accepts a character returns a smart pointer of type Block
//  which is a Block of the accepted character type
shared_ptr<Block> Level1::requestBlock (char type) const {
    return make_shared<Block> (grid(type), spawnX, spawnY, type, colour(type), 1 , false, false); 
}
