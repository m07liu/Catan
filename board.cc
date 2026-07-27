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

// One entry per tile, in tile-id order: the resource it produces and its value.
export using Layout = vector<pair<TileType, int>>;

export class Board {
    vector<Tile> tiles;        // 19, indexed by tile id  
    vector<Vertex> vertices;
    vector<Edge> edges;
    int geeseTile = 0;

  protected:
    // Builds the whole graph (vertices, edges, tiles) from a finished layout.
    void initBoard(const Layout &layout);
  public:
    virtual void init() = 0; // For board setup

    map<Colour, Inventory> giveResources(int dieVal) const;
    int pointsOf(Colour c) const;


    //looking up things
    const Tile &findTile(int id) const;
    const vector<int> &findTiles(int val) const;
    const Vertex &findVertex(int id) const;
    const Edge &findEdge(int id) const;
    // const Tile &getTile(int id) const;
    // const Vertex &getVertex(int id) const;
    // const Edge &getEdge(int id) const;
    // const vector<Tile> &findTiles(int val) const; //find list of tiles with value val

    bool isVertex(int id) const;
    bool isEdge(int id) const;
    bool isTile(int id) const;

    //geese related
    int getGeeseTile() const;
    void moveGeese(int id);
    // Colours with a residence on this tile, in builder order, excluding active.
    vector<Colour> ownersOnTile(int tileId, Colour active) const;
    
    bool canBuild(int id, Colour c) const;
    bool canUpgrade(int id, Colour c) const;
    bool canPlaceRoad(int id, Colour c) const;

    void build(int id, Colour c);
    void improve(int id);
    void placeRoad(int id, Colour c);

    // save helpers used by Game
    vector<int> roadsOwnedBy(Colour c) const;
    vector<pair<int, BuildingLevel>> buildingsOwnedBy(Colour c) const;
    int pointsOf(Colour c) const;

    friend ostream &operator<<(ostream &out, const Board &b);
};


// Concrete Products
export class RandomBoard : public Board {
    unsigned seed;
  public:
    RandomBoard(unsigned seed);
    virtual init() override;
};

export class FileBoard : public Board {
    istream src;
  public:
    FileBoard(istream &src);
    void setStream(istream &src);
    virtual init() override;
};

// Concrete Creator
export class BoardFactory {
  public:
    unique_ptr<Board> createBoard(int type, unsigned seed, istream & src);

};
