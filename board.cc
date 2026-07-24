export module board;

import <vector>;
import <map>;
import <iostream>;
import <fstream>;
import <string>;
import tile;
import edge;
import vertex;

using namespace std;

export class Board {
    unsigned seed = 000;
    map<int, vector<Tile>> tiles;
    vector<Vertex> vertices;
    vector<Edge> edges;
    int geeseTile;
  protected:
    // Helpers for board initilization
    void addTile();
    void addVertex();
    void addEdge();
    void initBoard();
  public:
    virtual void init() = 0; // For board setup

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
    
    virtual ~Board();

    friend ostream &operator<<(ostream &out, const Board &b);

};


// Concrete Products
export class RandomBoard : public Board {
  public:
    RandomBoard(int seed);
    virtual init() override;
};

export class FileBoard : public Board {
    ifstream file;
  public:
    FileBoard(ifstream &file);
    virtual init() override;
};

// Concrete Creator
export class BoardFactory {
  public:
    static unique_ptr<Board> createBoard(const string & type);
};