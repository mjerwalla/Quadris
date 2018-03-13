#include "graphicsdisplay.h"
#include "window.h"

#include "blocktypes.h"

#include "block.h"
#include "board.h"

using namespace std;

GraphicsDisplay::GraphicsDisplay(int w, int h, int cellSize, int topBar, int bottomBar, blocktypes::Colour bgColour) :
                    gridWidth{w},
                    gridHeight{h},
                    window(w*cellSize, h*cellSize + topBar + bottomBar),
                    cellSize{cellSize},
                    gridX{0},
                    gridY{topBar + h*cellSize},
                    topBar{topBar},
                    bottomBar{bottomBar},
                    bgColour{bgColour} {}

void GraphicsDisplay::notify(Subject & whoNotified) {
    const Block * block;
    const Board * board;

    if ((block = dynamic_cast<Block*> (&whoNotified))) {
        // clear method indicates that this block should be cleared from the display
        blocktypes::Colour fill = (block->isCleared()) ?
                                   bgColour :
                                   block->getColour();
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
                        //charGrid[gridH - 1 - (y + j)][x + k] = fill;
                        window.fillRectangle(gridX + (k + x)*cellSize,
                                             gridY - (j + y + 1)*cellSize,
                                             cellSize,
                                             cellSize,
                                             fill);
                    }
                }
            }
        }
    }
    // Notifier is a (the) Board
    else if ((board = dynamic_cast<Board*> (&whoNotified))) {
        // blank out the areas above and below the grid
        if (board->isCleared()) {
            window.fillRectangle(0, 0, gridWidth*cellSize,
                                       topBar + bottomBar + gridHeight*cellSize,
                                       bgColour);
        } else {
            window.fillRectangle(0, 0, gridWidth*cellSize,
                                       topBar,
                                       bgColour);
            window.fillRectangle(0, topBar + gridHeight*cellSize,
                                 gridWidth*cellSize, bottomBar,
                                 bgColour);
        }

        // draw top and bottom lines
        window.fillRectangle(0, topBar - 4,
                             gridWidth*cellSize, 4,
                             Xwindow::Black);
        window.fillRectangle(0, topBar + gridHeight*cellSize,
                             gridWidth*cellSize, 4,
                             Xwindow::Black);

        // draw text
        window.drawString(225, 15, "Quadris");
        window.drawString(10, 17, "Level:    " + to_string(board->getLevel()));
        window.drawString(10, 30, "Score:    " + to_string(board->getScore()));
        window.drawString(10, 45, "Hi Score: " + to_string(board->getHighScore()));
        window.drawString(10, 20 + topBar + gridHeight*cellSize, "Next:");
        
        // draw next block type
        const vector<vector<bool> > & nextGrid = blocktypes::grid(board->getNextType());
        for (unsigned int j = 0; j < nextGrid.size(); j ++) {
            // nextGrid[0] is safe to access
            for (unsigned int k = 0; k < nextGrid[0].size(); k ++) {
                if (nextGrid[j][k]) {
                    window.fillRectangle(10 + k*cellSize,
                                         25 + topBar + (gridHeight + j)*cellSize,
                                         cellSize,
                                         cellSize,
                                         blocktypes::colour(board->getNextType()));
                }
            }
        }
    }
}
