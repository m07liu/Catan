module tile;

import <vector>;
import constants;

using namespace std;

Tile::Tile(int val, TileType type, vector<int> adjVertices, vector<int> adjEdges)
    : val{val}, type{type}, adjVertices{move(adjVertices)}, adjEdges{move(adjEdges)} {}

int Tile::getVal() const { return val; }
TileType Tile::getType() const { return type; }
const vector<int> &Tile::getAdjVertices() const { return vertices; }
const vector<int> &Tile::getEdges() const { return edges; }
bool Tile::hasGeese() const { return geese; }
void Tile::setGeese() { geese = true; }
