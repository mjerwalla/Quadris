#ifndef _TEXTDISPLAY_H_
#define _TEXTDISPLAY_H_

#include <vector>
#include <iostream>

#include "observer.h"

using std::vector;
using std::ostream;


class TextDisplay : public Observer {
    int gridWidth, gridHeight;
    vector<vector<char> > charGrid; // Grid to track blocks' display positions
    
    int level;
    int score;
    int highScore;
    char nextType; // next block type
  public:
    TextDisplay(int w, int h);

    void notify(Subject & whoNotified) override;
    
    friend ostream & operator<<(ostream & out, const TextDisplay & td); // output operator
};

#endif /* _TEXTDISPLAY_H_ */
