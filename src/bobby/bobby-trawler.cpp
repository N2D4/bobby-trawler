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

    std::cout << "Alright! Now enter a 4-character move, eg. 'e2e4', 'back' to take a move back, or 'go' to ask the engine" << std::endl;




    // Ask for moves, or let the AI play
    ChessBoard board = ChessBoard();
    bool goForever = false;   // true if the AI should play automatically instead of asking the user
    while (true) {
        // Print out board and message if check
        std::cout << board.toHumanReadable(true) << std::endl;
        if (board.isCheck()) std::cout << "Check!" << std::endl;

        // Read in what the user entered, if the AI isn't set to play automatically
        std::string movestr;
        if (!goForever) {
            std::cout << "Move: ";
            std::cin >> movestr;
            // If the user enters 'go4evah', make the AI start playing automatically
            if (movestr == "go4evah") goForever = true;
        }

        if (movestr == "back") {
            board.revert();
        } else if (movestr == "rem") {
            std::cout << "Removing a piece. Position: ";
            std::string remstr;
            std::cin >> remstr;
            board[remstr] = BoardSquare::EMPTY;
            std::cout << "Removed piece. Note that this might cause glitches, eg. w/ castling if you remove a rook" << std::endl;
        } else if (movestr == "undo") {
            board.revert();
            std::cout << "Reverted a move" << std::endl;
        } else {
            BoardMove move = "a1a1";
            if (goForever || movestr == "go") {                                         // If the AI should play...
                std::cout << "As a perfect AI, I choose... " << std::flush;
                std::pair<float, BoardMove> res = ChessEngine(board).findBestMove(4);   // ...ask the AI for the move
                move = res.second;
                std::cout << std::string(move) << "!" << std::endl;
                std::cout << "Score: " << (res.first * (board.curColor == BoardSquare::Color::WHITE ? 1 : -1)) << std::endl;
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