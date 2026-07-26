export module constants;

import <map>;
import <string>;

using namespace std;


export const int NUM_TILES = 19;
export const int NUM_VERTICES = 54;
export const int NUM_EDGES = 72;

export enum class Colour {BLUE, RED, ORANGE, YELLOW, NONE};
export enum class TileType {BRICK, ENERGY, GLASS, HEAT, WIFI, PARK};
export enum class ResourceType {BRICK, ENERGY, GLASS, HEAT, WIFI, NONE};
export enum class BuildingLevel {BASEMENT, HOUSE, TOWER, NONE};

export struct Inventory {
    int bricks = 0;
    int energy = 0;
    int glass = 0;
    int heat = 0;
    int wifi = 0;
        
    Inventory &operator+=(const Inventory &other);
    Inventory &operator-=(const Inventory &other);
    int &operator[](int i);
    int operator[](int i) const;
    bool isEmpty() const;
    bool covers(const Inventory &cost) const;
    int total() const;

};

export struct Building {
    Colour owner = Colour::NONE;
    BuildingLevel level = BuildingLevel::NONE;

    void upgrade();
    int yield() const;
    string print() const;
};

export struct Road {
    Colour owner = Colour::NONE;
    string print() const;
};

export const Inventory RoadCost{0, 0, 0, 1, 1};
export const map<BuildingLevel, Inventory> BuildingCosts {
    {BuildingLevel::BASEMENT, Inventory{1, 1, 1, 1, 0}},
    {BuildingLevel::HOUSE, Inventory{0, 0, 2, 3, 0}},
    {BuildingLevel::TOWER, Inventory{3, 2, 2, 2, 1}}
};

export string colourToString(Colour c);
export string tileTypeToString(TileType t);
export string buildingLevelToString(BuildingLevel level);
export char buildingLevelToChar(BuildingLevel level);
export char colourToChar(Colour c);
export string invnumToResource(int i) const;
export Inventory singletonInv(int i);

export optional<Colour> parseColour(const string &s);
export optional<int> parseResource(const string &s);


