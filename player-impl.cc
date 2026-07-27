module player;

import <sstream>;
import <memory>;
import <random>;
import dice;
import constants;

using namespace std;

Player::Player(Colour c): c{c} {}

Colour Player::getColour() const { return c; }
int Player::getPoints() const { return points; }
const Inventory &Player::getResources() const { return items; }

void Player::addPoints(int n) { points += n; }
void Player::addResources(const Inventory &i) { items += i; }
void Player::addBuildings(int vertexId) { buildingsOwned.push_back(vertexId); }
void Player::addRoads(int edgeId) { roadsOwned.push_back(edgeId); }
void Player::giveResources(const Inventory &i) { items -= i; }
bool Player::canAfford(const Inventory &i) const { return items.covers(i); }
void Player::setDice(unique_ptr<Dice> d) { die = move(d); }

int Player::roll(default_random_engine &rng) { return die->roll(rng); }

