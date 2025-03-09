#ifndef PLAYER_H
#define PLAYER_H

#include "Move.h"

class Player {
public:
    virtual ~Player() = default;
    virtual Move makeMove() = 0;
    virtual void recordResult(Move playerMove, Move opponentMove) = 0;
};

#endif
