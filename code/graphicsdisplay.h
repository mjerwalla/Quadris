#ifndef _GRAPHICSDISPLAY_H_
#define _GRAPHICSDISPLAY_H_

#include "window.h"
#include "observer.h"
#include "blocktypes.h"

class GraphicsDisplay : public Observer {
    int gridWidth, gridHeight; // unit number of cells
    Xwindow window;
    
    int cellSize;
    int gridX, gridY; // x and y of the bottom-left corner of the grid,
                      // in relation to the top-left corner of the window

    int topBar, bottomBar; // height of the spaces above and below the grid

    blocktypes::Colour bgColour;

  public:
    GraphicsDisplay(int w,
                    int h,
                    int cellSize=25,
                    int topBar=55,
                    int bottomBar=85,
                    blocktypes::Colour bgColour=blocktypes::Cyan);

    void notify(Subject &whoNotified) override;
};


#endif /* _GRAPHICSDISPLAY_H_ */
