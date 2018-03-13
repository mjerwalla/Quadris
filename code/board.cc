#include "board.h"
#include <memory>
#include <stdexcept>
#include <algorithm> // for remove_if
#include <fstream>

using namespace std;

Board::Board(int w, int h, int spawnX, int spawnY, int startLevel, string sequence) :
                    gridWidth{w},
                    gridHeight{h},
                    score{0},
                    highScore{0},
                    blockGen{spawnX, spawnY, startLevel, sequence} {
    // Try to read highScore from file
    ifstream hsFile(".quadrisHighScore");
    if (hsFile.is_open()) {
        hsFile >> highScore;
    }
}

void Board::restart() {
    score = 0;
    // clear grid
    for (auto & b : blocks) {
        b->isCleared();
        b->notifyObservers();
    }
    blocks.clear(); // delete contents of vector
    notifyObservers(); // tell displays that I've changed

    // Generate first block and send to displays
    blockGen.reset(); // reset block generator
    currentBlock = blockGen.getNext();
    cleared = true;
    notifyObservers();
    cleared = false;
    currentBlock->notifyObservers();
}

void Board::addBlockObserver(Observer &who) {
    blockGen.addBlockObserver(who);
}

// private function used by left, right, down, rcc, rccw below.
// Must specity the block pointer to act on (e.g. currentBlock).
// This is so move can also be used by showHint.
int Board::move(shared_ptr<Block> &block, const MoveType move, int n, bool updateDisplay/*=true*/) {
    shared_ptr<Block> newBlock = block; // does not create new block, just copies pointer
    
    int j;
    // Repeat move n times
    for (j = 0; j < n; j++) {
        shared_ptr<Block> tempBlock;
        
        switch (move) { // creates new block in  moved position
            case LEFT: tempBlock = newBlock->left(); break;
            case RIGHT: tempBlock = newBlock->right(); break;
            case DOWN: tempBlock = newBlock->down(); break;
            case RCW: tempBlock = newBlock->rcw(); break;
            case RCCW: tempBlock = newBlock->rccw(); break;
            default: return 0; // should never happen
        }

        // Check if block is contained within the grid
        int x = tempBlock->getX();
        int y = tempBlock->getY();
        int w = tempBlock->getWidth();
        int h = tempBlock->getHeight();
        if (x < 0 || x + w > gridWidth || y < 0 || y + h > gridHeight) break;

        // Check newBlock against all the blocks that have already been dropped
        vector<shared_ptr<Block> >::const_iterator it;
        for (it = blocks.begin(); it != blocks.end(); ++it) {
            if (tempBlock->overlap(*(*it))) break; // just breaks to * below
        } // *
        
        // If a collision occurs break (throwing away tempBlock)
        if (it != blocks.end()) break;

        // If no collision occurs (i.e. valid block placement)
        newBlock = tempBlock;
    }

    // Replace current block (i.e. block) with newBlock
    if (newBlock != block) {
        if (updateDisplay) {
            block->setClear();
            block->notifyObservers();
        }
        block = newBlock; // at this point the old block will be deleted
        if (updateDisplay) block->notifyObservers();
    }

    return j; // Return the number of times the move succeeded
}

// LEFT, RIGHT, DOWN, RCW, RCCW, DROP functions called by main()
// All act only on currentBlock (move, above, can act on any block ptr)
int Board::left(int n)  {
    int i = move(currentBlock, LEFT, n);
    if (currentBlock->isHeavy()) move(currentBlock, DOWN, 1);
    return i;
}
int Board::right(int n) {
    int i = move(currentBlock, RIGHT, n);
    if (currentBlock->isHeavy()) move(currentBlock, DOWN, 1);
    return i;
}
int Board::down(int n)  {
    int i = move(currentBlock, DOWN, n);
    if (currentBlock->isHeavy()) move(currentBlock, DOWN, 1);
    return i;
}
int Board::rcw(int n)   {
    int i = move(currentBlock, RCW, n);
    if (currentBlock->isHeavy()) move(currentBlock, DOWN, 1);
    return i;
}
int Board::rccw(int n)  {
    int i = move(currentBlock, RCCW, n);
    if (currentBlock->isHeavy()) move(currentBlock, DOWN, 1);
    return i;
}


int Board::drop(int n) {
    int j;
    // Drop n blocks
    for (j = 0; j < n; j ++) {
        // try to move the block to move down more times than would be possible
        down(gridHeight);
        
        // add block to blocks vector
        blocks.emplace_back(currentBlock);
        
        // generate new block in default spawn position
        currentBlock = blockGen.getNext();

        // check if currentBlock's placement is legal
        // if not, end game and ask user if they want to play again
        if (gameOverCheck()) return j;

        // draw block
        currentBlock->notifyObservers();

        clearRows(); // clear any filled rows

        // If new block is autodropping make loop run an extra time
        if (currentBlock->isAutoDrop()) j --;
    }
    
    notifyObservers(); // update display of board info (score, nextblock, etc.)

    return j; // Return number of blocks successfully dropped
    // Note: will only not return n when the game is over
}

bool Board::gameOverCheck() {
    // check currentBlock against existing blocks and end game if overlap
    for (const auto & b : blocks) {
        if (currentBlock->overlap(*b)) {
            // GAME OVER, LOSER!!
            // dunno how to end, yet. might change this.
            throw(runtime_error("Game over."));
        }
    }
    return false;
}

void Board::requestBlock(char type) {
    if (currentBlock) {
        currentBlock->setClear();
        currentBlock->notifyObservers();
    }
    currentBlock = blockGen.requestBlock(type);
    currentBlock->notifyObservers();
}

void Board::clearRows() {
    int numRowsCleared = 0;

    while (removeFirstFilledRow()) {
        numRowsCleared ++;
    }

    // Add score based on # rows cleared
    if (numRowsCleared > 0) {
        // Score (current level + # rows cleared)^2 points
        score += [](int x){return x*x;}(blockGen.getLevel() + numRowsCleared);
    }

    // Add score for any blocks completely destroyed
    // Use this lambda predicate to indicate whether a block should be removed
    // Also updates score
    auto pred = [&](shared_ptr<Block> b) {
        if (b->getHeight() == 0) {
            // increase score by (originLevel+1)^2 points when block removed
            score += [](int x){return x*x;}(b->getOriginLevel()+1);
            return true;
        }
        return false;
    };
    
    // Remove any blocks that have no rows, according to pred above
    blocks.erase(remove_if(blocks.begin(), blocks.end(), pred), blocks.end());

    // Update high score
    if (score > highScore) {
        highScore = score;
        ofstream hsFile;
        hsFile.open(".quadrisHighScore", ios::out | ios::trunc);
        if (hsFile.is_open()) {
            hsFile << highScore;
        }
    }
}

// Removes the first filled row and returns true. If no row is filled, returns
// false.
bool Board::removeFirstFilledRow() {
    // Check for filled rows
    for (int j = 0; j < gridHeight; j ++) {
        int filledCells = 0;
        for (int k = 0; k < gridWidth; k ++) {
            for (const auto &b : blocks) {
                // Count number of filled cells in a row
                if (b->getCell(k, j)) filledCells ++;
            }
        }

        // Remove a single row if it is filled
        if (filledCells == gridWidth) {
            blockGen.resetBlockCount();

            for (auto it = blocks.begin(); it != blocks.end(); ++it) {
                (*it)->setClear(); // clear original block
                (*it)->notifyObservers();
                
                (*it) = (*it)->removeRow(j); // replace block with new version
                (*it)->setClear(false);
                (*it)->notifyObservers(); // draw new version
            }
            return true;
        }

    }
    return false;
}

void Board::showHint() {
    clearHint(); // reset hint 

    // Simple hint: displays directly below
    //hintBlock = make_shared<Block>(*currentBlock);
    //hintBlock->setHint();
    //move(hintBlock, DOWN, gridHeight, false); // move without clearing old block

    vector<shared_ptr<Block>> hints; // store possible hints here  
   
    // Algorithm: for each of the four possible rotations of currentBlock,
    // create a copy and push it to the far right of the grid. Then drop it
    // as far down as possible and store the result. Move one to the left and
    // drop that, and store the result. Rinse and repeat. Finally, choose the
    // block that has the lowest top (i.e. the lowest value for y + height),
    // breaking any ties by taking the one the farthest to the right.

    for (int rotations = 0; rotations < 4; rotations ++) {
        shared_ptr<Block> baseRotation = make_shared<Block>(*currentBlock);
        move(baseRotation, RCW, rotations, false); // Rotate block 0-3 times
        move(baseRotation, RIGHT, gridWidth, false); // Move to right edge
    
        for (int j = 0; j < gridWidth; j ++) {
            shared_ptr<Block> tempHint = make_shared<Block>(*baseRotation);

            move(tempHint, LEFT, j, false);
            move(tempHint, DOWN, gridHeight, false);

            hints.emplace_back(tempHint);
        }
    }
    
    if (hints.size() < 1) return; // shouldn't happen
    hintBlock = hints.at(0);

    // Select lowest hint block (if two are at same height stick with first)
    for (auto &ptr : hints) {
        if (ptr->getY() + ptr->getHeight() < hintBlock->getY() + hintBlock->getHeight()) hintBlock = ptr;
    }

    // Display hint block
    hintBlock->setHint();
    hintBlock->notifyObservers();
}

void Board::clearHint() {
    // If a hint block exists, clear it from the display and delete it
    if (hintBlock) {
        hintBlock->setClear();
        hintBlock->notifyObservers();
        hintBlock.reset(); // delete block
    }
}

int Board::levelUp(int n) {
    int i = blockGen.levelUp(n);
    notifyObservers();
    return i;
}

int Board::levelDown(int n) {
    int i = blockGen.levelDown(n);
    notifyObservers();
    return i;
}

void Board::random() {
    blockGen.random();
}

void Board::noRandom(string file) {
    blockGen.noRandom(file);
}

int Board::getLevel() const {
    return blockGen.getLevel();
}

int Board::getScore() const {
    return score;
}

int Board::getHighScore() const {
    return highScore;
}

char Board::getNextType() const {
    return blockGen.nextType();
}

bool Board::isCleared() const {
    return cleared;
}
