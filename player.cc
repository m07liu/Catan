export module player;

import dice;

using namespace std;

class Player {
    Dice die;
  public:
    virtual int roll();
    virtual void save();
    virtual void load();
};
