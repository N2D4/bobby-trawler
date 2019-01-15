#include "bobby-trawler.h"

const std::vector<std::string> WHITESPACES = {" ", "\t", "\n", "\r\n"};


int main() {
    std::cout << "Type uci, then follow up with isready to get started" << std::endl;

    // Do the UCI stuff (it's how we talk to the UI)
    waitforcmd("uci");
    std::cout << "id name BobbyTrawler" << std::endl;
    std::cout << "id author some cool people" << std::endl;
    std::cout << "uciok" << std::endl;

    // Tell the GUI we're ready
    waitforcmd("isready");
    std::cout << "readyok" << std::endl;

    std::cout << "Alright! Now enter a 4-character move, eg. 'e2e4', 'back' to take a move back, or 'go' to ask the engine. Type help for more" << std::endl;




    // Ask for moves, or let the AI play
    ChessBoard board = ChessBoard();
    TannerEngine engine(board);
    int goRemaining = 0;   // number of moves the AI should play automatically instead of asking the user
    while (true) {
        // Print out board and message if check
        std::cout << board.toHumanReadable(true) << std::endl;
        if (board.isCheck()) std::cout << "Check!" << std::endl;

        // Read in what the user entered, if the AI isn't set to play automatically
        std::string movestr;
        if (goRemaining <= 0) {
            std::cout << "Move: ";
            std::cin >> movestr;
            // If the user enters 'go4evah', make the AI start playing automatically
            if (movestr == "go4evah") goRemaining = 10000;
            if (movestr == "go4") {
                std::cout << "Number of moves: " << std::endl;
                std::cin >> goRemaining;
            }
        }

        if (movestr == "help" || movestr == "h") {
            std::cout << "==== AVAILABLE COMMANDS ====" << std::endl;
            std::cout << "  A#A#P: 4 character move, whereas A is a character from a-h and # a number from 1-8 and P is optionally a promoted piece (Q/R/B/N, default Q)" << std::endl;
            std::cout << "  back: Take back the most recent move" << std::endl;
            std::cout << "  cachesize: Display cache size info" << std::endl;
            std::cout << "  check: Display check status" << std::endl;
            std::cout << "  go: Make engine play a single move" << std::endl;
            std::cout << "  go4: Make engine play a custom number of moves" << std::endl;
            std::cout << "  go4evah: Make engine play forever (actually, 10000 moves)" << std::endl;
            std::cout << "  help: Show this help page" << std::endl;
            std::cout << "  info: Display board position info" << std::endl;
            std::cout << "  legals: Show legal moves for a piece" << std::endl;
            std::cout << "  rem: Remove a piece" << std::endl;
            std::cout << "  resetcache: Empty the cache" << std::endl;
            std::cout << "  score: Display raw material score" << std::endl;
            std::cout << "  swcol: Switch current color" << std::endl;
            std::cout << "  undo: Alias for back" << std::endl;
            std::cout << "Available aliases are b(ack), g(o), h(elp), i(nfo), r(em), sc(ore)" << std::endl;
        } else if (movestr == "rem" || movestr == "r") {
            std::cout << "Removing a piece. Position: ";
            std::string remstr;
            std::cin >> remstr;
            board[remstr] = BoardSquares::EMPTY;
            std::cout << "Removed piece. Note that this might cause glitches, eg. w/ castling if you remove a rook" << std::endl;
        } else if (movestr == "undo" || movestr == "back" || movestr == "b") {
            board.revert();
            std::cout << "Reverted a move" << std::endl;
        } else if (movestr == "swcol") {
            board.curColor = !board.curColor;
            std::cout << "It is now " << (board.curColor == BoardSquares::Colors::WHITE ? "White" : "Black") << "'s turn!" << std::endl;
        } else if (movestr == "check") {
            std::cout << "King positions: " << std::string(board.kingPos.white) << " " << std::string(board.kingPos.black) << std::endl;
            std::cout << "isCheck(white): " << board.isCheck(BoardSquares::Colors::WHITE) << std::endl;
            std::cout << "isCheck(black): " << board.isCheck(BoardSquares::Colors::BLACK) << std::endl;
        } else if (movestr == "score" || movestr == "sc") {
            std::cout << "Material score: " << board.getMaterialScore() << std::endl;
        } else if (movestr == "info" || movestr == "i") {
            std::cout << board.getInfo(true) << std::endl;
        } else if (movestr == "cachesize") {
            std::cout << "Cache size: " << engine.getMemoizationCount() << " entries" << std::endl;
            std::cout << "Cache hit rates:" << std::endl;
            for (int i = 0; i < 33; i++) {
                if (engine.cacheCalls[i] <= 0) continue;
                std::cout << "  " + std::to_string(i) + ": " + std::to_string(100.0 * engine.cacheHits[i] / (float) engine.cacheCalls[i]) + "% of " + std::to_string(engine.cacheCalls[i]) << std::endl;
            }
        } else if (movestr == "resetcache") {
            engine.resetMemoizations();
            std::cout << "Removed all cache entries" << std::endl;
        } else if (movestr == "legals") {
            std::cout << "Position of the piece to check: ";
            std::string posstr;
            std::cin >> posstr;
            std::cout << "Legal moves for " << posstr << ":" << std::endl;
            board.forEachMove(posstr, [](BoardMove move) {
                std::cout << std::string(move) << std::endl;
            });
        } else {
            BoardMove move = "a1a1";
            if (--goRemaining >= 0 || movestr == "go" || movestr == "g") {                                         // If the AI should play...
                std::cout << "As a perfect AI, I choose... " << std::flush;
                ChessEngine::CalculatedMove res = engine.findBestMove();   // ...ask the AI for the move
                move = res.move;
                std::cout << std::string(move) << "!" << std::endl;
                float score = res.score * (board.curColor == BoardSquares::Colors::WHITE ? 1 : -1);
                std::cout << "Score: " << std::fixed << std::setprecision(3) << score << " (depth " << std::to_string(res.depth) << ", " << std::to_string(res.movesAnalyzed) << " positions analyzed)" << std::endl;
            } else {                                                                    // else, convert the string the
                move = movestr;                                                         // user entered into a move
            }

            // Get DetailedMove object; see board/boardmoves.h for explanation
            if (board.isLegal(move)) {
                board.move(move);
            } else {
                std::cout << "Move not legal!" << std::endl;
            }
        }

    }


    return 0;
}





std::string waitforcmd(std::string str) {
    return waitforcmd((std::vector<std::string>) {str});
}

std::string waitforcmd(std::vector<std::string> v) {
    while (true) {
        std::vector<std::string> launchcmd = readcmd();
        if (std::find(v.begin(), v.end(), launchcmd[0]) != v.end()) {
            return launchcmd[0];
        }
    }
}

std::vector<std::string> readcmd() {
    std::vector<std::string> res;
    while (res.empty()) {
        std::string str;
        getline(std::cin, str);
        res = nonEmptySplits(str);        
    }
    return res;
}

std::vector<std::string> nonEmptySplits(std::string str) {
    std::vector<std::string> res;
    size_t pos = 0;
    std::pair<size_t, size_t> npos;

    while ((npos = findany(str, WHITESPACES, pos)).first < str.length()) {
        std::string s = str.substr(pos, npos.first - pos);
        if (s.length() > 0) {
            res.push_back(s);
        }
        pos = npos.second;
    }

    if (pos < str.length()) {
        res.push_back(str.substr(pos));
    }

    return res;
}

std::pair<size_t, size_t> findany(std::string s, std::vector<std::string> find, size_t start) {
    size_t min = s.length();
    size_t npos = min;
    for (std::string f : find) {
        size_t m = s.find(f, start);
        size_t n = m + f.length();
        if (m < min || (m == min && n > npos)) {
            min = m;
            npos = n;
        }
    }
    return std::pair<size_t, size_t>(min, npos);
}
