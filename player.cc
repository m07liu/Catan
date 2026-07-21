export module player;

import dice;
import constants;

using namespace std;

export class Player {
    bool computer;
    int points;
    Inventory items;
    Dice die;
  public:
    virtual int roll();
    virtual void save();
    virtual void load();
};
