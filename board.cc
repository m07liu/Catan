export module board;

import <vector>;
import <map>;
import tile;

using namespace std;

export class Board {
    int boardType; // 1 for random, 2 for file
    map<int, vector<Tile>> tiles;
    vector<Vertex> vertices;
    vector<Edge> edges;
  public:
    Board(int boardType);
    void display();
    void save();
    void giveResources(int die);
}