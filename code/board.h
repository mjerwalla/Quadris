#ifndef _BOARD_H_
#define _BOARD_H_

#include "block.h"
#include "blockgenerator.h"
#include "subject.h"
#include <vector>
#include <memory>

class Board : public Subject {
	
    std::vector<std::shared_ptr<Block> > blocks;
    std::shared_ptr<Block> currentBlock;
    std::shared_ptr<Block> hintBlock;

    const int gridWidth;
	const int gridHeight;

	int score;
	int highScore;

    BlockGenerator blockGen;

    enum MoveType { LEFT, RIGHT, DOWN, RCW, RCCW };
    int move(std::shared_ptr<Block> &block, const MoveType move, int n, bool updateDisplay=true);
    bool gameOverCheck();
    void clearRows();
    bool removeFirstFilledRow();

    bool cleared;
public:
    Board(int w, int h, int spawnX, int spawnY, int startLevel, std::string sequence);

    void restart();

    void addBlockObserver(Observer &who); // BlockGenerator will add these
                                          // observers when blocks are created
    int left(int n);
	int right(int n);
	int down(int n);
	int rcw(int n);  // rotate clockwise
	int rccw(int n); // rotate counter clockwise

	int drop(int n);

	void random();
    void noRandom(std::string file);
	int levelUp(int n);
	int levelDown(int n);
	
    void showHint();
    void clearHint();
    void requestBlock(char type);

    int getLevel() const;
    int getScore() const;
    int getHighScore() const;
    char getNextType() const;

    bool isCleared() const;
};

#endif /* _BOARD_H_ */
