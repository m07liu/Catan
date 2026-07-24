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
        else if (20 <= i && i <= 25) edges.emplace_back(Edge{i, vector<int>{2, 3}});
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

const Tile &Board::findTiles(int id) const;

const vector<Tile> &Board::findVertex(int val) const;

const Edge &Board::findEdge(int id) const;

int Board::getGeeseTile() const { return geeseTile; }; 

void Board::moveGeese(int id);

bool Board::canBuild(int id, Colour c) const;

bool Board::canPlaceRoad(int id, Colour c) const;

Board::~Board();

//------------------------------Concrete Products-------------------------------

RandomBoard::RandomBoard(int seed) : seed{seed} {}

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
    int park = 0; // tile id for the park
    for (auto iter : dicenums) {
        if (iter == 7) break;
        park++;
    }
    for (int i = 0; i < 18; i++) {
        
    }
}

FileBoard::FileBoard(ifstream &file) : file{file} {}

void FileBoard::init() {
    initBoard();
    int input; 

}
