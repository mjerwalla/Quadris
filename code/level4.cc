#include "level4.h"

using namespace std; 
using blocktypes::grid;
using blocktypes::colour; 

// Constructor for Level4
Level4::Level4(ifstream *file,
               int spawnX,
               int spawnY,
               int blockCount,
               char nextType,
               bool random) : Level3(file,
                                     spawnX,
                                     spawnY,
                                     blockCount,
                                     nextType,
                                     random) {}

// updateNextChat() calls Level3's implementation of updateNextChar
void Level4::updateNextChar() {
    Level3::updateNextChar();
}

// getNext() returns a smart pointer of type Block which contains of either a 
// Block of type '*' if 5 ordinary blocks have already been passed or
// a Block of type nextCharType
std::shared_ptr<Block> Level4::getNext() {
    if (blockCount == 5) {
        blockCount = 0;
        shared_ptr<Block> nextBlock = make_shared<Block>(grid('*'), 5, 14, '*', colour('*'), 4, false, true);
        return nextBlock;
    }
    blockCount++;
    char newType = nextTypeChar; 
    updateNextChar(); 
    return requestBlock(newType);
}

// requestBlock() accepts a character returns a smart pointer of type Block
//  which is a Block of the accepted character type
shared_ptr<Block> Level4::requestBlock (char type) const {
    return make_shared<Block> (grid(type), spawnX, spawnY, type, colour(type), 4, true, false); 
}
