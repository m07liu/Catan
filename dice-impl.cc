module dice;
import <cstdlib>;
import <iostream>;

using namespace std;

Dice::Dice(int type) : type{type} {};

int FairDice::roll() {
    srand(time(0));
    return (rand() % 6) + 1;
}

int LoadedDice::roll() {
    int input = -1;
    cout << "Input a roll between 2 and 12:" << endl;
    cin >> input;

    while(input < 2 || input > 12) {
        cout << "Invalid roll." << endl;
        cout << "Input a roll between 2 and 12:" << endl;
        cin >> input;
    }

    return input;
}