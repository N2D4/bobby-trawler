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

    std::cout << "Alright! Now enter a 4-character move, eg. 'e2e4', or 'go' to ask the engine" << std::endl;

    ChessBoard board = ChessBoard();
    bool goForever = false;
    while (true) {
        std::cout << board.toHumanReadable(true) << std::endl;
        if (board.isCheck()) std::cout << "Check!" << std::endl;

        std::string movestr;
        if (!goForever) {
            std::cout << "Move: ";
            std::cin >> movestr;
            if (movestr == "go4evah") goForever = true;
        }

        BoardMove move = "a1a1";
        if (goForever || movestr == "go") {
            std::cout << "As a perfect AI, I choose... ";
            move = ChessEngine(board).findBestMove();
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            std::cout << std::string(move) << "!" << std::endl;
        } else {
            move = movestr;
        }

        DetailedMove detailed = move.detailed(board);
        if (board.isLegal(detailed)) {
            board.move(detailed);
        } else {
            std::cout << "Move not legal!" << std::endl;
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