export module tile;

import <vector>;

using namespace std;

export class Tile {
    int id;
    vector<Vertex *> vertices;
    vector<Edge *> edges; 
  public:
    
}

export class Vertex {
    int id;
    vector<Edge *> edges;
  public:
    
}

export class Edge {
    int id;
    
    vector<Vertex *> vertices;
    vector<Edge *> edges;
  public:
    
}