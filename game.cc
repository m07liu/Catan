export module game;

import board;

using namespace std;

class Game {
    Board * theBoard;
  public:
    Game();
    void run();
    void moveGeese();
    void nextPhase();
    void rollAndGive();
    void display();
    virtual save();
    virtual load();
    ~Game();
};
