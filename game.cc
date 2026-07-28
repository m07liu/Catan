export module game;

import <vector>;
import <string>;
import <random>;
import <memory>;
import <optional>;
import board;
import constants;
import player;

using namespace std;

export struct EndOfInput {};

export class Game {
    unique_ptr<Board> board;
    vector<Player> players;
    int curPlayer = 0;
    int winner = -1;
    bool loaded = false;
    default_random_engine rng; 

    void prompt() const;
    string readToken();
    optional<int> readInt();

    void printPlayer(const Player &p) const; // <colour> has <numPoints> building points, <numBrick> brick, ... and <numWiFi> WiFi.
    void printAll() const; // prints the current status of all builders in order from builder 0 to 3.
    void printBuildings(const Player &p) const; // <colour> has built: ...
    void printHelp() const; // Valid commands: board, status, residences ...

    void setup();
    void playTurn();
    void beginTurn(Player &p);
    void duringTurn(Player &p);
    void processRoll(Player &p, int rollVal);
    void moveGeese(Player &p);
    void distributeResources(int rollVal);

    bool isBuildable(const Player &p, int vertexId);
    bool isPlaceable(const Player &p, int edgeId);
    void build(Player &p, int vertexId);
    void place(Player &p, int edgeId);
    bool canImprove(const Player &p, int vertexId);
    void improve(Player &p, int vertexId);
    bool canTrade(Player &p1, const string &colour, const string &give, const string &receive); // <colour1> offers <colour2> one <resource1> for one <resource2>.
    void trade(Player &p1, const string &colour, const string &give, const string &receive); 
    bool canSteal(auto c, vector<Colour> &pv);

    vector<int> housesOf(const Player &p) const; // vertex ids
    vector<int> roadsOf(const Player &p) const; // edge ids
    Player &colourToPlayer(Colour c) const;
    Colour playerToColour(const Player &p) const;
    string savePlayer(int i) const;
    string saveBoard() const;

    
  public:
    Game(unique_ptr<Board> board, unsigned seed);
    Game(int type, unsigned seed, const string & file);
    bool run();
    void save(const string &file) const;
    void load(istream &in);
    
};
