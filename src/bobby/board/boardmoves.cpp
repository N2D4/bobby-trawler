#include "boardmoves.h"

BoardMove::BoardMove(BoardPosition from, BoardPosition to, BoardSquare::Type promoteTo) : from(from), to(to), promoteTo(promoteTo) { }
BoardMove::BoardMove(BoardPosition from, BoardPosition to) : BoardMove(from, to, BoardSquare::Type::QUEEN) { }


bool BoardMove::operator==(const BoardMove& w) {
    return this->from == w.from && this->to == w.to && this->promoteTo == w.promoteTo;
}
bool BoardMove::operator!=(const BoardMove& w) {
    return !(*this == w);
}



bool BoardMove::isPossibleFor(BoardSquare square, bool isCapture) {
    int cdif = this->to.column - this->from.column;
    int rdif = this->to.row - this->from.row;
    int cadif = std::abs(cdif);
    int radif = std::abs(rdif);

    switch (square.typeId()) {
        case 5: {   // Pawn
            int rdir = square.color() == BoardSquare::Color::WHITE ? 1 : -1;
            if (isCapture) {
                return rdir == rdif && cadif == 1;
            } else {
                int fromr = this->from.row;
                return cadif == 0 && (rdir == rdif || (rdif == 2 * rdir && (fromr == 1 || fromr == 6)));
            }
        }
        case 4:    // Knight
            return cadif > 0 && radif > 0 && cadif + radif == 3;
        case 3:    // Bishop
            return cadif == radif;
        case 2:    // Rook
            return cadif == 0 || radif == 0;
        case 1:    // Queen
            return cadif == radif || cadif == 0 || radif == 0;
        case 0:    // King
            return cadif <= 1 && radif <= 1;
        default:
            return false;
    }
}



static BoardSquare::Type bmstrtotype(std::string str) {
    if (str.size() >= 5) {
        std::cout << str[4] << std::endl;
        switch (str[4]) {
            case 'Q': return BoardSquare::Type::QUEEN;
            case 'R': return BoardSquare::Type::ROOK;
            case 'B': return BoardSquare::Type::BISHOP;
            case 'N': return BoardSquare::Type::KNIGHT;
        }
    }
    return BoardSquare::Type::QUEEN;
}

// Casting from/to std::string
BoardMove::BoardMove(const std::string& str) : BoardMove(str.substr(0, 2), str.substr(2, 2), bmstrtotype(str)) { }
BoardMove& BoardMove::operator= (const std::string& str) { return *this = BoardMove(str); }
BoardMove::operator std::string() {
    return std::string(this->from) + std::string(this->to);
}

// Casting from char*
BoardMove::BoardMove(const char* str) : BoardMove(std::string(str)) { }
BoardMove& BoardMove::operator= (const char* str) { return *this = std::string(str); }





DetailedMove::DetailedMove(BoardPosition from, BoardPosition to, BoardSquare::Type promoteTo, BoardSquare captured, MoveType type, int prevFlags) : BoardMove(from, to, promoteTo), captured(captured), type(type), prevFlags(prevFlags) { }

bool DetailedMove::isPossibleFor(BoardSquare square) {
    return BoardMove::isPossibleFor(square, this->captured != BoardSquare::EMPTY);
}

bool DetailedMove::isCastling() {
    return this->type >= DetailedMove::MoveType::CASTLING_KINGSIDE;
}
