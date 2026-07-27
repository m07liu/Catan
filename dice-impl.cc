module dice;

import <random>
import <string>;
import <limits>;
import <iostream>;

using namespace std;

Dice::Dice(int type) : type{type} {};
int Dice::getType() const { return type; }

FairDice::FairDice(unsigned seed) : Dice{0}, rng{seed} {}
int FairDice::roll() {
    vector<int> v1 = {1,2,3,4,5,6};
    vector<int> v2 = {1,2,3,4,5,6};

    shuffle(v1.begin(), v1.end(), rng);
    shuffle(v2.begin(), v2.end(), rng);

    return v1[0] + v2[0];
}

LoadedDice::LoadedDice() : Dice{1} {}

int LoadedDice::roll() {
   int input = -1;
    cout << "Input a roll between 2 and 12:" << endl;

    while (!(cin >> input) || input < 2 || input > 12) {
        if (cin.eof()) return -1;          // let the caller handle end-of-file
        cout << "Invalid roll." << endl;
        cin.clear();                        
        //for multi character wrong input we only want one invalid output line
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input a roll between 2 and 12:" << endl;
    }
    return input;
}