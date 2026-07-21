export module dice;

using namespace std;

export class Dice {
    int type;
  public:
    Dice(int type); // 0 for fair, 1 for loaded
    virtual int roll() = 0;
};

export class FairDice : public Dice {
  public:
    virtual int roll();
};

export class LoadedDice : public Dice {
  public:
    virtual int roll();
};
