#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "subject.h"
#include <vector>
#include <memory>
#include <string>
#include "blocktypes.h" // just for the colour-related stuff

class Block : public Subject {
    std::vector<std::vector<bool> > grid;
    int x;
    int y;
    char typeChar;
    blocktypes::Colour typeColour;
    int originLevel;
    bool heavy;
    bool autodrop;
    bool clear;
    int rows, cols; // height and width of grid

  public:
    Block(std::vector<std::vector<bool> > grid,
                                          int x,
                                          int y,
                                          char typeChar,
                                          blocktypes::Colour typeColour,
                                          int originLevel,
                                          bool heavy,
                                          bool autodrop);

    bool getCell(int gx, int gy) const;
    int getHeight() const;
    int getWidth() const;
    char getChar() const;
    blocktypes::Colour getColour() const;
    bool isHeavy() const;
    bool isAutoDrop() const;
    bool isCleared() const;

    void setClear(bool state=true);

    int getX() const;
    int getY() const;
    
    void setX(int k);
    void setY(int k);
    void setGrid(std::vector<std::vector<bool> > k);
    void setHint();

    int getOriginLevel() const;

    std::shared_ptr<Block> left() const;
    std::shared_ptr<Block> right() const;
    std::shared_ptr<Block> down() const;
    std::shared_ptr<Block> rcw() const;
    std::shared_ptr<Block> rccw() const;

    std::shared_ptr<Block> removeRow(int rowNumber) const;

    bool overlap(const Block& other) const;
};
#endif
