#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include "Player.h"
#include "Strategy.h"
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
        // Add the moves to the history
        history.emplace_back(playerMove, computerMove);
        
        // Update the frequencies in the strategy
        strategy->updateFrequencies(history);
    }
    
    void saveState() {
        strategy->saveState();
    }
    
    std::string getStrategyName() const {
        return strategy->getName();
    }
};

#endif
