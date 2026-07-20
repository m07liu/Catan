export module dice;

using namespace std;

class Dice {
    int type;
  public:
    Dice(int type); // 1 for fair, 2 for loaded
    virtual int roll() = 0;
};

class FairDice : public Dice {
  public:
    virtual int roll();
};

class LoadedDice : public Dice {
  public:
    virtual int roll();
};
