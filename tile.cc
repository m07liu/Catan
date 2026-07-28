export module tile;

import <vector>;
import constants;

using namespace std;

export class Tile {
    int id;
    int val;
    TileType type;
    bool geese = false;
    vector<int> adjVertices;
    vector<int> adjEdges; 

  public:
    Tile(int id, int val, TileType type, vector<int> adjVertices, vector<int> adjEdges);
    int getId() const;
    int getVal() const;
    TileType getType() const;
    const vector<int> &getAdjVertices() const;
    const vector<int> &getAdjEdges() const;
    void changeTile(int val, TileType type);
    bool hasGeese() const;
    void setGeese(bool b);
    
};


