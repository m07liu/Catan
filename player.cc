export module player;

import dice;
import constants;

using namespace std;

export class Player {
    Colour c;
    bool computer;
    int points;
    Inventory items;
    Dice die;
  public:
    Player(Colour c);

    Colour getColour() const;
    int getPoints() const;
    const Inventory &getResources() const;

    void addPoints(int n);
    void addResources(const Inventory &i);
    void giveResources(const Inventory &i);
    bool canAfford(const Inventory &i) const;
    void setDice(unique_ptr<Dice> d);

    virtual int roll();
    virtual void save();
    virtual void load();
};
