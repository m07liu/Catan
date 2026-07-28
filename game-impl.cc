module game;

import <vector>;
import <string>;
import <random>;
import <memory>;
import <iostream>;
import <fstream>;
import <sstream>;
import <map>;
import board;
import constants;
import player;
import tile;
import edge;
import vertex;

using namespace std;


Game::Game(int type, unsigned seed, const string & file, default_random_engine &rng) : rng{seed} {
    ifstream source {file};
    auto factory = make_unique<BoardFactory>();
    board = factory->createBoard(type, seed, source);
    players.reserve(4);
    for (int i = 0; i < 4; ++i) {
        players.emplace_back(static_cast<Colour>(i));
    }
}

void Game::prompt() const { cout << "> "; }
string Game::readToken() {
    string str;
    if (!(cin >> str)) throw EndOfInput{};
    return str;
}

// helper function
optional<int> Game::readInt() {
    string str = readToken();
    try {
        return stoi(str);
    } catch (...) {
        return nullopt;
    }
}

void Game::printPlayer(const Player &p) const {
    const Inventory &r = p.getResources();
    cout << colourToString(p.getColour()) << " has " << p.getPoints() << " building points, "
         << r.bricks << " brick, " << r.energy << " energy, " << r.glass << " glass, " << r.heat << " heat, and " << r.wifi << " WiFi." << endl;
}
void Game::printAll() const {
    for (int i = 0; i < 4; ++i) {
        printPlayer(players[i]);
    }
}

void Game::printBuildings(const Player &p) const {
    cout << colourToString(p.getColour()) << " has built:\n";
    for (pair<int, BuildingLevel> pr: board->buildingsOwnedBy(p.getColour())) {
        cout << pr.first << " " << buildingLevelToChar(pr.second) << "\n";
    }
}

void Game::printHelp() const {
    cout << "Valid commands:\n"
         << "board\n"
         << "status\n"
         << "residences\n"
         << "build-road <edge#>\n"
         << "build-res <housing#>\n"
         << "improve <housing#>\n"
         << "trade <colour> <give> <take>\n"
         << "next\n"
         << "save <file>\n"
         << "help\n";
}


// each player places two basements
void Game::setup() {
    vector<int> order{0, 1, 2, 3, 3, 2, 1, 0};
    for (int i = 0; i < 8; ++i) {
        int idx = order[i];
        Player &p = players[idx];
        Colour c = p.getColour();

        int n;
        while (true) {
            cout << "Builder " << colourToString(c) << ", where do you want to build a basement?" << endl;
            prompt();

            try { n = stoi(readToken()); } 
            catch (EndOfInput &) { throw; } 
            catch (...) { cout << "You cannot build here." << endl; continue; }

            if (!board->canBuild(n, c, true)) {
                cout << "You cannot build here." << endl; continue;
            }

            break;
        }

        board->build(n, c);
        p.addPoints(1);
        if (i >= 4) {
            Inventory inventory;
            for (int id: board->findVertex(n).getAdjTiles()) {
                const Tile &t = board->findTile(id);
                if (t.getType() != TileType::PARK) inventory[static_cast<int>(t.getType())] += 1;
            }
            p.addResources(inventory);
        }

    }

    cout << *board;

}

void Game::playTurn() {
    Player &p = players[curPlayer];
    beginTurn(p);
    duringTurn(p);
}

void Game::beginTurn(Player &p) {
    cout << *board;
    cout << "Builder " << colourToString(p.getColour()) << "'s turn." << endl;
    printPlayer(p);

    while (true) {
        prompt();
        string cmd = toLower(readToken());
        if (cmd == "load") { p.setDice(1); }
        else if (cmd == "fair") { p.setDice(0); }
        else if (cmd == "roll") {
            processRoll(p, p.roll(rng));
            return;
        }
        else {
            cout << "Invalid command." << endl;
        }
    }
}
void Game::duringTurn(Player &p) {
    while (true) {
        prompt();
        string cmd = toLower(readToken());

        if (cmd == "board") {
            cout << *board;

        } else if (cmd == "status") {
            printAll();

        } else if (cmd == "residences") {
            printBuildings(p);

        } else if (cmd == "build-road") {
            auto num = readInt();
            if (!num) cout << "You cannot build here." << endl;
            else if (isPlaceable(p, *num)) place(p, *num);

        } else if (cmd == "build-res") {
            auto num = readInt();
            if (!num) cout << "You cannot build here." << endl;
            else if (isBuildable(p, *num)) build(p, *num);
            
        } else if (cmd == "improve") {
            auto num = readInt();
            if (!num) cout << "You cannot build here." << endl;
            else if (canImprove(p, *num)) improve(p, *num);

        } else if (cmd == "trade") {
            string toWhom = readToken();
            string give = readToken();
            string receive = readToken();
            trade(p, toWhom, give, receive);

        } else if (cmd == "next") {
            return;
        } else if (cmd == "save") {
            save(readToken());
        } else if (cmd == "help") {
            printHelp();
        } else {
            cout << "Invalid command." << endl;
        }
        if (winner != -1) return;
    }
}


void Game::processRoll(Player &p, int rollVal) {
    if (rollVal == 7) moveGeese(p);
    else distributeResources(rollVal);
}

// helper function
int pickRandomResource(const Inventory &inventory, default_random_engine &rng) {
    int n = rng() % inventory.total() + 1;
    for (int i = 0; i < 5; ++i) {
        n -= inventory[i];
        if (n <= 0) return i;
    }

    return 4;
}


// helper function
bool Game::canSteal(const optional<Colour> &c, vector<Colour> &pv) {
    if (!c) {
        cout << "Invalid command." << endl;
        return false;
    }

    for (int i = 0; i < pv.size(); i++) {
        if (pv[i] == *c) return true;
    }

    cout << "Invalid command." << endl;
    return false;

}

void Game::moveGeese(Player &p) {

    // lose resources if total >= 10
    for (int i = 0; i < 4; ++i) {
        Player &victim = players[i];
        Inventory inventory = victim.getResources();
        int total = inventory.total();
        if (total < 10) continue;

        int loseNum = total / 2;
        Inventory res = inventory;
        Inventory lost;
        for (int j = 0; j < loseNum; ++j) {
            int idx = pickRandomResource(res, rng);
            lost[idx] += 1;
            res[idx] -= 1;
        }
        victim.giveResources(lost);

        cout << "Builder " << colourToString(victim.getColour()) << " loses " << loseNum << " resources to the geese. They lose:\n";
        for (int j = 0; j < 5; ++j) {
            int n = lost[j];
            if (n > 0) cout << n << " " << invnumToResource(j) << "\n";
        }
    }

    // Choose where to place the GEESE.
    int n;
    while (true) {
        cout << "Choose where to place the GEESE.\n";
        prompt();

        try { n = stoi(readToken()); } 
        catch (EndOfInput &) { throw; } 
        catch (...) { continue; }

        if (n < 0 || n >= NUM_TILES || n == board->getGeeseTile()) continue;

        break;
    }
    board->moveGeese(n);


    // find possible players to steal from
    vector<bool> possibleVictims(4, false);
    for (int id: board->findTile(n).getAdjVertices()) {
        const Vertex &vertex = board->findVertex(id);
        if (vertex.hasBuilding() && vertex.getOwner() != p.getColour() && players[static_cast<int>(vertex.getOwner())].getResources().total() != 0) {
            Colour c = vertex.getOwner();
            possibleVictims[static_cast<int>(c)] = true;
        }
    }

    vector<Colour> pv;
    for (int i = 0; i < 4; ++i) {
        if (possibleVictims[i]) pv.push_back(static_cast<Colour>(i));
    }
    if (pv.empty()) {
        cout << "Builder " << colourToString(p.getColour()) << " has no builders to steal from." << endl; 
        return; 
    }

    cout << "Builder " << colourToString(p.getColour()) << " can choose to steal from ";
    for (int i = 0; i < pv.size(); ++i) {
        if (i != 0) cout << ", ";
        cout << colourToString(pv[i]);
    }
    cout << "." << endl;

    // Choose a builder to steal from.
    optional<Colour> target;
    while (true) {
        cout << "Choose a builder to steal from." << endl;
        prompt();
        target = parseColour(readToken());
        if (!canSteal(target, pv)) continue;
        break;
    }

    Player &victim = players[static_cast<int>(*target)];
    int pick = pickRandomResource(victim.getResources(), rng);
    victim.giveResources(singletonInv(pick));
    p.addResources(singletonInv(pick));
    cout << "Builder " << colourToString(p.getColour()) << " steals " << invnumToResource(pick) << " from builder " << colourToString(*target) << "." << endl;

}

void Game::distributeResources(int rollVal) {
    vector<Inventory> distribute(4);
    for (int tid: board->findTiles(rollVal)) {
        const Tile &tile = board->findTile(tid);
        if (tile.getType() == TileType::PARK || tile.hasGeese()) continue;
        for (int id: tile.getAdjVertices()) {
            const Vertex &v = board->findVertex(id);
            if (v.hasBuilding()) {
                Colour c = v.getBuilding().owner;
                distribute[static_cast<int>(c)][static_cast<int>(tile.getType())] += v.getBuilding().yield();
            }
        }
    }

    vector<int> gained;
    for (int i = 0; i < 4; ++i) {
        if (!distribute[i].isEmpty()) gained.push_back(i);
    }

    if (gained.empty()) {
        cout << "No builders gained resources." << endl;
        return;
    }

    for (int n : gained) {
        players[n].addResources(distribute[n]);
        cout << "Builder " << colourToString(static_cast<Colour>(n)) << " gained:\n";
        for (int i = 0; i < 5; ++i) {
            int num = distribute[n][i];
            if (num > 0) {
                cout << num << " " << invnumToResource(i) << "\n";
            }
        }
    }

}


bool Game::isBuildable(const Player &p, int vertexId) {
    if (vertexId < 0 || vertexId >= NUM_VERTICES || !board->canBuild(vertexId, p.getColour())) {
        cout << "You cannot build here." << endl;
        return false;
    }
    if (!p.canAfford(BuildingCosts.at(BuildingLevel::BASEMENT))) {
        cout << "You do not have enough resources." << endl;
        return false;
    }
    return true;
}

bool Game::isPlaceable(const Player &p, int edgeId) {
    if (edgeId < 0 || edgeId >= NUM_EDGES || !board->canPlaceRoad(edgeId, p.getColour())) {
        cout << "You cannot build here." << endl;
        return false;
    }
    if (!p.canAfford(RoadCost)) {
        cout << "You do not have enough resources." << endl;
        return false;
    }
    return true;
}

void Game::build(Player &p, int vertexId) {
    if (!isBuildable(p, vertexId)) return;

    board->build(vertexId, p.getColour());
    p.giveResources(BuildingCosts.at(BuildingLevel::BASEMENT));
    p.addPoints(1);
    if (p.getPoints() >= 10) winner = static_cast<int>(p.getColour());

}
void Game::place(Player &p, int edgeId) {
    if (!isPlaceable(p, edgeId)) return;

    board->placeRoad(edgeId, p.getColour()); 
    p.giveResources(RoadCost);

}

bool Game::canImprove(const Player &p, int vertexId) {
    if (!board->canUpgrade(vertexId, p.getColour())) {
        cout << "You cannot build here." << endl;
        return false;
    }

    Inventory bc;
    if (board->findVertex(vertexId).getBuilding().level == BuildingLevel::BASEMENT) bc = BuildingCosts.at(BuildingLevel::HOUSE);
    else bc = BuildingCosts.at(BuildingLevel::TOWER); 

    if (!p.canAfford(bc)) {
        cout << "You do not have enough resources." << endl;
        return false;
    }
    return true;

}

void Game::improve(Player &p, int vertexId) {
    if (!canImprove(p, vertexId)) return;

    Inventory bc;
    if (board->findVertex(vertexId).getBuilding().level == BuildingLevel::BASEMENT) bc = BuildingCosts.at(BuildingLevel::HOUSE);
    else bc = BuildingCosts.at(BuildingLevel::TOWER); 

    p.giveResources(bc);
    board->improve(vertexId); 
    p.addPoints(1);
    if (p.getPoints() >= 10) winner = static_cast<int>(p.getColour());

}

bool Game::canTrade(Player &p1, const string &colour, const string &give, const string &receive) {
    auto r1 = parseResourceIdx(give);
    auto r2 = parseResourceIdx(receive);
    auto c = parseColour(colour);

    if (!c || !r1 || !r2 || *c == p1.getColour()) {
        cout << "Invalid command." << endl;
        return false;
    }

    if (p1.getResources()[*r1] == 0) {
        cout << "You do not have enough resources." << endl;
        return false;
    }

    Player &p2 = players[static_cast<int>(*c)];
    if (p2.getResources()[*r2] == 0) {
        cout << "The other player doesn't have enough resources." << endl;
        return false;
    }

    return true;
}

void Game::trade(Player &p1, const string &colour, const string &give, const string &receive) {
    if (!canTrade(p1, colour, give, receive)) return;

    auto r1 = parseResourceIdx(give);
    auto r2 = parseResourceIdx(receive);
    auto c = parseColour(colour);

    cout << colourToString(p1.getColour()) << " offers " << colourToString(*c) << " one " << invnumToResource(*r1) << " for one " << invnumToResource(*r2) << "." << endl;

    while (true) {
        cout << "Does " << colourToString(*c) << " accept this offer?" << endl;
        prompt();
        string str = toLower(readToken());
        if (str == "yes") break;
        if (str == "no") return;
        cout << "Invalid command." << endl;
        
    }

    Player &p2 = players[static_cast<int>(*c)];
    p1.giveResources(singletonInv(*r1));
    p2.giveResources(singletonInv(*r2));
    p1.addResources(singletonInv(*r2));
    p2.addResources(singletonInv(*r1));


}

string Game::savePlayer(int i) const {
    ostringstream ss;
    for (int j = 0; j < 5; ++j) {
        if (j != 0) ss << " ";
        ss << players[i].getResources()[j];
    }
    ss << "r";
    for (int id: board->roadsOwnedBy(static_cast<Colour>(i))) {
        ss << " " << id;
    }
    ss << " h";
    for (pair<int, BuildingLevel> pr: board->buildingsOwnedBy(static_cast<Colour>(i))) {
        ss << " " << pr.first << " " << buildingLevelToChar(pr.second);
    }
    return ss.str();

}
string Game::saveBoard() const {
    ostringstream ss;
    for (int i = 0; i < NUM_TILES; ++i) {
        const Tile &tile = board->findTile(i);
        if (i != 0) ss << " ";
        ss << static_cast<int>(tile.getType()) << " " << tile.getVal();
    } 

    return ss.str();

}

void Game::save(const string &file) const {
    ofstream fout{file};
    fout << (curPlayer + 1) % 4 << endl;
    for (int i = 0; i < 4; ++i) {
        fout << savePlayer(i) << endl;
    }
    fout << saveBoard() << endl;
    fout << board->getGeeseTile() << endl;

}

void Game::load(istream &in) {
    loaded = true;
    in >> curPlayer;
    in.ignore();

    string line;
    for (int i = 0; i < 4; ++i) {
        getline(in, line); // player lines
        istringstream ss{line};

        Inventory inv;
        for (int i = 0; i < 5; ++i) {
            int num;
            ss >> num;
            inv[i] += num;
        }
        players[i].addResources(inv);

        string road;
        ss >> road; // "r"

        while (ss >> road && road != "h") {
            board->placeRoad(stoi(road), static_cast<Colour>(i));
        }

        int vertexId;
        char level;
        while (ss >> vertexId >> level) {
            board->build(vertexId, static_cast<Colour>(i));
            players[i].addPoints(1);
            if (level == 'H') {
                board->improve(vertexId);
                players[i].addPoints(1);
            }
            if (level == 'T') {
                board->improve(vertexId);
                board->improve(vertexId);
                players[i].addPoints(2);
            }
        }

    }

    getline(in, line); 
    istringstream bs{line};
    board->setLayout(bs);

    int geeseLoc;
    in >> geeseLoc;
    board->moveGeese(geeseLoc);

}

bool Game::run() {
    try {
        if (!loaded) setup();
        while (winner == -1) {
            playTurn();
            curPlayer = (curPlayer + 1) % 4;
        }
        cout << colourToString(players[winner].getColour()) << " wins!" << endl;

        while (true){
            cout << "Would you like to play again?" << endl;
            prompt();
            string str = toLower(readToken());
            if (str == "yes") return true;
            else if (str == "no") return false;
            
            cout << "Invalid command." << endl;
        }


    } catch (EndOfInput &e) {
        save("backup.sv");
        return false;
    }
    
    return false;

}


