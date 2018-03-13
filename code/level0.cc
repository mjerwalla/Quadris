#include "level0.h"

using namespace std;
using blocktypes::grid;
using blocktypes::colour; 

#include <iostream>

// Constructor for Level0
Level0::Level0(ifstream *file, int spawnX, int spawnY, int blockCount, char nextType, bool random):
                Level(file, nextType, spawnX, spawnY, blockCount, random) {
                    if (!nextTypeChar) {
                        updateNextChar();
                    }
}

// updateNextChar() checks if the current file has any inputs left, else it 
// reintializes the ifstream to back to the start of the file and returns the
// next character in the file 
void Level0::updateNextChar() {
    if(!(*in >> nextTypeChar)) {
        in->clear();
        in->seekg(0, ios::beg);
        *in >> nextTypeChar;
    }
}

// getNext() returns a smart pointer of type Block which contains the the next
// Block type
shared_ptr<Block> Level0::getNext() {
    char newType = nextTypeChar;
    updateNextChar(); 
    blockCount+=1;
    return requestBlock(newType);
}

// requestBlock() accepts a character returns a smart pointer of type Block
//  which is a Block of the accepted character type
shared_ptr<Block> Level0::requestBlock (char type) const {
    return make_shared<Block> (grid(type), spawnX, spawnY, type, colour(type), 0 , false, false); 
}
