export module dice;

import <random>;

using namespace std;

export class Dice {
    int type;
  public:
    Dice(int type); // 0 for fair, 1 for loaded
    int getType() const;   // lets Game/Player report or save which dice are set
    virtual int roll(default_random_engine &rng) = 0;
    virtual ~Dice() = default;
};

export class FairDice : public Dice {

  public:
    FairDice();
    int roll(default_random_engine &rng) override;
};

export class LoadedDice : public Dice {
  public:
    LoadedDice();
    int roll(default_random_engine &rng) override;
};
