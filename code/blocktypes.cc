#include "blocktypes.h"
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using namespace blocktypes;
using std::map;
using std::pair;
using std::vector;
using std::out_of_range;

// See blocktypes.h for details

static const map<char, pair<blocktypes::Colour, vector<vector<bool> > > > theMap = {
    { 'I', { Red,    { { 1, 1, 1, 1 } } } },

    { 'J', { Blue,   { { 1, 0, 0 },
                         { 1, 1, 1 } } } },
    
    { 'L', { Green,  { { 0, 0, 1 },
                         { 1, 1, 1 } } } },
    
    { 'O', { Orange, { { 1, 1 },
                         { 1, 1 } } } },
    
    { 'S', { Yellow,  { { 0, 1, 1 },
                         { 1, 1, 0 } } } },
    
    { 'Z', { Magenta,   { { 1, 1, 0 },
                         { 0, 1, 1 } } } },
    
    { 'T', { White, { { 1, 1, 1 },
                         { 0, 1, 0 } } } },
    { '*', { Brown, { { 1 } } } }
};

Colour blocktypes::colour(char type) {
    try {
        return theMap.at(type).first;
    } catch (out_of_range &e) {
        return Black;
    }
}

vector<vector<bool> > blocktypes::grid(char type) {
    try {
        return theMap.at(type).second;
    } catch (out_of_range &e) {
        return {};
    }
}
