export module player;

import <random>;
import <memory>
import dice;
import constants;

using namespace std;

export class Player {
    Colour c;
    bool computer = false;
    int points = 0;
    Inventory items;
    unique_ptr<Dice> die = make_unique<LoadedDice>();
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

    int roll(default_random_engine &rng);

};
