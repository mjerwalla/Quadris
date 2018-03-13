#include "level2.h"

using namespace std; 
using blocktypes::grid;
using blocktypes::colour; 

// Constructor for Level2
Level2::Level2(ifstream *file, int spawnX, int spawnY, int blockCount, char nexType, bool random):
                Level(file, nexType, spawnX, spawnY, blockCount, random) {
                    if (!nextTypeChar) {
                        updateNextChar();
                    }
                }

// updateNextChar() updates nextTypeChar given characters S,Z,I,J,L,O,T have a 
// 1/7 probabilty each to occur next
void Level2::updateNextChar() {
    int num = rand () % 7;
    if (num == 0) {
        nextTypeChar = 'S'; 
    } else if (num == 1) {
        nextTypeChar = 'Z';
    } else if (num == 2) { 
        nextTypeChar = 'I'; 
    } else if (num == 3) {
        nextTypeChar = 'J'; 
    } else if (num == 4) {
        nextTypeChar = 'L'; 
    } else if (num == 5) {
        nextTypeChar = 'O'; 
    } else {
        nextTypeChar = 'T'; 
    }
}

// getNext() returns a smart pointer of type Block which contains the the next
// Block of type nextCharType
std::shared_ptr<Block> Level2::getNext() {
    char newType = nextTypeChar;
    updateNextChar(); 
    blockCount+=1;
    return requestBlock(newType);
}

// requestBlock() accepts a character returns a smart pointer of type Block
//  which is a Block of the accepted character type
shared_ptr<Block> Level2::requestBlock (char type) const {
    return make_shared<Block> (grid(type), spawnX, spawnY, type, colour(type), 2, false, false); 
}
