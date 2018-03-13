#ifndef _BLOCKTYPES_H_
#define _BLOCKTYPES_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

// blocktypes.h provides functions to access the default colours and grid layouts
//    for each block type

// Actual block type definitions in blocktypes.cc

namespace blocktypes {
    // Possible colours (same as in Xwindow)
    enum Colour { White=0, Black, Red, Green, Blue, Cyan, Yellow, Magenta, Orange, Brown };
    
    // Colour colour(type) returns a Colour from above describing the colour of
    //    the specified block type. If no such block type is defined, it
    //    returns Black.
    Colour colour(char type);

    // vector<vector<bool> > grid(type) returns a 2D boolean vector showing
    //    the initial grid configuration for the specified block type. If no
    //    such block type is defined, it returns an empty vector.
    vector<vector<bool> > grid(char type);
}

#endif /* _BLOCKTYPES_H_ */
