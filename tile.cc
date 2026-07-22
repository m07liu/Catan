export module tile;

import <vector>;
import constants;

using namespace std;

export class Tile {
    int val;
    TileType type;
    bool hasGeese;
    vector<int> adjVertices;
    vector<int> adjEdges; 

  public:
    Tile(int val, TileType type, vector<int> adjVertices, vector<int> adjEdges);
    int getVal() const;
    TileType getType() const;
    const vector<int> &getAdjVertices() const;
    const vector<int> &getEdges() const;
    bool hasGeese() const;
    void setGeese();
    
}


