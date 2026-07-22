module edge;

import <vector>;
import constants;
import <string>;
using namespace std;

Edge::Edge(int id, vector<int> adjVertices, vector<int> adjEdges)
    : id{id}, adjVertices{move(adjVertices)}, adjEdges{move(adjEdges)} {}

int Edge::getId() const { return id; }
const vector<int> &Edge::getAdjVertices const { return adjVertices; }
const vector<int> &Edge::getAdjEdges() const { return adjEdges; }
Colour Edge::getOwner() const { return owner; }
bool Edge::hasRoad() const { return owner != Colour::NONE; }
string Edge::printRoad() const { return road->print(); }
void Edge::placeRoad(Colour c) { road = Road{c}; }
