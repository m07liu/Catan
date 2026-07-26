module board;

import <random>;
import <algorithm>;
import <chrono>;
import <vector>;
import <map>;
import <iostream>;
import <fstream>;
import <string>;
import constants;
import tile;
import edge;
import vertex;

using namespace std;

//  Geometry things that only the display needs
enum { TL, TR, ML, MR, BL, BR };
enum { E_TOP, E_UL, E_UR, E_LL, E_LR, E_BOT };
 
const int NUM_TILES = 19, NUM_VERTICES = 54, NUM_EDGES = 72;
 
const vector<vector<int>> TILE_VERTICES = {
    { 0,  1,  3,  4,  8,  9},
    { 2,  3,  7,  8, 13, 14}, { 4,  5,  9, 10, 15, 16},
    { 6,  7, 12, 13, 18, 19}, { 8,  9, 14, 15, 20, 21}, {10, 11, 16, 17, 22, 23},
    {13, 14, 19, 20, 25, 26}, {15, 16, 21, 22, 27, 28},
    {18, 19, 24, 25, 30, 31}, {20, 21, 26, 27, 32, 33}, {22, 23, 28, 29, 34, 35},
    {25, 26, 31, 32, 37, 38}, {27, 28, 33, 34, 39, 40},
    {30, 31, 36, 37, 42, 43}, {32, 33, 38, 39, 44, 45}, {34, 35, 40, 41, 46, 47},
    {37, 38, 43, 44, 48, 49}, {39, 40, 45, 46, 50, 51},
    {44, 45, 49, 50, 52, 53}
};
 
const vector<vector<int>> TILE_EDGES = {
    { 0,  1,  2,  6,  7, 10},
    { 3,  5,  6, 13, 14, 18}, { 4,  7,  8, 15, 16, 19},
    { 9, 12, 13, 20, 21, 26}, {10, 14, 15, 22, 23, 27}, {11, 16, 17, 24, 25, 28},
    {18, 21, 22, 30, 31, 35}, {19, 23, 24, 32, 33, 36},
    {26, 29, 30, 37, 38, 43}, {27, 31, 32, 39, 40, 44}, {28, 33, 34, 41, 42, 45},
    {35, 38, 39, 47, 48, 52}, {36, 40, 41, 49, 50, 53},
    {43, 46, 47, 54, 55, 60}, {44, 48, 49, 56, 57, 61}, {45, 50, 51, 58, 59, 62},
    {52, 55, 56, 63, 64, 67}, {53, 57, 58, 65, 66, 68},
    {61, 64, 65, 69, 70, 71}
};
 
const int TILE_COL[NUM_TILES]  = {20, 10, 30, 0, 20, 40, 10, 30, 0, 20, 40,
                                  10, 30, 0, 20, 40, 10, 30, 20};
const int TILE_LINE[NUM_TILES] = { 0,  4,  4, 8,  8,  8, 12, 12, 16, 16, 16,
                                  20, 20, 24, 24, 24, 28, 28, 32};

void Board::initBoard() {
    int numofvertices = 54;
    int numofedges = 72;
    // Create Vertices
    for (int i = 0; i < numofvertices; i++) {
        if (i == 0) vertices.emplace_back(Vertex{i, vector<int>{0, 1}}); 
        else if (i == 1) vertices.emplace_back(Vertex{i, vector<int>{0, 2}}); 
        else if (i == 2) vertices.emplace_back(Vertex{i, vector<int>{3, 5}}); 
        else if (i == 3) vertices.emplace_back(Vertex{i, vector<int>{1, 3, 6}}); 
        else if (i == 4) vertices.emplace_back(Vertex{i, vector<int>{2, 4, 7}}); 
        else if (i == 5) vertices.emplace_back(Vertex{i, vector<int>{4, 8}}); 
        else if (i == 6) vertices.emplace_back(Vertex{i, vector<int>{9, 12}}); 
        else if (i == 7) vertices.emplace_back(Vertex{i, vector<int>{5, 9, 13}}); 
        else if (i == 8) vertices.emplace_back(Vertex{i, vector<int>{6, 10, 14}}); 
        else if (i == 9) vertices.emplace_back(Vertex{i, vector<int>{7, 10, 15}}); 
        else if (i == 10) vertices.emplace_back(Vertex{i, vector<int>{8, 11, 16}}); 
        else if (i == 11) vertices.emplace_back(Vertex{i, vector<int>{11, 17}}); 
        else if (i == 12) vertices.emplace_back(Vertex{i, vector<int>{12, 20}}); 
        else if (i == 13) vertices.emplace_back(Vertex{i, vector<int>{13, 18, 21}}); 
        else if (i == 14) vertices.emplace_back(Vertex{i, vector<int>{14, 18, 22}}); 
        else if (i == 15) vertices.emplace_back(Vertex{i, vector<int>{15, 19, 23}}); 
        else if (i == 16) vertices.emplace_back(Vertex{i, vector<int>{16, 19, 24}}); 
        else if (i == 17) vertices.emplace_back(Vertex{i, vector<int>{17, 25}}); 
        else if (i == 18) vertices.emplace_back(Vertex{i, vector<int>{20, 26, 29}}); 
        else if (i == 19) vertices.emplace_back(Vertex{i, vector<int>{21, 26, 30}}); 
        else if (i == 20) vertices.emplace_back(Vertex{i, vector<int>{22, 27, 31}}); 
        else if (i == 21) vertices.emplace_back(Vertex{i, vector<int>{23, 27, 32}}); 
        else if (i == 22) vertices.emplace_back(Vertex{i, vector<int>{24, 28, 33}}); 
        else if (i == 23) vertices.emplace_back(Vertex{i, vector<int>{25, 28, 34}}); 
        else if (i == 24) vertices.emplace_back(Vertex{i, vector<int>{29, 37}}); 
        else if (i == 25) vertices.emplace_back(Vertex{i, vector<int>{30, 35, 38}}); 
        else if (i == 26) vertices.emplace_back(Vertex{i, vector<int>{31, 35, 39}}); 
        else if (i == 27) vertices.emplace_back(Vertex{i, vector<int>{32, 36, 40}}); 
        else if (i == 28) vertices.emplace_back(Vertex{i, vector<int>{33, 36, 41}}); 
        else if (i == 29) vertices.emplace_back(Vertex{i, vector<int>{34, 42}}); 
        else if (i == 30) vertices.emplace_back(Vertex{i, vector<int>{37, 43, 46}}); 
        else if (i == 31) vertices.emplace_back(Vertex{i, vector<int>{38, 43, 47}}); 
        else if (i == 32) vertices.emplace_back(Vertex{i, vector<int>{39, 44, 48}}); 
        else if (i == 33) vertices.emplace_back(Vertex{i, vector<int>{40, 44, 49}}); 
        else if (i == 34) vertices.emplace_back(Vertex{i, vector<int>{41, 45, 50}}); 
        else if (i == 35) vertices.emplace_back(Vertex{i, vector<int>{42, 45, 51}}); 
        else if (i == 36) vertices.emplace_back(Vertex{i, vector<int>{46, 54}}); 
        else if (i == 37) vertices.emplace_back(Vertex{i, vector<int>{47, 52, 55}}); 
        else if (i == 38) vertices.emplace_back(Vertex{i, vector<int>{48, 52, 56}}); 
        else if (i == 39) vertices.emplace_back(Vertex{i, vector<int>{49, 53, 57}}); 
        else if (i == 40) vertices.emplace_back(Vertex{i, vector<int>{50, 53, 58}}); 
        else if (i == 41) vertices.emplace_back(Vertex{i, vector<int>{51, 59}}); 
        else if (i == 42) vertices.emplace_back(Vertex{i, vector<int>{54, 60}}); 
        else if (i == 43) vertices.emplace_back(Vertex{i, vector<int>{55, 60, 63}}); 
        else if (i == 44) vertices.emplace_back(Vertex{i, vector<int>{56, 61, 64}}); 
        else if (i == 45) vertices.emplace_back(Vertex{i, vector<int>{57, 61, 65}}); 
        else if (i == 46) vertices.emplace_back(Vertex{i, vector<int>{58, 62, 66}}); 
        else if (i == 47) vertices.emplace_back(Vertex{i, vector<int>{59, 62}}); 
        else if (i == 48) vertices.emplace_back(Vertex{i, vector<int>{63, 67}}); 
        else if (i == 49) vertices.emplace_back(Vertex{i, vector<int>{64, 67, 69}}); 
        else if (i == 50) vertices.emplace_back(Vertex{i, vector<int>{65, 68, 70}}); 
        else if (i == 51) vertices.emplace_back(Vertex{i, vector<int>{66, 68}}); 
        else if (i == 52) vertices.emplace_back(Vertex{i, vector<int>{69, 71}}); 
        else vertices.emplace_back(Vertex{i, vector<int>{70, 71}}); 
    }
    // Create Edges
    int row = 0;
    for (int i = 0; i < numofedges; i++) {
        if (i == 0) edges.emplace_back(Edge{i, vector<int>{0, 1}});
        else if (i == 1 || i == 2) edges.emplace_back(Edge{i, vector<int>{i-1, i+2}});
        else if (i == 3) edges.emplace_back(Edge{i, vector<int>{2, 3}});
        else if (i == 4) edges.emplace_back(Edge{i, vector<int>{4, 5}});
        else if (5 <= i && i <= 8) edges.emplace_back(Edge{i, vector<int>{i-3, i+2}});
        else if (i == 9) edges.emplace_back(Edge{i, vector<int>{6, 7}});
        else if (i == 10) edges.emplace_back(Edge{i, vector<int>{8, 9}});
        else if (i == 11) edges.emplace_back(Edge{i, vector<int>{10, 11}});
        else if (12 <= i && i <= 17) edges.emplace_back(Edge{i, vector<int>{i-6, i}});
        else if (i == 18) edges.emplace_back(Edge{i, vector<int>{13, 14}});
        else if (i == 19) edges.emplace_back(Edge{i, vector<int>{15, 16}});
        else if (20 <= i && i <= 25) edges.emplace_back(Edge{i, vector<int>{i-8, i-2}});
        else if (i == 26) edges.emplace_back(Edge{i, vector<int>{18, 19}});
        else if (i == 27) edges.emplace_back(Edge{i, vector<int>{20, 21}});
        else if (i == 28) edges.emplace_back(Edge{i, vector<int>{22, 23}});
        else if (29 <= i && i <= 34) edges.emplace_back(Edge{i, vector<int>{i-11, i-5}});
        else if (i == 35) edges.emplace_back(Edge{i, vector<int>{25, 26}});
        else if (i == 36) edges.emplace_back(Edge{i, vector<int>{27, 28}});
        else if (37 <= i && i <= 42) edges.emplace_back(Edge{i, vector<int>{i-13, i-7}});
        else if (i == 43) edges.emplace_back(Edge{i, vector<int>{30, 31}});
        else if (i == 44) edges.emplace_back(Edge{i, vector<int>{32, 33}});
        else if (i == 45) edges.emplace_back(Edge{i, vector<int>{34, 35}});
        else if (46 <= i && i <= 51) edges.emplace_back(Edge{i, vector<int>{i-16, i-10}});
        else if (i == 52) edges.emplace_back(Edge{i, vector<int>{37, 38}});
        else if (i == 53) edges.emplace_back(Edge{i, vector<int>{39, 40}});
        else if (54 <= i && i <= 59) edges.emplace_back(Edge{i, vector<int>{i-18, i-12}});
        else if (i == 60) edges.emplace_back(Edge{i, vector<int>{42, 43}});
        else if (i == 61) edges.emplace_back(Edge{i, vector<int>{44, 45}});
        else if (i == 62) edges.emplace_back(Edge{i, vector<int>{46, 47}});
        else if (63 <= i && i <= 66) edges.emplace_back(Edge{i, vector<int>{i-20, i-15}});
        else if (i == 67) edges.emplace_back(Edge{i, vector<int>{48, 49}});
        else if (i == 68) edges.emplace_back(Edge{i, vector<int>{50, 51}});
        else if (i == 69 || i == 70) edges.emplace_back(Edge{i, vector<int>{i-20, i-17}});
        else edges.emplace_back(Edge{i, vector<int>{52, 53}});
    }
}

void tileNeighbors(int id);

void Board::addTile();

void Board::addVertex();

void Board::addEdge();

void Board::display();

void Board::save();

void Board::giveResources(int dieVal);

//lookups
bool Board::isVertex(int id) const { return id >= 0 && id < NUM_VERTICES; }
bool Board::isEdge(int id) const   { return id >= 0 && id < NUM_EDGES; }
bool Board::isTile(int id) const   { return id >= 0 && id < NUM_TILES; }
 
const Tile &Board::findTile(int id) const     { return tiles[id]; }
const Vertex &Board::findVertex(int id) const { return vertices[id]; }
const Edge &Board::findEdge(int id) const     { return edges[id]; }
 
vector<int> Board::findTiles(int val) const {
    vector<int> found;
    for (const Tile &t : tiles) {
        if (t.getVal() == val && t.getType() != TileType::PARK) {
            found.emplace_back(t.getId());
        }
    }
    return found;
}

int Board::getGeeseTile() const { return geeseTile; }; 

void Board::moveGeese(int id) {
    if (!isTile(id) || id == geeseTile) return;
    tiles[geeseTile].setGeese(false);
    geeseTile = id;
    tiles[geeseTile].setGeese(true);
}

vector<Colour> Board::ownersOnTile(int tileId, Colour active) const {
    vector<Colour> owners;
    if (!isTile(tileId)) return owners;
    for (int vid : tiles[tileId].getAdjVertices()) {
        const Vertex &v = vertices[vid];
        if (!v.hasBuilding()) continue;
        Colour owner = v.getOwner();
        if (owner == active) continue;
        if (find(owners.begin(), owners.end(), owner) == owners.end()) {
            owners.emplace_back(owner);
        }
    }
    sort(owners.begin(), owners.end(), [](Colour a, Colour b) {
        return static_cast<int>(a) < static_cast<int>(b);
    });
    return owners;
}

bool Board::canBuild(int id, Colour c) const;

bool Board::canUpgrade(int id, Colour c) const {
    if (!isVertex(id)) return false;
    const Vertex &v = vertices[id];
    return v.hasBuilding()
        && v.getOwner() == c
        && v.getBuilding().level != BuildingLevel::TOWER;
}

bool Board::canPlaceRoad(int id, Colour c) const;

void Board::build(int id, Colour c)     { vertices[id].build(c); }
void Board::improve(int id)             { vertices[id].upgradeBuilding(); }
void Board::placeRoad(int id, Colour c) { edges[id].placeRoad(c); }

Board::~Board();

//------------------------------Concrete Products-------------------------------

RandomBoard::RandomBoard(unsigned seed) : seed{seed} {}

void RandomBoard::init() {
    initBoard();
    for (int i = 2; i <= 12; i++) {
        tiles.insert({i, vector<Tile>{}});
    }
    vector<int> dicenums =
        {2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    vector<TileType> tiles = {
        TileType::WIFI, TileType::WIFI, TileType::WIFI,
        TileType::HEAT, TileType::HEAT, TileType::HEAT,
        TileType::BRICK, TileType::Brick, TileType::Brick, TileType::BRICK, 
        TileType::ENERGY, TileType::ENERGY,TileType::ENERGY, TileType::ENERGY,  
        TileType::GLASS, TileType::GLASS, TileType::GLASS, TileType::GLASS,
    };

    default_random_engine rng{seed};
    shuffle(dicenums.begin(), dicenums.end(), rng);
    shuffle(tiles.begin(), tiles.end(), rng);

    int tilesindex = 0; // Used to skip when the tile is of type PARK
    for (int i = 0; i < 19; i++) {
        if (dicenums[i] == 7) { // Park Tile
            tiles.emplace_back(Tile{i, dicenums[i], TileType::PARK, TILE_VERTICES[i], TILE_EDGES[i]});
            tiles[i].setGeese(); // Change Geese to True
            geeseTile = i;
        } else {
            tiles.emplace_back(Tile{i, dicenums[i], tiles[tilesindex], TILE_VERTICES[i], TILE_EDGES[i]});
            tilesindex++;
        }
    }
}

FileBoard::FileBoard(ifstream &file) : file{file} {}

void FileBoard::init() {
    initBoard();
    TileType tt;
    int input; 
    bool dice = false;
    int id = 0;
    while (file >> input) {
        if (dice && 2 <= input && input <= 12) { // Input is dice number for the tile
            tiles.emplace_back(Tile{id, input, tt, TILE_VERTICES[id], TILE_EDGES[id]});
            id++;
        } else if (! dice && 0 <= input && input <= 5) { // Input is tile type
            tt = static_cast<TileType>(input); // Convert input to TileType
        } else { // Invalid input
            cout << "Invalid Input! Please re-enter a valid number:" << endl;
            continue;
        }
        dice = ! dice;
    }
}

unique_ptr<Board> BoardFactory::createBoard(int type, unsigned seed, ifstream &file) {
    if (type == 0) {
        auto ret = make_unique<Board>(seed);
        ret->init();
        return ret;
    }
    auto ret = make_unique<Board>(file);
    ret->init();
    return ret;
}
