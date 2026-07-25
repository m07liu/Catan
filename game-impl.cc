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


Game::Game(unique_ptr<Board> board, unsigned seed): board{move(board)}, rng{seed} {
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
            // ***************** need to turn toWhom from string to color to player ****************//
            tryTrade(p, toWhom, give, take);

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
    Colour target;
    while (true) {
        cout << "Choose a builder to steal from." << endl;
        prompt();
        // check string
        // string to colour

    }
    Player &victim{target};
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



bool Game::isBuildable(const Player &p, int vertexId);
bool Game::isPlaceable(const Player &p, int edgeId);
void Game::build(Player &p, int vertexId);
void Game::place(Player &p, int edgeId);
bool Game::canImprove(const Player &p, int vertexId);
void Game::improve(Player &p, int vertexId);
bool Game::canTrade(Player &p1, Player &p2, const string &give, const string &receive);


