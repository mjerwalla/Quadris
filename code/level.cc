#include "level.h"

using namespace std;

// Constructor for Level
Level::Level(ifstream *file,
             char nextType,
             int spawnX,
             int spawnY,
             int blockCount,
             bool random) : in{file},
                            nextTypeChar{nextType},
                            spawnX{spawnX},
                            spawnY{spawnY},
                            blockCount{blockCount},
                            random{random} {}

// nextType() returns the value of variable nextTypeChar
char Level::nextType() {
    return nextTypeChar;
}

// resetBlockCount() resets the Block Count (blockCount) back to 0
void Level::resetBlockCount() {
    blockCount = 0;
}

// setRandom(bool) accepts a bool and mutates the state of random to equal 
// the argument bool
void Level::setRandom(bool newRandom) {
    random = newRandom; 
}

// setInStream(ifstream) accepts an ifstream object and modifies in to point 
// the new file ifstream object
void Level::setInStream(ifstream *file) {
    in = file;
}
