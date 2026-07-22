export module vertex;

import <vector>;
import constants;
import <string>;
using namespace std;

export class Vertex {
    int id;
    Building building;
    Colour owner = Colour::NONE;
    vector<int> adjEdges;

  public:
    Vertex(int id, vector<int> adjEdges);

    int getId() const;
    const vector<int> &getAdjEdges() const;
    const Building &getBuilding() const;
    Colour getOwner() const;
    bool hasBuilding() const;
    void upgradeBuilding();
    string printBuilding() const;
    void build(Colour c);
    
    // bool isBuildableBy(Colour c);

}

