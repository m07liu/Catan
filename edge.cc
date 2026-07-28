export module edge;

import <vector>;
import constants;
import <string>;
using namespace std;

export class Edge {
    int id;
    Road road;
    vector<int> adjVertices;

  public:
    Edge(int id, vector<int> adjVertices);
    int getId() const;
    const vector<int> &getAdjVertices() const;
    Colour getOwner() const;
    bool hasRoad() const;
    string printRoad() const;
    void placeRoad(Colour c);

};

