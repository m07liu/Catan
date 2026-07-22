module board;

import <cstdlib>;
import <vector>;
import <map>;
import <iostream>;
import <fstream>;
import <string>;
import tile;
import edge;
import vertex;

using namespace std;

void Board::addTile();

void Board::addVertex();

void Board::addEdge();

void Board::display();

void Board::save();

void Board::giveResources(int dieVal);

const Tile &Board::findTiles(int id) const;

const vector<Tile> &Board::findVertex(int val) const;

const Edge &Board::findEdge(int id) const;

int Board::getGeeseTile() const { return geeseTile; }

void Board::moveGeese(int id);

bool Board::canBuild(int id, Colour c) const;

bool Board::canPlaceRoad(int id, Colour c) const;

Board::~Board();

//------------------------------Concrete Products-------------------------------

RandomBoard::RandomBoard(int seed) : seed{seed} {}

void RandomBoard::init() {
    // use seeded random gen here
}

FileBoard::FileBoard(ifstream &file) : file{file} {}

void FileBoard::init() {
    int input; 
    
}
