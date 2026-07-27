module tile;

import <vector>;
import constants;

using namespace std;

Tile::Tile(int id, int val, TileType type, vector<int> adjVertices, vector<int> adjEdges)
    : id{id}, val{val}, type{type}, adjVertices{adjVertices}, adjEdges{adjEdges} {}

int Tile::getId() const { return id; }
int Tile::getVal() const { return val; }
TileType Tile::getType() const { return type; }
const vector<int> &Tile::getAdjVertices() const { return adjVertices; }
const vector<int> &Tile::getAdjEdges() const { return adjEdges; }
bool Tile::hasGeese() const { return geese; }
void Tile::setGeese() { geese = ! geese; }
