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

using namespace std;


Game::Game(int type, unsigned seed, const string & file) {
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
optional<int> readInt() {
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
    for (int id: houseOf(p)) {
        cout << id << " " << buildingLevelToChar(board->findVertex(id).getBuilding().level) << "\n";
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
            catch (...) { cout << "You cannot build here." << endl; continue; }

            if (n < 0 || n >= NUM_VERTICES || !board->findVertex(n).hasBuilding()) {
                cout << "You cannot build here." << endl; continue;
            }

            break;
        }

        board->findVertex(n).build(c);
        p.addPoints(1);
        if (i >= 4) {
            Inventory inventory;
            for (int id: board->findVertex(n).adjTiles) {
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
    cout << "Builder " << colourToString(p.getColour(c)) << "'s turn." << endl;
    printPlayer(p);

    while (true) {
        prompt();
        string cmd = readToken();
        if (cmd) == "load") { p.setDice(make_unique<LoadedDice>()); }
        else if (cmd == "fair") { p.setDice(make_unique<FairDice>()); }
        else if (cmd == "roll") {
            processRoll(p, p.roll());
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
        string cmd = readToken();

        if (cmd == "board") {
            cout << *board;

        } else if (cmd == "status") {
            printAll();

        } else if (cmd == "residences") {
            printBuildings(p);

        } else if (cmd == "build-road") {
            auto num = readInt();
            if (!num) cout << "You cannot build here." << endl;
            else if (isPlaceable(p, num)) place(p, num);

        } else if (cmd == "build-res") {
            auto num = readInt();
            if (!num) cout << "You cannot build here." << endl;
            else if (isBuildable(p, num)) build(p, num);
            
        } else if (cmd == "improve") {
            auto num = readInt();
            if (!num) cout << "You cannot build here." << endl;
            else if (canImprove(p, num)) improve(p, num);

        } else if (cmd == "trade") {
            string toWhom = readToken();
            string give = readToken();
            string receive = readToken();
            trade(p, toWhom, give, take);

        } else if (cmd == "next") {
            return;
        } else if (cmd == "save") {
            save(readToken());
        } else if (cmd == "help") {
            printHelp();
        } else {
            cout << "Invalid command." << endl;
        }
    }
}


void Game::processRoll(Player &p, int rollVal) {
    if (rollVal == 7) moveGeese(p);
    else distributeResources(rollVal);
}

// helper function
int pickRandomResource(const Inventory &inventory, default_random_engine &rng) {
    int n = rng() % inv.total() + 1;
    for (int i = 0; i < 5; ++i) {
        n -= inventory[i];
        if (n <= 0) return i;
    }

    return 4;
}


// helper function
bool Game::canSteal(auto c, vector<Colour> &pv) {
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
        catch (...) { continue; }

        if (n < 0 || n >= NUM_TILES || n == board->getGeeseTile()) continue;

        break;
    }
    board->moveGeese(n);


    // find possible players to steal from
    vector<bool> possibleVictims(4, false);
    for (int id: board->findTile(n).getAdjVertices()) {
        const Vertex &vertex = board->findVertex(id);
        if (vertex.hasBuilding() && vertex.getColour() != p.getColour() && players[i].getResources().total() != 0) {
            Colour c = vertex.getColour();
            possibleVictims[static_cast<int>(c)] = true;
        }
    }

    vector<Colour> pv;
    for (int i = 0; i < 4; ++i) {
        if (possibleVictims[i]) pv.push_back(static_cast<Colour>(i));
    }
    if (pv.empty()) {
        cout << cout << "Builder " << colourToString(p.getColour()) << " has no builders to steal from." << endl; 
        return; 
    }

    cout << "Builder " << colourToString(p.getColour()) << " can choose to steal from ";
    for (int i = 0; i < pv.size(); ++i) {
        if (i != 0) cout << ", ";
        cout << colourToString(pv[i]);
    }
    cout << "." << endl;

    // Choose a builder to steal from.
    auto target;
    while (true) {
        cout << "Choose a builder to steal from." << endl;
        prompt();
        target = parseColour(readToken());
        if (!canSteal(target, pv)) continue;
        break;
    }

    Player &victim{*target};
    int pick = pickRandomResource(victim.getResources(), rng);
    victim.giveResources(singletonInv(pick));
    player.addResources(singletonInv(pick));
    cout << "Builder " << colourToString(p.getColour()) << " steals " << numToResource(idx) << " from builder " << colourToString(target) << "." << endl;

}

void Game::distributeResources(int rollVal) {
    const vector<Tile> &tiles = board->findTiles(rollVal);
    vector<Inventory> distribute(4);
    for (const Tile &tile: tiles) {
        if (tile.getType() == TileType::PARK || tile.hasGeese()) continue;
        for (int id: tile.getAdjVertices()) {
            const Vertex &v = board->getVertex(id);
            if (v.hasBuilding()) {
                Colour c = v.getBuilding().owner;
                distribute[static_cast<int>(c)][static_cast<int>(tile.getType())] += v.getBuilding().yield();
            }
        }
    }

    vector<Colour> gained;
    for (int i = 0; i < 4; ++i) {
        if (!distribute[i].isEmpty()) gained.push_back(static_cast<Colour>(i));
    }

    if (gained.empty()) {
        cout << "No builders gained resources." << endl;
        continue;
    }

    for (Colour c : gained) {
        cout << "Builder " << colourToString(c) << " gained:\n";
        for (int i = 0; i < 5; ++i) {
            int num = distribute[static_cast<int>(c)][i]
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
    if (!p.canAfford(BuildCosts.at(BuildingLevel::BASEMENT))) {
        cout << "You do not have enough resources." << endl;
        return false;
    }
    return true;
}

bool Game::isPlaceable(const Player &p, int edgeId) {
    if (edgeId < 0 || edgeId >= NUM_EDGES || !board->canPlace(edgeId, p.getColour())) {
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

    board->findVertex(vertexId).build(p.getColour());
    p.giveResources(BuildCosts.at(BuildingLevel::BASEMENT));
    p.addPoints(1);
    if (p.getPoints() >= 10) winner = static_cast<int>(p.getColour());

}
void Game::place(Player &p, int edgeId) {
    if (!isPlaceable(p, edgeId)) return;

    board->findEdge(edgeId).placeRoad(p.getColour());
    p.giveResources(Roadcost);

}

bool Game::canImprove(const Player &p, int vertexId) {
    if (vertexId < 0 || vertexId >= NUM_VERTICES || !board->findVertex(vertexId).hasBuilding() 
        || board->findVertex(vertexId).getBuilding().level == BuildingLevel::TOWER) {
        
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
    board->findVertex(vertexId).upgradeBuilding();
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

    if (p1.getResources[*r1] == 0) {
        cout << "You do not have enough resources." << endl;
        return false;
    }

    Player &p2 = players[static_cast<int>(*c)];
    if (p2.getResource[*r2] == 0) {
        cout << "The other player don't have enough resources." << endl;
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
    cout << "Does " << colourToString(*c) << " accept this offer?" << endl;
    prompt();
    if (readToken() != "yes") return;

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
    in >> playerTurn;
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
        ss >> road; "r"

        int edgeId;
        while (ss >> edgeId && edgeId != "h") {
            board->placeRoad(edgeId, static_cast<Colour>(i));
        }

        int vertexId;
        char level;
        while (ss >> vertexId >> level) {
            board->build(vertexId, static_cast<Colour>(i));
            players[i].addPoints(1);
            if (level == 'H') {
                board.improve(vertexId);
                players[i].addPoints(1);
            }
            if (level == 'T') {
                board.improve(vertexId);
                board.improve(vertexId);
                players[i].addPoints(2);
            }
        }

    }

    getline(in, line); // board line

    int geeseLoc;
    in >> geeseLoc;
    board->moveGeese(geese);

}

bool Game::run() {
    try {
        if (!loaded) setup();
        while (winner == -1) {
            playTurn();
            playerTurn = (playerTurn + 1) % 4;
        }
        cout << colourToString(players[winner].getColour()) << " wins!" << endl;
        cout << "Would you like to play again?" << endl;
        prompt();
        while (readToken() != "yes" && readToken() != "no") {
            cout << "Invalid command." << endl;
            prompt();
        }
        if (readToken() == "yes") return true;
        else return false;

    } catch (EndOfInput &e) {
        save("backup.sv");
        return false;
    }

}


