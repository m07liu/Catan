module constants;

import <string>;
using namespace std;

Inventory &Inventory::operator+=(const Inventory &other) {
    bricks += other.bricks;
    energy += other.energy;
    glass += other.glass;
    heat += other.heat;
    wifi += other.wifi;
    return *this;
}


Inventory &Inventory::operator-=(const Inventory &other) {
    bricks -= other.bricks;
    energy -= other.energy;
    glass -= other.glass;
    heat -= other.heat;
    wifi -= other.wifi;
    return *this;
}

int &Inventory::operator[](int i) {
    if (i == 0) return bricks;
    else if (i == 1) return energy;
    else if (i == 2) return glass;
    else if (i == 3) return heat;
    return wifi;
}

int Inventory::operator[](int i) const {
    if (i == 0) return bricks;
    else if (i == 1) return energy;
    else if (i == 2) return glass;
    else if (i == 3) return heat;
    return wifi;
}

bool Inventory::isEmpty() const {
    return total() == 0;
}

bool Inventory::covers(const Inventory &cost) const {
    return bricks >= cost.bricks && 
            energy >= cost.energy && 
            glass >= cost.glass &&
            heat >= cost.heat && 
            wifi >= cost.wifi;
}

int Inventory::total() const {
    return bricks + energy + glass + heat + wifi;
}


string colourToString(Colour c) {
    if (c == Colour::BLUE) return "Blue";
    else if (c == Colour::RED) return "Red";
    else if (c == Colour::ORANGE) return "Orange";
    else if (c == Colour::YELLOW) return "Yellow";
    return "";
}

char colourToChar(Colour c) {
    if (c == Colour::BLUE) return "B";
    else if (c == Colour::RED) return "R";
    else if (c == Colour::ORANGE) return "O";
    else if (c == Colour::YELLOW) return "Y";
    return ' ';
}

string tileTypeToString(TileType t) {
    if (t == TileType::BRICK) return "BRICK";
    else if (t == TileType::ENERGY) return "ENERGY";
    else if (t == TileType::GLASS) return "GLASS";
    else if (t == TileType::HEAT) return "HEAT";
    else if (t == TileType::WIFI) return "WIFI";
    else if (t == TileType::PARK) return "PARK";
    return "";
}

string buildingLevelToString(BuildingLevel level) {
    if (level == BuildingLevel::BASEMENT) return "Basement";
    else if (level == BuildingLevel::HOUSE) return "House";
    else if (level == BuildingLevel::TOWER) return "Tower";
    return "";
}

char buildingLevelToChar(BuildingLevel level) {
    if (level == BuildingLevel::BASEMENT) return 'B';
    else if (level == BuildingLevel::HOUSE) return 'H';
    else if (level == BuildingLevel::TOWER) return 'T';
    return ' ';
}

string invnumToResource(int i) const {
    if (i == 0) return "BRICK";
    else if (i == 1) return "ENERGY";
    else if (i == 2) return "GLASS";
    else if (i == 3) return "HEAT";
    else if (i == 4) return "WIFI";
    return "";
}

Inventory singletonInv(int i) {
    Inventory inv;
    v[i] = 1;
    return inv;
}



int Building::yield() {
    if (level == BuildingLevel::BASEMENT) return 1;
    else if (level == BuildingLevel::HOUSE) return 2;
    else if (level == BuildingLevel::TOWER) return 3;
    return 0;
}

void Building::upgrade() {
    if (level == BuildingLevel::BASEMENT) level = BuildingLevel::HOUSE;
    else if (level == BuildingLevel::HOUSE) level = BuildingLevel::TOWER;
    else throw runtime_error("Building cannot be upgraded further.");
}

string Building::print() const {
    return string{colourToChar(owner)} + string{buildingLevelToChar(level)};
}

string Road::print() const {
    return string{colourToChar(owner)} + "R";
}

