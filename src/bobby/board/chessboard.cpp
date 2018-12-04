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


DetailedMove ChessBoard::createDetailedMove(BoardMove move) {
    DetailedMove::MoveType type = DetailedMove::MoveType::NORMAL;
    BoardSquare piece = (*this)[move.from];
    BoardSquare captured = (*this)[move.to];

    if (piece.type() == BoardSquare::Type::PAWN) {
        if (std::abs(move.from.row - move.to.row) == 2) {
            type = DetailedMove::MoveType::PAWN_DOUBLE;
        } else if (move.to.row == 0 || move.to.row == 7) {
            type = DetailedMove::MoveType::PROMOTION;
        } else {
            if ((*this)[move.to] == BoardSquare::EMPTY && move.from.column != move.to.column) {
                type = DetailedMove::MoveType::ENPASSANT;
                captured = (*this)[this->moves.back().to];
            }
        }
    } else if (piece.type() == BoardSquare::Type::KING) {
        switch (move.to.column - move.from.column) {
            case 2:
                type = DetailedMove::MoveType::CASTLING_KINGSIDE;
                break;
            case -2:
                type = DetailedMove::MoveType::CASTLING_QUEENSIDE;
                break;
        }
    }
    return DetailedMove(move.from, move.to, captured, type);
}



void ChessBoard::move(BoardMove move) {
    this->move(this->createDetailedMove(move));
}
void ChessBoard::move(DetailedMove move) {
    this->moves.push_back(move);

    switch (move.type) {
        case DetailedMove::MoveType::PROMOTION:
            // TODO Promotion
            break;

        case DetailedMove::MoveType::ENPASSANT:
            this->squares[move.to.column][move.from.row] = BoardSquare::EMPTY;

        default:
            (*this)[move.to] = (*this)[move.from];
            (*this)[move.from] = BoardSquare::EMPTY;

            if (move.isCastling()) {
                // Move rook
                int r = move.from.row;
                int fc = move.type == DetailedMove::MoveType::CASTLING_KINGSIDE ? 7 : 0;
                this->squares[4][r] = this->squares[fc][r];
                this->squares[fc][r] = BoardSquare::EMPTY;
            }
    }
    this->curColor = !this->curColor;
}
void ChessBoard::revert() {
    DetailedMove move = this->moves.back();
    this->moves.pop_back();

    this->curColor = !this->curColor;
    switch (move.type) {
        case DetailedMove::MoveType::PROMOTION:
            // TODO Promotion
            break;

        case DetailedMove::MoveType::ENPASSANT:
            this->squares[move.to.column][move.from.row] = move.captured;
            (*this)[move.from] = (*this)[move.to];
            (*this)[move.to] = BoardSquare::EMPTY;
            break;
        
        case DetailedMove::MoveType::CASTLING_KINGSIDE:
        case DetailedMove::MoveType::CASTLING_QUEENSIDE: {
            // Move rook back
            int r = move.from.row;
            int fc = move.type == DetailedMove::MoveType::CASTLING_KINGSIDE ? 7 : 0;
            this->squares[fc][r] = this->squares[4][r];
            this->squares[4][r] = BoardSquare::EMPTY;
        }

        default:
            (*this)[move.from] = (*this)[move.to];
            (*this)[move.to] = move.captured;
    }
}





bool ChessBoard::isCheck() {
    return this->isCheck(this->curColor);
}

bool ChessBoard::isCheck(BoardSquare::Color color) {
    // Find king position
    BoardSquare kingSquare(color, BoardSquare::Type::KING);
    BoardPosition king(0, 0);
    for (; king.column < 8; king.column++) {
        for (; king.row < 8; king.row++) {
            if ((*this)[king] == kingSquare) {
                return this->isSquareAttacked(color, king);
            }
        }
    }
    return false; // No king for whatever reason?
}

bool ChessBoard::isSquareAttacked(BoardSquare::Color color, BoardPosition position) {
    // Go horizontally, vertically, and diagonally and find the closest piece each
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                cont: continue;
            }

            BoardPosition pos = position;
            do {
                pos.column += i;
                pos.row += j;
                if (!pos.isValid()) goto cont;
            } while ((*this)[pos].isEmpty());

            BoardSquare sq = (*this)[pos];
            if (sq.color() == color) continue;
            if (!BoardMove(pos, position).isPossibleFor(sq, true)) continue;
            return true;
        }
    }

    // Check knights in range
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (std::abs(i) + std::abs(j) != 3) continue;

            BoardPosition pos = BoardPosition(position.column + i, position.row + j);
            if (!pos.isValid()) continue;

            BoardSquare sq = (*this)[pos];
            if (sq.color() == color) continue;
            if (!BoardMove(pos, position).isPossibleFor(sq, true)) continue;

            return true;
        }
    }

    // No threats, no check!
    return false;
}






bool ChessBoard::isLegal(BoardMove bmove) {
    DetailedMove move = this->createDetailedMove(bmove);

    BoardSquare fromSq = (*this)[move.from];
    BoardSquare toSq = (*this)[move.to];

    // Simple cases
    if (fromSq.color() != this->curColor) return false;
    if (toSq.color() == this->curColor) return false;
    if (move.from == move.to) return false;

    // That piece can't move like that
    switch (move.type) {
        case DetailedMove::MoveType::CASTLING_KINGSIDE:
        case DetailedMove::MoveType::CASTLING_QUEENSIDE:
            // TODO Castling

        case DetailedMove::MoveType::ENPASSANT: {
            DetailedMove lastMove = this->moves.back();
            if (lastMove.type != DetailedMove::MoveType::PAWN_DOUBLE) return false;
            if (lastMove.from.column != move.to.column) return false;
            if (lastMove.to.row - move.to.row != move.to.row - lastMove.from.row) return false;
        }            
        
        default:
            if (!move.isPossibleFor(fromSq)) return false;
    }

    // Filter out cases with another piece in the way
    if (fromSq.type() != BoardSquare::Type::KNIGHT) {
        if (!isLineEmpty(move.from, move.to)) return false;
    }

    // Can't move into check
    this->move(move);
    bool wasCheck = this->isCheck(fromSq.color());
    this->revert();
    if (wasCheck) return false;

    // Legal move!
    return true;
}

bool ChessBoard::isLineEmpty(BoardPosition pos1, BoardPosition pos2) {
    #define sgn(x, y)  (x < y) - (y < x)  // -1 if x < y, 0 if x == y, +1 if x > y
    int iInc = sgn(pos1.column, pos2.column);
    int jInc = sgn(pos1.row, pos2.row);
    BoardPosition pos = pos1;
    while (true) {
        pos.column += iInc;
        pos.row += jInc;
        if (sgn(pos.column, pos2.column) != iInc || sgn(pos.row, pos2.row) != jInc) return true;
        if (!(*this)[pos].isEmpty()) return false;
    }
    #undef sgn
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
