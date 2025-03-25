#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include "Player.h"
#include "Strategy.h"
#include "SmartStrategy.h"  // So we can use dynamic_cast
#include <memory>
#include <vector>

class ComputerPlayer : public Player {
private:
    std::unique_ptr<Strategy> strategy;
    std::vector<std::pair<Move, Move>> history;

public:
    ComputerPlayer(std::unique_ptr<Strategy> strat) : strategy(std::move(strat)) {}
    
    Move makeMove() override {
        return strategy->makeMove(history);
    }
    
    void recordResult(Move playerMove, Move computerMove) override {
        history.emplace_back(playerMove, computerMove);
        strategy->updateFrequencies(history);
    }
    
    void saveState() {
        strategy->saveState();
    }
    
    std::string getStrategyName() const {
        return strategy->getName();
    }

    // NEW: Return the underlying strategy pointer.
    Strategy* getStrategy() const {
        return strategy.get();
    }

    // NEW: Return the last predicted human move if the strategy is Smart.
    Move getLastPredictedHumanMove() const {
        auto *smart = dynamic_cast<SmartStrategy*>(strategy.get());
        if (smart) {
            return smart->getLastPredictedHumanMove();
        }
        // For RandomStrategy or others, no prediction is available.
        return Move::ROCK; // or you might return a sentinel value if defined
    }
};

#endif
