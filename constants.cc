export module constants;

import <map>;

using namespace std;

export enum class Color {BLUE, RED, ORANGE, YELLOW};
export enum class TileType {BRICK, ENERGY, GLASS, HEAT, WIFI, PARK};
export enum class ResourceType {BRICK, ENERGY, GLASS, HEAT, WIFI, NONE};
export enum class BuildingLevel {NONE, ROAD, BASEMENT, HOUSE, TOWER};

export struct Inventory {
    int Bricks;
    int Energy;
    int Glass;
    int Heat;
    int Wifi;
};

export const map<BuildingLevel, Inventory> BuildingCosts {
    {BuildingLevel::ROAD, Inventory{0, 0, 0, 1, 1}},
    {BuildingLevel::BASEMENT, Inventory{1, 1, 1, 1, 0}},
    {BuildingLevel::HOUSE, Inventory{0, 0, 2, 3, 0}},
    {BuildingLevel::TOWER, Inventory{3, 2, 2, 2, 1}}
};

