#include "block.h"

#include "blocktypes.h" // just for the colour related stuff

using namespace std;

Block::Block(vector<vector<bool> > grid,
             int x,
             int y, 
             char typeChar,
             blocktypes::Colour typeColour,
             int originLevel,
             bool heavy,
             bool autodrop) : grid{grid},
                              x{x},
                              y{y},
                              typeChar{typeChar},
                              typeColour{typeColour},
                              originLevel{originLevel},
                              heavy{heavy},
                              autodrop{autodrop},
                              clear{false},
                              rows{static_cast<int>(grid.size())},
                              cols{(rows > 0) ? static_cast<int>(grid[0].size()) : 0} {}


int Block::getOriginLevel() const {
    return originLevel;
}

bool Block::isHeavy() const {
    return heavy;
}

bool Block::isAutoDrop() const {
    return autodrop;
}

//returns ptr to new block shifted one unit left
shared_ptr<Block> Block::left() const {
    shared_ptr<Block> ptr = make_shared<Block>(*this);
    ptr->setX(x - 1);
    return ptr; 
}

//returns ptr to new block shifted one unit right
shared_ptr<Block> Block::right() const {
    shared_ptr<Block> ptr = make_shared<Block>(*this);
    ptr->setX(x + 1);
    return ptr;
}


//returns ptr to new block shifted one unit down
shared_ptr<Block> Block::down() const {
    shared_ptr<Block> ptr = make_shared<Block>(*this);
    ptr->setY(y - 1);
    return ptr;
}

//returns ptr to new block rotated clockwise 90 degrees 
shared_ptr<Block> Block::rcw() const {
    // Initialize new grid with width and height swapped
    vector<vector<bool> > newGrid(cols, vector<bool>(rows, 0));
    for (int i = 0; i < rows; i ++) {
        for (int j = 0; j < cols; j ++) {
            newGrid[j][i] = grid[rows - 1 -i][j]; // puts CW rotated version into newgrid
        }
    }

    shared_ptr<Block> ptr = make_shared<Block>(*this);
    ptr->setGrid(newGrid);
    return ptr;
}

//returns ptr to new block rotated counterclockwise 90 degrees
shared_ptr<Block> Block::rccw() const {
    // Initialize new grid with width and height swapped
    vector<vector<bool> > newGrid(cols, vector<bool>(rows, 0));

    for (int i = 0; i < rows; i ++) {
        for (int j = 0; j < cols; j ++) {
            newGrid[j][i] = grid[i][cols - 1 - j]; // puts CCW-rotated version into newgrid
        }
    }
    shared_ptr<Block> ptr = make_shared<Block>(*this);
    ptr->setGrid(newGrid);
    return ptr;
}



bool Block::getCell(int gx, int gy) const {  
    int bCol = gx - x;
    int bRow = (getHeight() - 1) - (gy - y);
    
    if (bCol < 0 || bCol >= cols || bRow < 0 || bRow >= rows) return false;
    return grid[bRow][bCol];
}


int Block::getWidth() const {
    return cols;
}

int Block::getHeight() const {
    return rows;
}

int Block::getX() const {
    return x;
}

int Block::getY() const {
    return y;
}

void Block::setX(int k) {
    x = k;
}

void Block::setY(int k) {
    y = k;
}

void Block::setHint() {
    typeChar = '?';
    typeColour = blocktypes::Black;
}

void Block::setGrid(vector<vector<bool> > k) {
    grid = k;
    rows = static_cast<int>(grid.size());
    cols = (rows > 0) ? static_cast<int>(grid[0].size()) : 0;
}

//determines if collision occurs between this and other
bool Block::overlap(const Block &other) const {
    for (int j = 0; j < rows; j ++) {
        for (int k = 0; k < cols; k ++) {
            if (grid[j][k] && other.getCell(x + k, y + rows - 1 - j)) return true;
        }
    }
    return false;
}


std::shared_ptr<Block> Block::removeRow(int rowNumber) const {
    shared_ptr<Block> ptr = make_shared<Block>(*this); // copy block
    
    // If row to be removed is below the block
    if (rowNumber < y) {
        ptr->setY(y - 1);
    }
    // If row to be removed is in the block
    else if (rowNumber >= y && rowNumber < y + rows) {
        vector<vector<bool> > newGrid = grid;
        newGrid.erase(newGrid.begin() + (rows - 1) - (rowNumber - y));
        ptr->setGrid(newGrid);
    }
    // If row to be removed is above the block, return an unchanged copy
    // of the block
    return ptr;
}

void Block::setClear(bool state) {
    clear = state;
}

bool Block::isCleared() const {
    return clear;
}

char Block::getChar() const {
    return typeChar;
}

blocktypes::Colour Block::getColour() const {
    return typeColour;
}
