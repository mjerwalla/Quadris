#include "level3.h"
using namespace std; 
using blocktypes::grid;
using blocktypes::colour; 

// Constructor for Level3
Level3::Level3(ifstream *file, int spawnX, int spawnY, int blockCount, char nextType, bool random):
               Level(file, nextType, spawnX, spawnY, blockCount, random) {
                if (!nextTypeChar) {
                    updateNextChar();
                }
            }

// // updateNextChar() either updates nextTypeChar given characters S and Z 
// have a 2/9 pobability to occur next while I,J,L,O,T have a 1/9 probabilty 
// each to occur next or checks if the current file has any inputs left,
// else it reintializes the ifstream to back to the start of the file and
// returns the next character in the file
void Level3::updateNextChar() {
    if (random) {
        int randNum = rand() % 8;
        if (randNum == 0 || randNum == 1) {
            nextTypeChar = 'S'; 
        } else if (randNum == 2 || randNum ==3) {
            nextTypeChar = 'Z'; 
        } else if (randNum == 4) {
            nextTypeChar = 'I'; 
        } else if (randNum == 5) {
            nextTypeChar = 'O'; 
        } else if (randNum == 6) {
            nextTypeChar = 'T'; 
        } else if (randNum == 7) {
            nextTypeChar = 'J';
        } else {
            nextTypeChar = 'L';
        }
    } else {
        if(!(*in >> nextTypeChar)) {
            in->clear();
            in->seekg(0, ios::beg);
            *in >> nextTypeChar;
        }
    }
}

// getNext() returns a smart pointer of type Block which contains the the next
// Block of type nextCharType
std::shared_ptr<Block> Level3::getNext() {
    char newType = nextTypeChar;    
    updateNextChar();
    blockCount+=1;
    return requestBlock(newType); 
}

// requestBlock() accepts a character returns a smart pointer of type Block
//  which is a Block of the accepted character type
shared_ptr<Block> Level3::requestBlock (char type) const {
    return make_shared<Block> (grid(type), spawnX, spawnY, type, colour(type), 3, true, false); 
}
