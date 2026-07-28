module vertex;

import <vector>;
import constants;
import <string>;
using namespace std;

Vertex::Vertex(int id, vector<int> adjEdges) : id{id}, adjEdges{move(adjEdges)} {}

int Vertex::getId() const { return id; }
const vector<int> &Vertex::getAdjEdges() const { return adjEdges; }
const Building &Vertex::getBuilding() const { return building; }
Colour Vertex::getOwner() const { return building.owner; }
bool Vertex::hasBuilding() const { return building.owner != Colour::NONE; }
void Vertex::upgradeBuilding() { building.upgrade(); }
string Vertex::printBuilding() const { return building.print(); }
void Vertex::build(Colour c) { 
    building.owner = c;
    building.level = BuildingLevel::BASEMENT;
}

