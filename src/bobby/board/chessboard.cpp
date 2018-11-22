#include "chessboard.h"

ChessBoard::ChessBoard() : curColor(BoardSquare::Color::WHITE) {
    BoardSquare square = BoardSquare::EMPTY;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
                this->squares[i][j] = BoardSquare::EMPTY;
        }
    }
    for (int i = 0; i < 8; i++) {
        this->squares[i][1] = BoardSquare::WHITE_PAWN;
        this->squares[i][6] = BoardSquare::BLACK_PAWN;
    }
    (*this)["a1"] = (*this)["h1"] = BoardSquare::WHITE_ROOK;
    (*this)["a8"] = (*this)["h8"] = BoardSquare::BLACK_ROOK;
    (*this)["b1"] = (*this)["g1"] = BoardSquare::WHITE_KNIGHT;
    (*this)["b8"] = (*this)["g8"] = BoardSquare::BLACK_KNIGHT;
    (*this)["c1"] = (*this)["f1"] = BoardSquare::WHITE_BISHOP;
    (*this)["c8"] = (*this)["f8"] = BoardSquare::BLACK_BISHOP;
    (*this)["d1"] = BoardSquare::WHITE_QUEEN;
    (*this)["d8"] = BoardSquare::BLACK_QUEEN;
    (*this)["e1"] = BoardSquare::WHITE_KING;
    (*this)["e8"] = BoardSquare::BLACK_KING;
}

BoardSquare& ChessBoard::operator[](BoardPosition position) {
    return this->squares[position.column][position.row];
}



void ChessBoard::move(DetailedMove move) {
    (*this)[move.to] = (*this)[move.from];
    (*this)[move.from] = BoardSquare::EMPTY;
    this->curColor = !this->curColor;
}
void ChessBoard::revert(DetailedMove move) {
    (*this)[move.from] = (*this)[move.to];
    (*this)[move.to] = move.captured;
    this->curColor = !this->curColor;
}





bool ChessBoard::isCheck() {
    return this->isCheck(this->curColor);
}

bool ChessBoard::isCheck(BoardSquare::Color color) {
    // Find king position
    BoardSquare kingSquare(color, BoardSquare::Type::KING);
    BoardPosition king(0, 0);
    for (king.column = 0; king.column < 8; king.column++) {
        for (king.row = 0; king.row < 8; king.row++) {
            if ((*this)[king] == kingSquare) {
                goto outofloop;
            }
        }
    }
    outofloop:

    // Go horizontally, vertically, and diagonally and find the closest piece each
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                cont: continue;
            }

            BoardPosition pos = king;
            do {
                pos.column += i;
                pos.row += j;
                if (!pos.isValid()) goto cont;
            } while ((*this)[pos].isEmpty());

            BoardSquare sq = (*this)[pos];
            if (sq.color() == kingSquare.color()) continue;
            if (!BoardMove(pos, king).isPossibleFor(sq, true)) continue;
            return true;
        }
    }

    // Check knights in range
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (std::abs(i) + std::abs(j) != 3) continue;
            BoardPosition pos = BoardPosition(king.column + i, king.row + j);
            if (!pos.isValid()) continue;
            BoardSquare sq = (*this)[pos];
            if (sq.color() == kingSquare.color()) continue;
            if (!BoardMove(pos, king).isPossibleFor(sq, true)) continue;
            return true;
        }
    }

    // No threats, no check!
    return false;
}



bool ChessBoard::isLegal(DetailedMove move) {
    BoardSquare fromSq = (*this)[move.from];
    BoardSquare toSq = (*this)[move.to];

    // Simple cases
    if (fromSq.color() != this->curColor) return false;
    if (toSq.color() == this->curColor) return false;
    if (move.from == move.to) return false;

    // That piece can't move like that
    bool isCapture = toSq.color() != BoardSquare::Color::EMPTY;
    if (!move.isPossibleFor(fromSq)) return false;

    // Filter out cases with another piece in the way
    if (fromSq.type() != BoardSquare::Type::KNIGHT) {
        #define sgn(x, y)  (x < y) - (y < x)  // -1 if x < y, 0 if x == y, +1 if x > y
        int iInc = sgn(move.from.column, move.to.column);
        int jInc = sgn(move.from.row, move.to.row);
        BoardPosition pos = move.from;
        while (true) {
            pos.column += iInc;
            pos.row += jInc;
            if (sgn(pos.column, move.to.column) != iInc || sgn(pos.row, move.to.row) != jInc) break;
            if (!(*this)[pos].isEmpty()) return false;
        }
        #undef sgn
    }

    // Can't move into check
    this->move(move);
    bool wasCheck = this->isCheck(fromSq.color());
    this->revert(move);
    if (wasCheck) return false;

    // Legal move!
    return true;
}





std::string ChessBoard::toHumanReadable(bool ansi) {
    std::string result;
    for (int i = 7; i >= 0; i--) {
        result += ('1' + i);
        result += " ";
        for (int j = 0; j < 8; j++) {
            BoardSquare sq = this->squares[j][i];
            if (ansi) result += (i + j) % 2 == 0 ? "\033[48;5;249m" : "\033[48;5;253m";
            result += " " + sq.asUnicode() + " ";
            if (ansi) result += "\033[0m";
        }
        result += "\n";
    }
    result += "  ";
    for (int j = 0; j < 8; j++) {
        result += " ";
        result += ('a' + j);
        result += " ";
    }
    return result;
}