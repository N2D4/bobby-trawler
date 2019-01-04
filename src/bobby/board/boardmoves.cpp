#include "boardmoves.h"

// TODO Do we constexpr/inline this?
bool BoardMove::isPossibleFor(BoardSquare square, bool isCapture) const {
    int cdif = this->to.column - this->from.column;
    int rdif = this->to.row - this->from.row;
    int cadif = std::abs(cdif);
    int radif = std::abs(rdif);

    switch (square.typeId()) {
        case 5: {   // Pawn
            int rdir = square.color() == BoardSquares::Colors::WHITE ? 1 : -1;
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
