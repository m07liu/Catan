export module edge

import <vector>;
import constants;
import <string>;
using namespace std;

export class Edge {
    int id;
    Colour owner = Colour::NONE;
    Road road;
    vector<int> adjVertices;
    vector<int> adjEdges;

  public:
    Edge(int id, vector<int> adjVertices, vector<int> adjEdges);
    int getId() const;
    const vector<int> &getAdjVertices const;
    const vector<int> &getAdjEdges() const;
    Colour getOwner() const;
    bool hasRoad() const;
    string printRoad() const;
    void placeRoad(Colour c);

}

