#include "chessboard.h"

ChessBoard::ChessBoard() : curColor(BoardSquares::Colors::WHITE), allowCastlingKingside(true, true), allowCastlingQueenside(true, true), kingPos("e1", "e8") {
    BoardSquare square = BoardSquares::EMPTY;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
                this->squares[i][j] = BoardSquares::EMPTY;
        }
    }
    for (int i = 0; i < 8; i++) {
        this->squares[i][1] = BoardSquares::WHITE_PAWN;
        this->squares[i][6] = BoardSquares::BLACK_PAWN;
    }
    (*this)["a1"] = (*this)["h1"] = BoardSquares::WHITE_ROOK;
    (*this)["a8"] = (*this)["h8"] = BoardSquares::BLACK_ROOK;
    (*this)["b1"] = (*this)["g1"] = BoardSquares::WHITE_KNIGHT;
    (*this)["b8"] = (*this)["g8"] = BoardSquares::BLACK_KNIGHT;
    (*this)["c1"] = (*this)["f1"] = BoardSquares::WHITE_BISHOP;
    (*this)["c8"] = (*this)["f8"] = BoardSquares::BLACK_BISHOP;
    (*this)["d1"] = BoardSquares::WHITE_QUEEN;
    (*this)["d8"] = BoardSquares::BLACK_QUEEN;
    (*this)["e1"] = BoardSquares::WHITE_KING;
    (*this)["e8"] = BoardSquares::BLACK_KING;

    this->materialScore = 0;
    this->pieceCount = 32;
}


DetailedMove ChessBoard::createDetailedMove(BoardMove move) const {
    DetailedMove::MoveType type = DetailedMove::MoveType::NORMAL;

    BoardSquare piece = (*this)[move.from];
    BoardSquare captured = (*this)[move.to];

    if (piece.type() == BoardSquares::Types::PAWN) {
        if (std::abs(move.from.row - move.to.row) == 2) {
            type = DetailedMove::MoveType::PAWN_DOUBLE;
        } else if (move.to.row == 0 || move.to.row == 7) {
            type = DetailedMove::MoveType::PROMOTION;
        } else {
            if (move.from.column != move.to.column && (*this)[move.to] == BoardSquares::EMPTY && this->moves.size() > 0) {
                type = DetailedMove::MoveType::ENPASSANT;
                captured = (*this)[this->moves.back().to];
            }
        }
    } else if (piece.type() == BoardSquares::Types::KING) {
        switch (move.to.column - move.from.column) {
            case 2:
                type = DetailedMove::MoveType::CASTLING_KINGSIDE;
                break;
            case -2:
                type = DetailedMove::MoveType::CASTLING_QUEENSIDE;
                break;
        }
    }
    int flags = 0;
    if (allowCastlingKingside.white) flags |= 0x1;
    if (allowCastlingQueenside.white) flags |= 0x2;
    if (allowCastlingKingside.black) flags |= 0x4;
    if (allowCastlingQueenside.black) flags |= 0x8;
    return DetailedMove(move.from, move.to, move.promoteTo, captured, type, flags);
}




void ChessBoard::moveDetailed(const DetailedMove& move) {
    BoardSquare piece = (*this)[move.from];
    float materialScoreIncrease = move.captured.type().getScore();
    if (move.captured.type() == BoardSquares::Types::PAWN) {
        materialScoreIncrease += (move.type == DetailedMove::MoveType::ENPASSANT ? BoardPosition(move.to.column, move.from.row) : move.to).getPawnBonusScore(!this->curColor);
    }

    this->moves.push_back(move);

    switch (move.type) {
        case DetailedMove::MoveType::PROMOTION:
            (*this)[move.to] = BoardSquare(this->curColor, move.promoteTo);
            (*this)[move.from] = BoardSquares::EMPTY;
            materialScoreIncrease += move.promoteTo.getScore() - BoardSquares::Types::PAWN.getScore();
            break;

        case DetailedMove::MoveType::ENPASSANT:
            this->squares[move.to.column][move.from.row] = BoardSquares::EMPTY;

        default:
            (*this)[move.to] = piece;
            (*this)[move.from] = BoardSquares::EMPTY;

            if (move.isCastling()) {
                // Move rook
                int r = move.from.row;
                int fc = move.type == DetailedMove::MoveType::CASTLING_KINGSIDE ? 7 : 0;
                this->squares[(move.from.column + move.to.column)/2][r] = this->squares[fc][r];
                this->squares[fc][r] = BoardSquares::EMPTY;
            }
    }

    // TODO Use switch instead
    if (piece.type() == BoardSquares::Types::KING) {
        this->allowCastlingKingside[this->curColor] = false;
        this->allowCastlingQueenside[this->curColor] = false;
        this->kingPos[this->curColor] = move.to;
    } else if (piece.type() == BoardSquares::Types::ROOK) {
        int r = this->curColor == BoardSquares::Colors::WHITE ? 0 : 7;
        if (move.from.row == r) {
            if (move.from.column == 0) this->allowCastlingQueenside[this->curColor] = false;
            if (move.from.column == 7) this->allowCastlingKingside[this->curColor] = false;
        }
    } else if (piece.type() == BoardSquares::Types::PAWN) {
        materialScoreIncrease -= move.from.getPawnBonusScore(this->curColor);
        if (move.type != DetailedMove::MoveType::PROMOTION) materialScoreIncrease += move.to.getPawnBonusScore(this->curColor);
    }

    this->materialScore += this->curColor == BoardSquares::Colors::WHITE ? materialScoreIncrease : -materialScoreIncrease;
    if (!move.captured.isEmpty()) this->pieceCount--;
    this->curColor = !this->curColor;
}
void ChessBoard::revert() {
    DetailedMove move = this->moves.back();
    this->moves.pop_back();
    
    this->curColor = !this->curColor;

    float materialScoreDecrease = move.captured.type().getScore();
    if (move.captured.type() == BoardSquares::Types::PAWN) {
        materialScoreDecrease += (move.type == DetailedMove::MoveType::ENPASSANT ? BoardPosition(move.to.column, move.from.row) : move.to).getPawnBonusScore(!this->curColor);
    }

    this->allowCastlingKingside.white = move.prevFlags & 0x1;
    this->allowCastlingQueenside.white = move.prevFlags & 0x2;
    this->allowCastlingKingside.black = move.prevFlags & 0x4;
    this->allowCastlingQueenside.black = move.prevFlags & 0x8;

    BoardSquare piece = (*this)[move.to];
    if (piece.type() == BoardSquares::Types::KING) {
        this->kingPos[this->curColor] = move.from;
    } else if (piece.type() == BoardSquares::Types::PAWN ||move.type == DetailedMove::MoveType::PROMOTION) {
        materialScoreDecrease -= move.from.getPawnBonusScore(this->curColor);
        if (move.type != DetailedMove::MoveType::PROMOTION) materialScoreDecrease += move.to.getPawnBonusScore(this->curColor);
    }

    switch (move.type) {
        case DetailedMove::MoveType::PROMOTION:
            (*this)[move.from] = BoardSquare(this->curColor, BoardSquares::Types::PAWN);
            (*this)[move.to] = move.captured;
            materialScoreDecrease += move.promoteTo.getScore() - BoardSquares::Types::PAWN.getScore();
            break;

        case DetailedMove::MoveType::ENPASSANT:
            this->squares[move.to.column][move.from.row] = move.captured;
            (*this)[move.from] = piece;
            (*this)[move.to] = BoardSquares::EMPTY;
            break;
        
        case DetailedMove::MoveType::CASTLING_KINGSIDE:
        case DetailedMove::MoveType::CASTLING_QUEENSIDE: {
            // Move rook back
            int r = move.from.row;
            int fc = move.type == DetailedMove::MoveType::CASTLING_KINGSIDE ? 7 : 0;
            int f = (move.from.column + move.to.column)/2;
            this->squares[fc][r] = this->squares[f][r];
            this->squares[f][r] = BoardSquares::EMPTY;
        }

        default:
            (*this)[move.from] = piece;
            (*this)[move.to] = move.captured;
    }

    this->materialScore -= this->curColor == BoardSquares::Colors::WHITE ? materialScoreDecrease : -materialScoreDecrease;
    if (!move.captured.isEmpty()) this->pieceCount++;
}




bool ChessBoard::isSquareAttacked(BoardSquare::Color by, BoardPosition position) const {
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
            if (sq.color() != by) continue;
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
            if (sq.color() != by) continue;
            if (!BoardMove(pos, position).isPossibleFor(sq, true)) continue;

            return true;
        }
    }

    // No threats, no check!
    return false;
}






bool ChessBoard::isLegal(BoardMove bmove) {
    BoardSquare fromSq = (*this)[bmove.from];
    BoardSquare toSq = (*this)[bmove.to];

    // Simple cases
    if (fromSq.color() != this->curColor) return false;
    if (toSq.color() == this->curColor) return false;
    if (bmove.from == bmove.to) return false;

    DetailedMove move = this->createDetailedMove(bmove);

    // That piece can't move like that
    switch (move.type) {
        case DetailedMove::MoveType::CASTLING_KINGSIDE:
            if (!this->allowCastlingKingside[this->curColor]) return false;
            if (!this->isLineEmpty(move.from, BoardPosition(7, move.from.row))) return false;
            for (BoardPosition pos = move.from; pos.column <= 7; pos.column++) {
                if (this->isSquareAttacked(!this->curColor, pos)) return false;
            }
            goto castling;
            
        case DetailedMove::MoveType::CASTLING_QUEENSIDE:
            if (!this->allowCastlingQueenside[this->curColor]) return false;
            if (!this->isLineEmpty(move.from, BoardPosition(0, move.from.row))) return false;
            for (BoardPosition pos = move.from; pos.column >= 0; pos.column--) {
                if (this->isSquareAttacked(!this->curColor, pos)) return false;
            }
            goto castling;
        
        castling: {
            int r = this->curColor == BoardSquares::Colors::WHITE ? 0 : 7;
            if (move.from.row != r) return false;
            if (move.to.row != r) return false;
            break;
        }
            

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
    if (fromSq.type() != BoardSquares::Types::KNIGHT) {
        if (!isLineEmpty(move.from, move.to)) return false;
    }

    // Can only promote to certain pieces
    if (move.type == DetailedMove::MoveType::PROMOTION) {
        if (move.promoteTo == BoardSquares::Types::PAWN ||
            move.promoteTo == BoardSquares::Types::KING ||
            move.promoteTo == BoardSquares::Types::EMPTY) {
            return false;
         }

    }

    // Can't move into check
    this->moveDetailed(move);
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

int ChessBoard::cacheNameHits[33] = {0};
int ChessBoard::cacheNameCalls[33] = {0};

// TODO: Add reading so we can verify and write testcases
ChessBoard::CacheName ChessBoard::getUniqueCacheName() const {
    int pieceCount = getPieceCount();

    CacheName result = _priv_getUniqueCacheName();
    if (pieceCount <= 32 && pieceCount >= 0) {
        ChessBoard::cacheNameCalls[pieceCount]++;
        if (!result.isInvalid()) ChessBoard::cacheNameHits[pieceCount]++;
    }

    return result;
}

ChessBoard::CacheName ChessBoard::_priv_getUniqueCacheName() const {
    uint64_t a = 0, b = 0, c = 0;
    uint64_t *p = &b;
    int pos = 0;
    int nonEmpty = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            BoardSquare square = squares[i][j];
            if (!square.isEmpty()) {
                a |= 1 << pos;
                if (nonEmpty >= 18) {
                    p = &c;
                    nonEmpty = 0;
                }
                *p *= 2;
                *p += square.colorId();
                *p *= 6;
                *p += square.typeId();
                nonEmpty++;
            }
            pos++;
        }
    }

    c |= (uint64_t) curColor.id() << 63;
    c |= (uint64_t) allowCastlingKingside.white << 62;
    c |= (uint64_t) allowCastlingQueenside.white << 61;
    c |= (uint64_t) allowCastlingKingside.black << 60;
    c |= (uint64_t) allowCastlingQueenside.black << 59;
    const DetailedMove& mback = moves.back();
    c |= (uint64_t) (mback.type == DetailedMove::MoveType::PAWN_DOUBLE ? 0b1000 | mback.to.column : 0) << 55;

    if (nonEmpty > 32) return CacheName();
    else return CacheName(a, b, c);
}

unsigned long long ChessBoard::_priv_getShortUniqueCacheName() const {
    unsigned long long result = 0;
    #define writeToResult(I, LEN)  {                                                \
        if (__builtin_umulll_overflow(result, LEN, &result)) return 0;        \
        if (__builtin_uaddll_overflow(result, I, &result)) return 0;          \
    }

    // In order to guarantee bijection, the length of every write MUST be determinable given all FUTURE writes (since reading goes in reverse order)


    int maxEnPassants = 0;
    int isEnPassant = 0;
    BoardSquare opawn(!curColor, BoardSquares::Types::PAWN);
    BoardSquare opppwn(curColor, BoardSquares::Types::PAWN);
    int bsqci = curColor == BoardSquares::Colors::WHITE ? 4 : 3;
    for (int j = 0; j < 8; j++) {
        if (squares[bsqci][j] == opppwn) {
            if ((j >= 1 && squares[bsqci][j-1] == opawn) || (j <= 7 && squares[bsqci][j+1] == opawn)) {
                maxEnPassants++;
                if (moves.back().to == BoardPosition(bsqci, j)) {
                    isEnPassant = maxEnPassants;
                }
            }
        }
    }
    outer:
    if (maxEnPassants >= 1) {
        writeToResult(isEnPassant, maxEnPassants + 1);
    }

    bool hasFoundKing = false;
    int sinceLastPiece = 0;
    const int maxSinceLastPiece = 10;
    writeToResult(maxSinceLastPiece + 1, maxSinceLastPiece + 2);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            BoardSquare square = squares[i][j];
            if (square.isEmpty()) {
                sinceLastPiece += 1;
            } else {
                writeToResult((sinceLastPiece % 23) % maxSinceLastPiece, maxSinceLastPiece + 2);
                if (square.type() == BoardSquares::Types::KING) {
                    if (hasFoundKing) {
                        writeToResult(square.colorId(), 2);
                    } else {
                        hasFoundKing = true;
                    }
                }
                writeToResult(square.typeId(), 6);
                while (sinceLastPiece % 23 >= maxSinceLastPiece) {
                    writeToResult(maxSinceLastPiece, maxSinceLastPiece + 2);
                    sinceLastPiece -= maxSinceLastPiece;
                }
                sinceLastPiece -= sinceLastPiece % 23;
            }
            while (sinceLastPiece >= 23) {
                writeToResult(maxSinceLastPiece + 1, maxSinceLastPiece + 2);
                sinceLastPiece -= 23;
            }
        }
    }

    writeToResult(curColor == BoardSquares::Colors::BLACK, 2);

    #undef writeToResult

    return result;
}







std::string ChessBoard::toHumanReadable(bool ansi) const {
    BoardMove lastm = BoardMove(BoardPosition(-1, -1), BoardPosition(-1, -1));
    if (this->moves.size() >= 1) lastm = this->moves.back();
    std::string result;
    for (int i = 7; i >= 0; i--) {
        result += ('1' + i);
        result += " ";
        for (int j = 0; j < 8; j++) {
            BoardPosition pos = BoardPosition(j, i);
            BoardSquare sq = (*this)[pos];
            if (ansi) {
                result += "\033[38;5;0m";
                if (lastm.from == pos || lastm.to == pos) {
                    result += (i + j) % 2 == 0 ? "\033[48;5;142m" : "\033[48;5;185m";
                } else {
                    result += (i + j) % 2 == 0 ? "\033[48;5;249m" : "\033[48;5;253m";
                }
            }
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

std::string ChessBoard::getInfo(bool ansi) const {
    std::string result;
    result += "curColor: " + std::string(curColor == BoardSquares::Colors::WHITE ? "White" : "Black") + "\n";
    result += "score (white): " + std::to_string(this->getMaterialScore()) + "\n";
    result += "pieceCount: " + std::to_string(this->getPieceCount()) + "\n";
    result += "allowCastlingKingside.white: " + std::string(allowCastlingKingside.white ? "true" : "false") + "\n";
    result += "allowCastlingQueenside.white: " + std::string(allowCastlingQueenside.white ? "true" : "false") + "\n";
    result += "allowCastlingKingside.black: " + std::string(allowCastlingKingside.black ? "true" : "false") + "\n";
    result += "allowCastlingQueenside.black: " + std::string(allowCastlingQueenside.black ? "true" : "false") + "\n";
    result += "kingPos.white: " + std::string(kingPos.white) + "\n";
    result += "kingPos.black: " + std::string(kingPos.black) + "\n";
    CacheName posName = getUniqueCacheName();
    result += "Unique position name: " + std::string(posName.isInvalid() ? "None" : std::string(posName)) + "\n";
    result += "moves: (length " + std::to_string(moves.size()) + ") ";
    for (int i = 0; i < moves.size(); i++) {
        result += std::string(moves[i]) + ", ";
    }
    result += "\n";
    result += "Cache name success rates:\n";
    for (int i = 0; i < 33; i++) {
        if (cacheNameHits[i] <= 0) continue;
        result += "  " + std::to_string(i) + ": " + std::to_string(100.0 * cacheNameHits[i] / (float) cacheNameCalls[i]) + "% of " + std::to_string(cacheNameCalls[i]) + "\n";
    }
    
    result += toHumanReadable(ansi);
    return result;
}




/*ChessBoard::MoveIterator ChessBoard::MoveIterator::end = MoveIterator();


ChessBoard::MoveIterator& ChessBoard::LineMoveIterator::operator++() {
    do {
        if (rem == 0) return MoveIterator::end;
        move.to.column += dx;
        move.to.row += dy;
        if (!board[move.to].isEmpty()) {
            if (board.isLegal(move)) return *this;
            goto skp;
        }
        if (!move.to.isValid()) {
            skp:
                nextRem();
                move.to = move.from;
                continue;
        }
    } while (!board.isLegal(move));
    return *this;
}

ChessBoard::MoveIterator ChessBoard::getMoves(const BoardPosition pos) const {
    switch ((*this)[pos].typeId()) {
        case -1:            // empty
            return MoveIterator::end;
        case 0:             // king
            class {

            } KingIterator;
            return ArrayMoveIterator();

    }
}*/

void ChessBoard::forEachMove(const BoardPosition pos, std::function<void (BoardMove)> func) {
    BoardMove move(pos, pos);
    BoardSquare square = (*this)[pos];

    #define CCHECK(i, j)                                                                            \
        move.to.column += i;                                                                        \
        move.to.row += j;                                                                           \
        if (move.to.isValid() && isLegal(move)) func(move)

    #define LCHECK(i, j)                                                                            \
        move.to = move.from;                                                                        \
        while (true) {                                                                              \
            move.to.row += i, move.to.column += j;                                                  \
            if (!move.to.isValid()) break;                                                          \
            if (isLegal(move)) func(move);                                                          \
            if (!(*this)[move.to].isEmpty()) break;                                                 \
        }

    switch (square.typeId()) {
        case 0:             // king
            CCHECK(-1, 0);
            CCHECK(-1, 0);
            CCHECK(4, 0);
            CCHECK(-1, 0);
            CCHECK(0, 1);
            CCHECK(-1, 0);
            CCHECK(-1, 0);
            CCHECK(0, -2);
            CCHECK(1, 0);
            CCHECK(1, 0);
            break;
        case 1:             // queen
            LCHECK(1, 0);
            LCHECK(-1, 0);
            LCHECK(0, 1);
            LCHECK(0, -1);
        case 3:             // bishop
            LCHECK(1, 1);
            LCHECK(1, -1);
            LCHECK(-1, 1);
            LCHECK(-1, -1);
            break;
        case 2:             // rook
            LCHECK(1, 0);
            LCHECK(-1, 0);
            LCHECK(0, 1);
            LCHECK(0, -1);
            break;
        case 4:             // knight
            CCHECK(2, 1);
            CCHECK(0, -2);
            CCHECK(-1, -1);
            CCHECK(-2, 0);
            CCHECK(-1, 1);
            CCHECK(0, 2);
            CCHECK(1, 1);
            CCHECK(2, 0);
            break;
        case 5:             // pawn
            int rn = square.color() == BoardSquares::Colors::WHITE ? 1 : -1;
            CCHECK(0, rn);
            CCHECK(1, 0);
            CCHECK(-2, 0);
            CCHECK(1, rn);
            break;

    }

    #undef LCHECK
    #undef CCHECK
}

