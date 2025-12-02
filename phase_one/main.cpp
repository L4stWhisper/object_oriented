// main.cpp
// 单文件实现：五子棋 + 围棋（控制台）

// 编译示例： g++ -std=c++11 main.cpp -O2 -o game.exe

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <queue>

using namespace std;

enum class Piece { EMPTY = 0, BLACK = 1, WHITE = 2 };

string pieceToChar(Piece p) {
    if (p == Piece::EMPTY) return ".";
    if (p == Piece::BLACK) return "X";
    return "O";
}

Piece opposite(Piece p) {
    if (p == Piece::BLACK) return Piece::WHITE;
    if (p == Piece::WHITE) return Piece::BLACK;
    return Piece::EMPTY;
}

struct MoveRecord {
    int x, y; // 1-based coordinates; if pass: isPass=true and x=y=0
    Piece who;
    bool isPass;
    vector<pair<int,int>> captured; // 1-based positions
    MoveRecord() : x(0), y(0), who(Piece::EMPTY), isPass(false) {}
};

class Board {
private:
    int n;
    vector<vector<Piece> > grid;
public:
    Board(int n_=9) : n(n_), grid(n_, vector<Piece>(n_, Piece::EMPTY)) {}
    int size() const { return n; }
    Piece at(int r, int c) const { return grid[r][c]; } // 0-based
    void set(int r, int c, Piece p) { grid[r][c] = p; }
    void clear() { 
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                grid[i][j] = Piece::EMPTY;
    }
    vector<vector<Piece> > snapshot() const { return grid; }
    void restore(const vector<vector<Piece> >& s) { grid = s; n = (int)s.size(); }

    string toString() const {
        ostringstream oss;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                oss << pieceToChar(grid[i][j]);
                if (j + 1 < n) oss << ' ';
            }
            if (i + 1 < n) oss << '\n';
        }
        return oss.str();
    }

    void serialize(ostream &os) const {
        os << n << "\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                os << (int)grid[i][j] << (j+1==n?'\n':' ');
            }
        }
    }

    bool deserialize(istream &is) {
        int sz;
        if (!(is >> sz)) return false;
        if (sz < 1 || sz > 999) return false;
        vector<vector<Piece> > g(sz, vector<Piece>(sz, Piece::EMPTY));
        for (int i = 0; i < sz; ++i) {
            for (int j = 0; j < sz; ++j) {
                int v; if (!(is >> v)) return false;
                if (v < 0 || v > 2) return false;
                g[i][j] = static_cast<Piece>(v);
            }
        }
        n = sz; grid = g; return true;
    }
};

struct Player {
    string name;
    Piece color;
    Player() : name(""), color(Piece::EMPTY) {}
    Player(const string &n, Piece c) : name(n), color(c) {}
};

class Game {
protected:
    Board board;
    Player black, white;
    Piece curPlayer;
    bool running;
    bool showHints;
    vector<MoveRecord> history;
public:
    Game(int n=9) : board(n), black("Black", Piece::BLACK), white("White", Piece::WHITE),
                    curPlayer(Piece::BLACK), running(false), showHints(true) {}
    virtual ~Game() {}
    virtual string gameName() const = 0;
    int size() const { return board.size(); }
    bool isRunning() const { return running; }
    Piece currentPlayer() const { return curPlayer; }

    virtual bool start(int n) {
        if (n < 8 || n > 19) return false;
        board = Board(n);
        black = Player("Black", Piece::BLACK);
        white = Player("White", Piece::WHITE);
        curPlayer = Piece::BLACK;
        running = true;
        history.clear();
        return true;
    }

    virtual string displayBoard() const {
        ostringstream oss;
        int n = board.size();
        oss << "   ";
        for (int c = 1; c <= n; ++c) {
            if (c < 10) oss << c << " ";
            else oss << c << " ";
        }
        oss << "\n";
        for (int r = 0; r < n; ++r) {
            if (r+1 < 10) oss << " " << (r+1) << " ";
            else oss << (r+1) << " ";
            for (int c = 0; c < n; ++c) {
                oss << pieceToChar(board.at(r,c));
                if (c+1 < n) oss << " ";
            }
            oss << "\n";
        }
        return oss.str();
    }

    virtual bool place(int x, int y, string &msg) = 0;
    virtual bool pass(string &msg) { msg = "pass not supported in this game"; return false; }

    virtual bool resign(string &msg) {
        running = false;
        if (curPlayer == Piece::BLACK) msg = "Black resigns. White wins.";
        else msg = "White resigns. Black wins.";
        return true;
    }

    virtual bool undo(string &msg) {
        if (history.empty()) { msg = "No move to undo."; return false; }
        MoveRecord rec = history.back();
        history.pop_back();
        if (rec.isPass) {
            curPlayer = rec.who;
            msg = "Undid a pass.";
            return true;
        } else {
            board.set(rec.x - 1, rec.y - 1, Piece::EMPTY);
            for (size_t i = 0; i < rec.captured.size(); ++i) {
                board.set(rec.captured[i].first - 1, rec.captured[i].second - 1,
                          (rec.who == Piece::BLACK ? Piece::WHITE : Piece::BLACK));
            }
            curPlayer = rec.who;
            msg = "Undo successful.";
            return true;
        }
    }

    virtual bool saveToFile(const string &fn, string &msg) {
        ofstream ofs(fn.c_str());
        if (!ofs) { msg = "Cannot open file to save."; return false; }
        ofs << gameName() << "\n";
        ofs << board.size() << " " << (int)curPlayer << "\n";
        board.serialize(ofs);
        ofs << history.size() << "\n";
        for (size_t i = 0; i < history.size(); ++i) {
            MoveRecord &m = history[i];
            ofs << m.x << " " << m.y << " " << (int)m.who << " " << (m.isPass?1:0) << " " << m.captured.size();
            for (size_t k = 0; k < m.captured.size(); ++k) {
                ofs << " " << m.captured[k].first << " " << m.captured[k].second;
            }
            ofs << "\n";
        }
        ofs.close();
        msg = "Saved to " + fn;
        return true;
    }

    virtual bool loadFromFile(const string &fn, string &msg) {
        ifstream ifs(fn.c_str());
        if (!ifs) { msg = "Cannot open file to load."; return false; }
        string gname;
        if (!(ifs >> gname)) { msg = "Bad file format"; return false; }
        int n; int cur;
        if (!(ifs >> n >> cur)) { msg = "Bad file format"; return false; }
        vector<vector<int> > raw(n, vector<int>(n,0));
        for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
            int v; if (!(ifs >> v)) { msg = "Bad file format"; return false; }
            raw[i][j] = v;
        }
        Board nb(n);
        for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) nb.set(i,j, static_cast<Piece>(raw[i][j]));
        board = nb;
        curPlayer = static_cast<Piece>(cur);
        size_t hsz = 0;
        if (!(ifs >> hsz)) hsz = 0;
        history.clear();
        for (size_t i = 0; i < hsz; ++i) {
            MoveRecord m;
            size_t capsz;
            int ispass;
            ifs >> m.x >> m.y >> (int&)m.who >> ispass >> capsz;
            m.isPass = (ispass!=0);
            for (size_t k = 0; k < capsz; ++k) {
                int a,b; ifs >> a >> b; m.captured.push_back(make_pair(a,b));
            }
            history.push_back(m);
        }
        running = true;
        msg = "Loaded from " + fn;
        return true;
    }

    virtual void toggleHints() { showHints = !showHints; }
    virtual bool hintsOn() const { return showHints; }

    virtual string showStatus() const {
        ostringstream oss;
        oss << "Game: " << gameName() << "  Size: " << board.size() << "\n";
        oss << "Current player: " << (curPlayer==Piece::BLACK? "Black (X)" : "White (O)") << "\n";
        return oss.str();
    }

    void pushMove(const MoveRecord &m) { history.push_back(m); curPlayer = opposite(m.who); }
};

class GomokuGame : public Game {
public:
    GomokuGame(int n=15) : Game(n) {}
    virtual string gameName() const { return "Gomoku"; }

    /*virtual bool place(int x, int y, string &msg) {
        if (!running) { msg = "Game not started."; return false; }
        int n = board.size();
        if (x < 1 || x > n || y < 1 || y > n) { msg = "Coordinates out of range."; return false; }
        if (board.at(x-1,y-1) != Piece::EMPTY) { msg = "Position already occupied."; return false; }
        board.set(x-1,y-1, curPlayer);
        MoveRecord rec; rec.x = x; rec.y = y; rec.who = curPlayer; rec.isPass = false;
        pushMove(rec);
        if (checkFive(x-1,y-1,curPlayer)) {
            running = false;
            if (curPlayer == Piece::BLACK) msg = "Black (X) wins by five in a row!";
            else msg = "White (O) wins by five in a row!";
            return true;
        }
        bool full = true;
        for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (board.at(i,j) == Piece::EMPTY) full = false;
        if (full) { running = false; msg = "Board full. Draw."; return true; }
        msg = "Placed.";
        return true;
    }*/
   virtual bool place(int x, int y, string &msg) {
    if (!running) { msg = "Game not started."; return false; }
    int n = board.size();
    if (x < 1 || x > n || y < 1 || y > n) { msg = "Coordinates out of range."; return false; }
    if (board.at(x-1,y-1) != Piece::EMPTY) { msg = "Position already occupied."; return false; }

    // 保存当前操作者（落子的一方）
    Piece mover = curPlayer;

    // 放子到棋盘
    board.set(x-1,y-1, mover);

    // 先判断是否连五（使用 mover，而不是 curPlayer，因为 curPlayer 可能在 pushMove 后改变）
    if (checkFive(x-1, y-1, mover)) {
        // 记录该步到历史（保持与之前行为一致）
        MoveRecord rec; rec.x = x; rec.y = y; rec.who = mover; rec.isPass = false;
        history.push_back(rec); // 直接入历史，不调用 pushMove 以避免切换玩家（游戏结束，无需切换）
        running = false;
        if (mover == Piece::BLACK) msg = "Black (X) wins by five in a row!";
        else msg = "White (O) wins by five in a row!";
        return true;
    }

    // 若未结束，仍需将这一步加入历史并切换玩家（使用已有 pushMove）
    MoveRecord rec; rec.x = x; rec.y = y; rec.who = mover; rec.isPass = false;
    pushMove(rec);

    // 检查平局（棋盘是否已满）
    bool full = true;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (board.at(i,j) == Piece::EMPTY) full = false;
    if (full) {
        running = false;
        msg = "Board full. Draw.";
        return true;
    }

    msg = "Placed.";
    return true;
}


private:
    bool checkFive(int r, int c, Piece who) {
        const int dr[4] = {0,1,1,1};
        const int dc[4] = {1,0,1,-1};
        int n = board.size();
        for (int k = 0; k < 4; ++k) {
            int cnt = 1;
            for (int s = 1; s <= 4; ++s) {
                int nr = r + dr[k]*s, nc = c + dc[k]*s;
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) break;
                if (board.at(nr,nc) == who) cnt++; else break;
            }
            for (int s = 1; s <= 4; ++s) {
                int nr = r - dr[k]*s, nc = c - dc[k]*s;
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) break;
                if (board.at(nr,nc) == who) cnt++; else break;
            }
            if (cnt >= 5) return true;
        }
        return false;
    }
};

class GoGame : public Game {
private:
    vector<vector<Piece> > previousBoard;
public:
    GoGame(int n=9) : Game(n) { previousBoard.clear(); }
    virtual string gameName() const { return "Go"; }

    virtual bool start(int n) {
        bool ok = Game::start(n);
        previousBoard.clear();
        return ok;
    }

    virtual bool pass(string &msg) {
        if (!running) { msg = "Game not started."; return false; }
        MoveRecord rec;
        rec.isPass = true;
        rec.x = rec.y = 0;
        rec.who = curPlayer;
        pushMove(rec);
        if (curPlayer == Piece::BLACK) msg = "Black passes."; else msg = "White passes.";
        return true;
    }

    virtual bool place(int x, int y, string &msg) {
        if (!running) { msg = "Game not started."; return false; }
        int n = board.size();
        if (x < 1 || x > n || y < 1 || y > n) { msg = "Coordinates out of range."; return false; }
        if (board.at(x-1,y-1) != Piece::EMPTY) { msg = "Position already occupied."; return false; }

        vector<vector<Piece> > before = board.snapshot();
        board.set(x-1,y-1, curPlayer);
        vector<pair<int,int> > removed;
        vector<pair<int,int> > adj = neighbors(x-1,y-1);
        for (size_t i = 0; i < adj.size(); ++i) {
            int r = adj[i].first, c = adj[i].second;
            if (board.at(r,c) == opposite(curPlayer)) {
                vector<pair<int,int> > grp = collectGroup(r,c);
                if (groupLiberties(grp) == 0) {
                    for (size_t k = 0; k < grp.size(); ++k) {
                        board.set(grp[k].first, grp[k].second, Piece::EMPTY);
                        removed.push_back(make_pair(grp[k].first+1, grp[k].second+1));
                    }
                }
            }
        }
        vector<pair<int,int> > mygrp = collectGroup(x-1,y-1);
        if (groupLiberties(mygrp) == 0) {
            board.restore(before);
            msg = "Illegal move: suicide is not allowed.";
            return false;
        }
        if (!previousBoard.empty()) {
            if (board.snapshot() == previousBoard) {
                board.restore(before);
                msg = "Illegal move: violates simple KO rule (repeats previous board).";
                return false;
            }
        }
        MoveRecord rec; rec.isPass = false; rec.x = x; rec.y = y; rec.who = curPlayer; rec.captured = removed;
        pushMove(rec);
        previousBoard = before;
        msg = "Placed.";
        return true;
    }

    vector<pair<int,int> > neighbors(int r, int c) const {
        vector<pair<int,int> > res;
        int n = board.size();
        if (r > 0) res.push_back(make_pair(r-1,c));
        if (r+1 < n) res.push_back(make_pair(r+1,c));
        if (c > 0) res.push_back(make_pair(r,c-1));
        if (c+1 < n) res.push_back(make_pair(r,c+1));
        return res;
    }

    vector<pair<int,int> > collectGroup(int r, int c) const {
        int n = board.size();
        Piece who = board.at(r,c);
        vector<pair<int,int> > group;
        if (who == Piece::EMPTY) return group;
        vector<vector<char> > vis(n, vector<char>(n, 0));
        queue<pair<int,int> > q;
        q.push(make_pair(r,c)); vis[r][c] = 1;
        while (!q.empty()) {
            pair<int,int> cur = q.front(); q.pop();
            group.push_back(cur);
            vector<pair<int,int> > nb = neighbors(cur.first, cur.second);
            for (size_t i = 0; i < nb.size(); ++i) {
                int nx = nb[i].first, ny = nb[i].second;
                if (!vis[nx][ny] && board.at(nx,ny) == who) {
                    vis[nx][ny] = 1; q.push(make_pair(nx,ny));
                }
            }
        }
        return group;
    }

    int groupLiberties(const vector<pair<int,int> > &group) const {
        int n = board.size();
        vector<vector<char> > mark(n, vector<char>(n, 0));
        for (size_t i = 0; i < group.size(); ++i) {
            int r = group[i].first, c = group[i].second;
            vector<pair<int,int> > nb = neighbors(r,c);
            for (size_t j = 0; j < nb.size(); ++j) {
                int nr = nb[j].first, nc = nb[j].second;
                if (board.at(nr,nc) == Piece::EMPTY) mark[nr][nc] = 1;
            }
        }
        int cnt = 0;
        for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (mark[i][j]) ++cnt;
        return cnt;
    }

    virtual bool undo(string &msg) {
        if (history.empty()) { msg = "No move to undo."; return false; }
        MoveRecord rec = history.back();
        history.pop_back();
        Board nb(board.size());
        for (size_t i = 0; i < history.size(); ++i) {
            MoveRecord &m = history[i];
            if (m.isPass) continue;
            nb.set(m.x - 1, m.y - 1, m.who);
            for (size_t k = 0; k < m.captured.size(); ++k) {
                nb.set(m.captured[k].first - 1, m.captured[k].second - 1,
                       (m.who == Piece::BLACK ? Piece::WHITE : Piece::BLACK));
            }
        }
        board = nb;
        curPlayer = rec.who;
        if (history.size() >= 1) {
            Board tb(board.size());
            for (size_t i = 0; i + 1 < history.size(); ++i) {
                MoveRecord &m = history[i];
                if (m.isPass) continue;
                tb.set(m.x - 1, m.y - 1, m.who);
                for (size_t k = 0; k < m.captured.size(); ++k) {
                    tb.set(m.captured[k].first - 1, m.captured[k].second - 1,
                           (m.who == Piece::BLACK ? Piece::WHITE : Piece::BLACK));
                }
            }
            previousBoard = tb.snapshot();
        } else previousBoard.clear();
        msg = "Undo successful.";
        return true;
    }
};

enum class GameType { GOMOKU, GO };

Game* createGame(GameType t, int n) {
    if (t == GameType::GOMOKU) return new GomokuGame(n);
    return new GoGame(n);
}

class ConsoleUI {
private:
    Game *game;
    bool runningLoop;
public:
    ConsoleUI() : game(NULL), runningLoop(true) {}
    ~ConsoleUI() { if (game) delete game; }

    void printBanner() {
        cout << "=== Board Game Platform ===\n";
        cout << "Supports: gomoku (五子棋) and go (围棋)\n";
        cout << "Type 'help' for commands.\n\n";
    }

    void repl() {
        printBanner();
        string line;
        while (runningLoop && getline(cin, line)) {
            if (line.empty()) continue;
            handleLine(line);
        }
    }

    void handleLine(const string &line) {
        vector<string> toks = split(line);
        if (toks.empty()) return;
        string cmd = toks[0];
        if (cmd == "help") { showHelp(); return; }
        else if (cmd == "start") {
            if (toks.size() < 3) { cout << "Usage: start <gomoku|go> <size>\n"; return; }
            string g = toks[1];
            int n = atoi(toks[2].c_str());
            if (n < 8 || n > 19) { cout << "Board size must be between 8 and 19.\n"; return; }
            if (game) { delete game; game = NULL; }
            if (g == "gomoku") game = createGame(GameType::GOMOKU, n);
            else if (g == "go") game = createGame(GameType::GO, n);
            else { cout << "Unknown game type.\n"; return; }
            if (game->start(n)) cout << g << " started, size " << n << ".\n";
            else cout << "Failed to start " << g << ".\n";
            showBoardAndStatus();
            return;
        } else if (cmd == "place") {
            if (!ensureGame()) return;
            if (toks.size() < 3) { cout << "Usage: place x y\n"; return; }
            int x = atoi(toks[1].c_str()), y = atoi(toks[2].c_str());
            string msg;
            if (game->place(x,y,msg)) {
                cout << msg << "\n";
                showBoardAndStatus();
                if (!game->isRunning()) cout << "Game ended: " << msg << "\n";
            } else cout << "Failed: " << msg << "\n";
            return;
        } else if (cmd == "pass") {
            if (!ensureGame()) return;
            string msg;
            if (game->pass(msg)) { cout << msg << "\n"; showBoardAndStatus(); }
            else cout << "Failed: " << msg << "\n";
            return;
        } else if (cmd == "undo") {
            if (!ensureGame()) return;
            string msg;
            if (game->undo(msg)) { cout << msg << "\n"; showBoardAndStatus(); }
            else cout << "Failed: " << msg << "\n";
            return;
        } else if (cmd == "resign") {
            if (!ensureGame()) return;
            string msg;
            if (game->resign(msg)) { cout << msg << "\n"; showBoardAndStatus(); }
            else cout << "Failed: " << msg << "\n";
            return;
        } else if (cmd == "save") {
            if (!ensureGame()) return;
            if (toks.size() < 2) { cout << "Usage: save filename\n"; return; }
            string fn = toks[1];
            string msg;
            if (game->saveToFile(fn, msg)) cout << msg << "\n";
            else cout << "Failed: " << msg << "\n";
            return;
        } else if (cmd == "load") {
            if (toks.size() < 2) { cout << "Usage: load filename\n"; return; }
            string fn = toks[1];
            ifstream ifs(fn.c_str());
            if (!ifs) { cout << "Cannot open file.\n"; return; }
            string gname; ifs >> gname; ifs.close();
            Game *temp = NULL;
            if (gname == "Gomoku") temp = createGame(GameType::GOMOKU, 9);
            else temp = createGame(GameType::GO, 9);
            string msg;
            if (temp->loadFromFile(fn, msg)) { if (game) delete game; game = temp; cout << msg << "\n"; showBoardAndStatus(); }
            else { delete temp; cout << "Failed: " << msg << "\n"; }
            return;
        } else if (cmd == "restart") {
            if (game) { delete game; game = NULL; }
            cout << "Game reset. Use 'start' to begin a new game.\n";
            return;
        } else if (cmd == "hint") {
            if (!ensureGame()) return;
            game->toggleHints();
            cout << (game->hintsOn()? "Hints: ON" : "Hints: OFF") << "\n";
            return;
        } else if (cmd == "show") {
            if (!ensureGame()) return;
            showBoardAndStatus(); return;
        } else if (cmd == "quit" || cmd == "exit") {
            cout << "Bye.\n"; runningLoop = false; return;
        } else {
            cout << "Unknown command. Type 'help' for a list of commands.\n";
            return;
        }
    }

    void showHelp() {
        cout << "Commands:\n";
        cout << "  start gomoku <n>   -- start gomoku, size n (8-19)\n";
        cout << "  start go <n>       -- start go, size n (8-19)\n";
        cout << "  place x y          -- place at row x, col y (1-based)\n";
        cout << "  pass               -- pass (go only)\n";
        cout << "  undo               -- undo last move\n";
        cout << "  resign             -- current player resigns\n";
        cout << "  save filename      -- save current board to file\n";
        cout << "  load filename      -- load board from file\n";
        cout << "  restart            -- restart (clear current game)\n";
        cout << "  hint               -- toggle hints on/off\n";
        cout << "  show               -- display board and status\n";
        cout << "  quit               -- exit\n";
    }

    void showBoardAndStatus() {
        if (!game) { cout << "No game running. Use 'start'.\n"; return; }
        cout << game->displayBoard() << "\n";
        cout << game->showStatus();
        if (game->hintsOn()) {
            cout << "Hints: Use 'place x y' to put a stone. 'undo' to undo. 'save/load' to persist.\n";
        }
    }

    bool ensureGame() {
        if (!game) { cout << "No game started. Use 'start gomoku <n>' or 'start go <n>'.\n"; return false; }
        return true;
    }

    static vector<string> split(const string &s) {
        vector<string> res;
        string cur;
        stringstream ss(s);
        while (ss >> cur) res.push_back(cur);
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    ConsoleUI ui;
    ui.repl();
    return 0;
}
