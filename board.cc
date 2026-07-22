export module board;

import <vector>;
import <map>;
import <iostream>;
import <string>;
import tile;
import edge;
import vertex;

using namespace std;

export class Board {
    int boardType; // 1 for random, 2 for file
    unsigned int seed = 000;
    map<int, vector<Tile>> tiles;
    vector<Vertex> vertices;
    vector<Edge> edges;
    int geeseTile;
  public:
    Board(int boardType);
  
    void display();
    void save();
    void giveResources(int dieVal);

    const Tile &findTiles(int id) const;
    const vector<Tile> &findVertex(int val) const;
    const Edge &findEdge(int id) const;
    int getGeeseTile() const;
    void moveGeese(int id);
    
    bool canBuild(int id, Colour c) const;
    bool canPlaceRoad(int id, Colour c) const;
    
    friend ostream &operator<<(ostream &out, const Board &b);
  
}

