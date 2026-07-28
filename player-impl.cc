module player;

import <sstream>;
import <memory>;
import <random>;
import dice;
import constants;

using namespace std;

Player::Player(Colour c, unsigned seed): c{c} {
    dice[0] = make_unique<FairDice>(seed);
    dice[1] = make_unique<LoadedDice>();
    die = dice[1];
}

Colour Player::getColour() const { return c; }
int Player::getPoints() const { return points; }
const Inventory &Player::getResources() const { return items; }

void Player::addPoints(int n) { points += n; }
void Player::addResources(const Inventory &i) { items += i; }
void Player::giveResources(const Inventory &i) { items -= i; }
bool Player::canAfford(const Inventory &i) const { return items.covers(i); }
void Player::setDice(int type) { 
    if (type == 0) {
        die = dice[0];
        cout << "Switched die to fair die." << endl;
    } else if (type == 1) {
        die = dice[1];
        cout << "Switched die to loaded die." << endl;
    }
}

int Player::roll() { return die->roll(); }

