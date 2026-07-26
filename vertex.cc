export module vertex;

import <vector>;
import constants;
import <string>;
using namespace std;

export class Vertex {
    int id;
    Building building;
    vector<int> adjEdges;
    vector<int> adjTiles;

  public:
    Vertex(int id, vector<int> adjEdges, vector<int> adjTiles);

    int getId() const;
    const vector<int> &getAdjTiles() const;
    const vector<int> &getAdjEdges() const;
    const Building &getBuilding() const;
    Colour getOwner() const;
    bool hasBuilding() const;
    void upgradeBuilding();
    string printBuilding() const;
    void build(Colour c);
    
    // bool isBuildableBy(Colour c);

}

