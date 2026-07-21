export module game;

import board;
import constants;

using namespace std;

export class Game {
    Board * theBoard;
  public:
    Game();
    void run();
    void moveGeese();
    void nextPhase();
    void rollAndGive();
    void display();
    void build(BuildingType::type, int id);
    virtual save();
    virtual load();
    ~Game();
};
