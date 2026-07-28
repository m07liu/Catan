export module board;

import <vector>;
import <map>;
import <iostream>;
import <fstream>;
import <string>;
import <utility>;
import <memory>;
import tile;
import edge;
import vertex;
import constants;

using namespace std;


export class Board {
  protected:
    vector<Tile> tiles;        // 19, indexed by tile id  
    vector<Vertex> vertices;
    vector<Edge> edges;
    int geeseTile = 0;
    // Builds the whole graph (vertices, edges, tiles);
    void initBoard();
  public:
    virtual void init(bool load) = 0; // For board setup


    //looking up things
    const Tile &findTile(int id) const;
    const Vertex &findVertex(int id) const;
    const Edge &findEdge(int id) const;
    vector<int> findTiles(int val) const;
    // const Tile &getTile(int id) const;
    // const Vertex &getVertex(int id) const;
    // const Edge &getEdge(int id) const;
    // vector<Tile> findTiles(int val) const; //find list of tiles with value val

    vector<int> tilesAroundVertex(int vid) const;
    
    bool isVertex(int id) const;
    bool isEdge(int id) const;
    bool isTile(int id) const;

    //geese related
    int getGeeseTile() const;
    void moveGeese(int id);

    bool canBuild(int id, Colour c, bool setupPhase = false) const;
    bool canUpgrade(int id, Colour c) const;
    bool canPlaceRoad(int id, Colour c) const;

    void build(int id, Colour c);
    void improve(int id);
    void placeRoad(int id, Colour c);

    // save helpers used by Game
    vector<int> roadsOwnedBy(Colour c) const;
    vector<pair<int, BuildingLevel>> buildingsOwnedBy(Colour c) const;

    void setLayout(istream &in); 
    friend ostream &operator<<(ostream &out, const Board &b);
};


// Concrete Products
export class RandomBoard : public Board {
    unsigned seed;
  public:
    RandomBoard(unsigned seed);
    virtual void init(bool load) override;
};

export class FileBoard : public Board {
    istream *src;
  public:

    FileBoard(istream &src);
    virtual void init(bool load) override;

};

// Concrete Creator
export class BoardFactory {
  public:
    unique_ptr<Board> createBoard(int type, unsigned seed, istream & src);

};
