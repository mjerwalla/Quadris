/* CHECK OUT THIS D4NK MAIN FILE */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <map>
#include <memory>
#include <stdexcept>

#include "board.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"

using namespace std;

void commandLoop(Board &board, TextDisplay &td, istream &in, ostream &out);

// MAIN
int main(int argc, char *argv[]) {

    /////////////////////////////////////////////
    // Default command-line argument settings
    bool graphics = true;
    unsigned long seed = 1;
    string sequence = "sequence.txt";
    int startLevel = 0;

    /////////////////////////////////////////////
    // Non-command-line settings
    int boardWidth = 11;
    int boardHeight = 18; // i.e. 15 + 3 spaces at top
    int spawnX = 0;  // blocks will appear with the bottom-left
    int spawnY = 14; // corner at (0, 14), 3 rows below the top
                    

    /////////////////////////////////////////////
    // Read command-line arguments
    for (int j = 1; j < argc; j++) {
        if (strcmp(argv[j], "-text") == 0) {
            // disable graphics if flag present
            graphics = false;
            continue;
        }
        if (strcmp(argv[j], "-seed") == 0 && j + 1 < argc) {
            // set the random number generator's seed
            // either the current time or a specified number
            if (strcmp(argv[j + 1], "time") == 0) {
                seed = time(NULL);
            } else {
                try {
                    seed = stoul(argv[j + 1]);
                } catch (invalid_argument & e) {
                    cout << "Invalid seed: must be a number or \"time\"." << endl;
                    return 1;
                }
            }
            srand(seed);
            j++;
            continue;
        }
        if (strcmp(argv[j], "-scriptfile") == 0 && j + 1 < argc) {
            // change file to read block sequence from
            sequence = argv[j + 1];
            j++;
            continue;
        }
        if (strcmp(argv[j],"-startlevel") == 0 && j + 1 < argc) {
            // set start level
            try {
                startLevel = stoi(argv[j + 1]);
            } catch (invalid_argument &e) {
                cout << "Invalid level: must be a number." << endl;
                return 1;
            }
        }
    } // End reading command-line arguments


    /////////////////////////////////////////////
    // Create Board and Displays
    Board board(boardWidth, boardHeight, spawnX, spawnY, startLevel, sequence);
    
    TextDisplay td(boardWidth, boardHeight);
    shared_ptr<GraphicsDisplay> gd; // only initialize this if graphics is on
    if (graphics) gd = make_shared<GraphicsDisplay>(boardWidth, boardHeight);

    // Attach td and gd as Observers of board
    board.attachObserver(td);
    board.addBlockObserver(td);
    if (graphics) {
        board.attachObserver(*gd);
        board.addBlockObserver(*gd);
    }
    
    board.restart(); // to generate first block
    
    // Try-catch block used to exit program safely without leaking memory
    while (true) {
        try {
            commandLoop(board, td, cin, cout); // run command loop, read from stdin
        } catch (runtime_error &e) {
            cout << td; // print text display one more time
            cout << "GAME OVER!" << endl;
            while (true) {
                cout << "Play again? Type yes or no." << endl;
                string s = "";
                cin >> s;

                if (s == "yes") {
                    board.restart();
                    break;
                }
                if (s == "no") {
                    cout << "Thanks for playing!" << endl;
                    return 0;
                }
            }
        }
    }
} /* END of MAIN */

// The command loop is in a separate function so that it can call itself,
//    in order to read commands from a file or other stream (for the sequence
//    command)
void commandLoop(Board &board, TextDisplay &td, istream &in, ostream &out) {

    /////////////////////////////////////////////
    // Command interpreter
    enum Command { LEFT, RIGHT, DOWN, CW, CCW, DROP,
                   LUP, LDOWN, NORND, RND, SEQ, RESTART, HINT,
                   I, J, L, O, S, Z, T,
                   RENAME };

    map<string, Command> commandMap = { { "left", LEFT },
                                    { "right", RIGHT },
                                    { "down", DOWN },
                                    { "clockwise", CW },
                                    { "counterclockwise", CCW },
                                    { "drop", DROP },
                                    { "levelup", LUP },
                                    { "leveldown", LDOWN },
                                    { "norandom", NORND },
                                    { "random", RND },
                                    { "sequence", SEQ },
                                    { "restart", RESTART },
                                    { "hint", HINT },
                                    { "I", I }, // should come up with a
                                    { "J", J }, // better way to handle the
                                    { "L", L }, // specific block commands.
                                    { "O", O }, // e.g. any 1-char command
                                    { "S", S }, // tries to match with a block
                                    { "Z", Z },
                                    { "T", T },
                                    { "rename", RENAME } };
    
    // Loop
    int n;
    string s;

    while (true) {
        out << td;
        // Try to read a multiplier integer
        if (!(in >> n)) {
            in.clear();
            n = 1; // If the read fails, set multiplier to 1
        }

        // Try to read command
        if (!(in >> s)) break; // exit loop if read fails

        Command cmd;
        /* For exact matching commands only:
        try {
            cmd = commandMap.at(s);
        } catch (const out_of_range &e) {
            out << "Command not recognized: " << s << endl;
        }
        */
        
        // Search map for first key that is >= to the entered command
        map<string, Command>::const_iterator m = commandMap.lower_bound(s);

        // Check to make sure the entered command is actually a substring of
        // the value returned above. First check against .end() to avoid segfault
        if (!(m == commandMap.end()) && (*m).first.compare(0, s.length(), s) == 0) {
            cmd = (*m).second;
        } else {
            out << "Command not recognized: " << s << endl;
            continue; // skip the rest of the loop
        }

        // Clear the hint (if one is shown). Happens right after the next
        // command is run, as required by the specification.
        board.clearHint();

        switch (cmd) {
            case LEFT :
                board.left(n);
                break;
            case RIGHT :
                board.right(n);
                break;
            case DOWN :
                board.down(n);
                break;
            case CW :
                board.rcw(n);
                break;
            case CCW :
                board.rccw(n);
                break;
            case DROP :
                board.drop(n);
                break;
            case LUP :
                board.levelUp(n);
                break;
            case LDOWN :
                board.levelDown(n);
                break;
            case NORND :
                if (!(in >> s)) {
                    out << "No file specified!"  << endl;
                    return;
                }
                out << "specified file: " << s << endl;
                board.noRandom(s);
                break;
            case RND :
                board.random();
                break;
            case SEQ :
                {
                    if (!(in >> s)) {
                        out << "No file specified!" << endl;
                        return;
                    }
                    // run a new command loop with input from the file s
                    ifstream fs(s);
                    if (fs.is_open()) {
                        commandLoop(board, td, fs, out);
                    } else {
                        out << "Cannot read file." << endl;
                    }
                }
                break;
            case RESTART :
                board.restart();
                break;
            case HINT :
                board.showHint();
                break;
            case I :
                board.requestBlock('I');
                break;
            case J :
                board.requestBlock('J');
                break;
            case L :
                board.requestBlock('L');
                break;
            case O :
                board.requestBlock('O');
                break;
            case S :
                board.requestBlock('S');
                break;
            case Z :
                board.requestBlock('Z');
                break;
            case T :
                board.requestBlock('T');
                break;
            case RENAME :
                {
                    string s2;
                    if (!((in >> s) && (in >> s2))) {
                        out << "Invalid rename command: rename old new" << endl;
                    }
                    commandMap[s2] = commandMap[s];
                    commandMap.erase(s);

                    cout << "Renamed command " << s << " to " << s2 << "." << endl;
                }
                break;
            default :
                break;
        };
    }
}
