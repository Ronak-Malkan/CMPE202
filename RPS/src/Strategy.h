#ifndef STRATEGY_H
#define STRATEGY_H

#include "Move.h"
#include <vector>
#include <string>

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual Move makeMove(const std::vector<std::pair<Move, Move>>& history) = 0;
    virtual void updateFrequencies(const std::vector<std::pair<Move, Move>>& history) = 0;
    virtual void saveState() = 0;
    virtual void loadState() = 0;
    virtual std::string getName() const = 0;
};

#endif
