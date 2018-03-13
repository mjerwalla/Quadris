#include "textdisplay.h"
#include <vector>
#include <iostream>
#include <iomanip>

#include "blocktypes.h"
#include "block.h"
#include "board.h"

using namespace std;

TextDisplay::TextDisplay(int w, int h) :
                    gridWidth{w},
                    gridHeight{h},
                    charGrid{vector<vector<char> >(h, vector<char>(w, ' '))},
                    level{0},
                    score{0},
                    highScore{0},
                    nextType{'\0'} {}

// Dynamic casting is a bit of a hack
// Maybe want to provide notify() methods specific to the types?

void TextDisplay::notify(Subject &whoNotified) {
    const Block * block;
    const Board * board;
    // Notifier is a Block
    if ((block = dynamic_cast<Block*> (&whoNotified))) {
        // clear method indicates that this block should be cleared from the display
        char fill = (block->isCleared()) ? ' ' : block->getChar();
        int w = block->getWidth();
        int h = block->getHeight();
        int x = block->getX(); // x and y of BOTTOM-LEFT corner of block
        int y = block->getY();


        // Only draw blocks that are fully on the grid.
        if (!(x < 0 || x + w > gridWidth || y < 0 || y + h > gridHeight)) {
            for (int j = 0; j < h; j ++) {
                for (int k = 0; k < w; k ++) {
                    // check value of block cell at x,y (absolute pos)
                    if (block->getCell(x + k, y + j)) {
                        charGrid[gridHeight - 1 - (y + j)][x + k] = fill;
                    }
                }
            }
        }
    }
    // Notifier is a (the) Board
    else if ((board = dynamic_cast<Board*> (&whoNotified))) {
        if (board->isCleared()) {
            for (auto &r : charGrid) {
                for (auto &c : r) {
                    c = ' ';
                }
            }
        }
        
        level = board->getLevel();
        score = board->getScore();
        highScore = board->getHighScore();
        nextType = board->getNextType();
    }
}

ostream & operator<<(ostream & out, const TextDisplay & td) {
    out << "Level:    " << setw(4) << td.level << endl;
    out << "Score:    " << setw(4) << td.score << endl;
    out << "Hi Score: " << setw(4) << td.highScore << endl;
    out << '+' << string(td.charGrid[0].size(), '=') << '+' << endl; // _ separator
    
    // need to reverse this
    for (const auto & r : td.charGrid) {
        out << '|';
        for (const auto & c : r) {
            out << c;
        }
        out << '|' << endl;
    }

    out << '+' << string(td.charGrid[0].size(), '=') << '+' << endl; // _ separator
    out << "Next:" << endl;
    
    // Print next block type
    if (td.nextType) { // if not set to NULL character
        const vector<vector<bool> > & nextGrid = blocktypes::grid(td.nextType);
        for (auto const & r : nextGrid) {
            for (auto const & c : r) {
                out << ((c) ? td.nextType : ' ');
            }
            out << endl;
        }
    }

    return out;
}
