module vertex

import <vector>;
import constants;
import <string>;
using namespace std;

Vertex::Vertex(int id, vector<int> adjEdges, vector<int> adjTiles) : id{id}, adjEdges{move(adjEdges)}, adjTiles{move(adjTiles)} {}

int Vertex::getId() const { return id; }
const vector<int> &Vertex::getAdjTiles() const { return adjTiles; }
const vector<int> &Vertex::getAdjEdges() const { return adjEdges; }
const Building &Vertex::getBuilding() const { return building; }
Colour Vertex::getOwner() const { return building.owner; }
bool Vertex::hasBuilding() const { return building.owner != Colour::NONE; }
void Vertex::upgradeBuilding() { 
    if (building.level != BuildingLevel::TOWER) {
        building.level = static_cast<BuildingLevel>(1+building.level);
    }
}
string Vertex::printBuilding() const { return building.print(); }
void Vertex::build(Colour c) { 
    building.owner = c;
    building.level = BuildingLevel::BASEMENT;
}

// bool Vertex::isBuildableBy(Colour c);

