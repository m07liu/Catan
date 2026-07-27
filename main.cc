import <iostream>;
import <fstream>;
import <string>;
import <chrono>;
import game;


using namespace std;

int main(int argc, char* argv[]) {
    unsigned seed = 123;
    string boardFile = "layout.txt";
    string loadFile = "";
    int res = 0; // 0: random, default
    bool load = false;
    bool board = false;
    bool randomBoard = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-seed" && i+1 < argc) {
            seed = static_cast<unsigned>(stoul(argv[++i]));

        } else if (arg == "-load" && i+1 < argc) {
            loadFile = argv[++i];
            load = true;

        } else if (arg == "-board" && i+1 < argc) {
            boardFile = argv[++i];
            board = true;

        } else if (arg == "-random-board") {
            randomBoard = true;

        } 
        
    }

    if (!load && randomBoard) res = 0;
    if (board) res = 1;
    bool firstRound = true;

    while (true) {
        Game game{res, seed, boardFile};
        if (load && firstRound) {
            ifstream in{loadFile};
            game.load(in);
        }

        firstRound = false;
        if (!game.run()) break;
    }
    return 0;

}


