module edge;

import <vector>;
import constants;
import <string>;
using namespace std;

Edge::Edge(int id, vector<int> adjVertices) : id{id}, adjVertices{move(adjVertices)} {}

int Edge::getId() const { return id; }

const vector<int> &Edge::getAdjVertices() const { return adjVertices; }

Colour Edge::getOwner() const { return road.owner; }

bool Edge::hasRoad() const { return road.owner != Colour::NONE; }

string Edge::printRoad() const { return road.print(); }

void Edge::placeRoad(Colour c) { road.owner = c; }
