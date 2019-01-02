#include "chessboard.h"

ChessBoard::ChessBoard() : curColor(BoardSquare::Color::WHITE), allowCastlingKingside(true, true), allowCastlingQueenside(true, true), kingPos("e1", "e8") {
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

    this->materialScore = 0;
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
            if (move.from.column != move.to.column && (*this)[move.to] == BoardSquare::EMPTY && this->moves.size() > 0) {
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
    int flags = 0;
    if (allowCastlingKingside.white) flags |= 0x1;
    if (allowCastlingQueenside.white) flags |= 0x2;
    if (allowCastlingKingside.black) flags |= 0x4;
    if (allowCastlingQueenside.black) flags |= 0x8;
    return DetailedMove(move.from, move.to, move.promoteTo, captured, type, flags);
}



void ChessBoard::move(BoardMove move) {
    this->move(this->createDetailedMove(move));
}
void ChessBoard::move(DetailedMove move) {
    BoardSquare piece = (*this)[move.from];
    float materialScoreIncrease = move.captured.type().getScore();
    if (move.captured.type() == BoardSquare::Type::PAWN) {
        materialScoreIncrease += (move.type == DetailedMove::MoveType::ENPASSANT ? BoardPosition(move.to.column, move.from.row) : move.to).getPawnBonusScore(!this->curColor);
    }

    this->moves.push_back(move);

    switch (move.type) {
        case DetailedMove::MoveType::PROMOTION:
            (*this)[move.to] = BoardSquare(this->curColor, move.promoteTo);
            (*this)[move.from] = BoardSquare::EMPTY;
            materialScoreIncrease += move.promoteTo.getScore() - BoardSquare::Type::PAWN.getScore();
            break;

        case DetailedMove::MoveType::ENPASSANT:
            this->squares[move.to.column][move.from.row] = BoardSquare::EMPTY;

        default:
            (*this)[move.to] = piece;
            (*this)[move.from] = BoardSquare::EMPTY;

            if (move.isCastling()) {
                // Move rook
                int r = move.from.row;
                int fc = move.type == DetailedMove::MoveType::CASTLING_KINGSIDE ? 7 : 0;
                this->squares[(move.from.column + move.to.column)/2][r] = this->squares[fc][r];
                this->squares[fc][r] = BoardSquare::EMPTY;
            }
    }

    // TODO Use switch instead
    if (piece.type() == BoardSquare::Type::KING) {
        this->allowCastlingKingside[this->curColor] = false;
        this->allowCastlingQueenside[this->curColor] = false;
        this->kingPos[this->curColor] = move.to;
    } else if (piece.type() == BoardSquare::Type::ROOK) {
        int r = this->curColor == BoardSquare::Color::WHITE ? 0 : 7;
        if (move.from.row == r) {
            if (move.from.column == 0) this->allowCastlingQueenside[this->curColor] = false;
            if (move.from.column == 7) this->allowCastlingKingside[this->curColor] = false;
        }
    } else if (piece.type() == BoardSquare::Type::PAWN) {
        materialScoreIncrease -= move.from.getPawnBonusScore(this->curColor);
        if (move.type != DetailedMove::MoveType::PROMOTION) materialScoreIncrease += move.to.getPawnBonusScore(this->curColor);
    }

    this->materialScore += this->curColor == BoardSquare::Color::WHITE ? materialScoreIncrease : -materialScoreIncrease;

    this->curColor = !this->curColor;
}
void ChessBoard::revert() {
    DetailedMove move = this->moves.back();
    this->moves.pop_back();

    this->curColor = !this->curColor;

    float materialScoreDecrease = move.captured.type().getScore();
    if (move.captured.type() == BoardSquare::Type::PAWN) {
        materialScoreDecrease += (move.type == DetailedMove::MoveType::ENPASSANT ? BoardPosition(move.to.column, move.from.row) : move.to).getPawnBonusScore(!this->curColor);
    }

    this->allowCastlingKingside.white = move.prevFlags & 0x1;
    this->allowCastlingQueenside.white = move.prevFlags & 0x2;
    this->allowCastlingKingside.black = move.prevFlags & 0x4;
    this->allowCastlingQueenside.black = move.prevFlags & 0x8;

    BoardSquare piece = (*this)[move.to];
    if (piece.type() == BoardSquare::Type::KING) {
        this->kingPos[this->curColor] = move.from;
    } else if (piece.type() == BoardSquare::Type::PAWN ||move.type == DetailedMove::MoveType::PROMOTION) {
        materialScoreDecrease -= move.from.getPawnBonusScore(this->curColor);
        if (move.type != DetailedMove::MoveType::PROMOTION) materialScoreDecrease += move.to.getPawnBonusScore(this->curColor);
    }

    switch (move.type) {
        case DetailedMove::MoveType::PROMOTION:
            (*this)[move.from] = BoardSquare(this->curColor, BoardSquare::Type::PAWN);
            (*this)[move.to] = move.captured;
            materialScoreDecrease += move.promoteTo.getScore() - BoardSquare::Type::PAWN.getScore();
            break;

        case DetailedMove::MoveType::ENPASSANT:
            this->squares[move.to.column][move.from.row] = move.captured;
            (*this)[move.from] = piece;
            (*this)[move.to] = BoardSquare::EMPTY;
            break;
        
        case DetailedMove::MoveType::CASTLING_KINGSIDE:
        case DetailedMove::MoveType::CASTLING_QUEENSIDE: {
            // Move rook back
            int r = move.from.row;
            int fc = move.type == DetailedMove::MoveType::CASTLING_KINGSIDE ? 7 : 0;
            int f = (move.from.column + move.to.column)/2;
            this->squares[fc][r] = this->squares[f][r];
            this->squares[f][r] = BoardSquare::EMPTY;
        }

        default:
            (*this)[move.from] = piece;
            (*this)[move.to] = move.captured;
    }

    this->materialScore -= this->curColor == BoardSquare::Color::WHITE ? materialScoreDecrease : -materialScoreDecrease;
}





bool ChessBoard::isCheck() {
    return this->isCheck(this->curColor);
}

bool ChessBoard::isCheck(BoardSquare::Color color) {
    return this->isSquareAttacked(!color, this->kingPos[color]);
}

bool ChessBoard::isSquareAttacked(BoardSquare::Color by, BoardPosition position) {
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
            int r = this->curColor == BoardSquare::Color::WHITE ? 0 : 7;
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
    if (fromSq.type() != BoardSquare::Type::KNIGHT) {
        if (!isLineEmpty(move.from, move.to)) return false;
    }

    // Can only promote to certain pieces
    if (move.type == DetailedMove::MoveType::PROMOTION) {
        if (move.promoteTo == BoardSquare::Type::PAWN ||
            move.promoteTo == BoardSquare::Type::KING ||
            move.promoteTo == BoardSquare::Type::EMPTY) {
             return false;
         }

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



float ChessBoard::getMaterialScore() {
    return this->materialScore;
}

float ChessBoard::getMaterialScore(BoardSquare::Color color) {
    return color == BoardSquare::Color::WHITE ? getMaterialScore() : -getMaterialScore();
}



unsigned long long ChessBoard::getUniqueCacheName() {
    if (allowCastlingKingside.white || allowCastlingKingside.black || allowCastlingQueenside.white || allowCastlingQueenside.black) {
        return 0;
    }

    std::vector<unsigned long> ivec;
    std::vector<unsigned long> lenvec;
    #define writeToResult(I, LEN)  ivec.push_back(I); lenvec.push_back(LEN);

    writeToResult(curColor == BoardSquare::Color::BLACK, 2);

    bool hasFoundKing = false;
    int sinceLastPiece = 0;
    int maxSinceLastPiece = 10;
    int maxEnPassants = 0;
    int isEnPassant = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            BoardSquare square = squares[i][j];
            while (sinceLastPiece >= 23) {
                writeToResult(maxSinceLastPiece + 1, maxSinceLastPiece + 2);
                sinceLastPiece -= 23;
            }
            if (square.isEmpty()) {
                sinceLastPiece += 1;
                continue;
            }
            if (isEnPassant <= 0 && i == (curColor == BoardSquare::Color::WHITE ? 4 : 5)) {
                if (square.type() == BoardSquare::Type::PAWN && square.color() == !curColor) {
                    BoardSquare opawn = BoardSquare::Type::PAWN.ofColor(curColor);
                    if ((j >= 1 && squares[i][j-1] == opawn) || (j <= 7 && squares[i][j+1] == opawn)) {
                        maxEnPassants++;
                        if (moves.back().to == BoardPosition(i, j)) isEnPassant = maxEnPassants;
                    }
                }
            }
            while (sinceLastPiece >= maxSinceLastPiece) {
                writeToResult(maxSinceLastPiece, maxSinceLastPiece + 2);
                sinceLastPiece -= maxSinceLastPiece;
            }
            writeToResult(sinceLastPiece, maxSinceLastPiece + 2);
            writeToResult(square.typeId(), 6);
            if (hasFoundKing || !(hasFoundKing = square.type() == BoardSquare::Type::KING)) {
                writeToResult(square.colorId(), 2);
            }
            sinceLastPiece = 0;
            maxSinceLastPiece = std::min(10, 64 - (8*i + j) - 1);
        }
    }
    writeToResult(maxSinceLastPiece + 1, maxSinceLastPiece + 2);

    if (maxEnPassants >= 1) {
        writeToResult(isEnPassant, maxEnPassants + 1);
    }

    #undef writeToResult

    unsigned long long result = 0;
    for (int i = ivec.size() - 1; i >= 0; i--) {
        if (__builtin_umulll_overflow(result, lenvec[i], &result)) return 0;
        if (__builtin_uaddll_overflow(result, ivec[i], &result)) return 0;
    }
    return result;
}







std::string ChessBoard::toHumanReadable(bool ansi) {
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

std::string ChessBoard::getInfo(bool ansi) {
    std::string result;
    result += "curColor: " + std::string(curColor == BoardSquare::Color::WHITE ? "White" : "Black") + "\n";
    result += "score (white): " + std::to_string(this->getMaterialScore()) + "\n";
    result += "allowCastlingKingside.white: " + std::string(allowCastlingKingside.white ? "true" : "false") + "\n";
    result += "allowCastlingQueenside.white: " + std::string(allowCastlingQueenside.white ? "true" : "false") + "\n";
    result += "allowCastlingKingside.black: " + std::string(allowCastlingKingside.black ? "true" : "false") + "\n";
    result += "allowCastlingQueenside.black: " + std::string(allowCastlingQueenside.black ? "true" : "false") + "\n";
    result += "kingPos.white: " + std::string(kingPos.white) + "\n";
    result += "kingPos.black: " + std::string(kingPos.black) + "\n";
    long long posName = getUniqueCacheName();
    result += "Unique position name: " + std::string(posName == 0 ? "None" : std::to_string(posName)) + "\n";
    result += "moves:\n";
    for (int i = 0; i < moves.size(); i++) {
        result += "  " + std::string(moves[i]) + "\n";
    }
    result += toHumanReadable(ansi);
    return result;
}

