#ifndef RPSGAMEMANAGER_H
#define RPSGAMEMANAGER_H

#include <memory>
#include <string>
#include "Move.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "Game.h"

class RPSGameManager
{
public:
    RPSGameManager();

    void setStrategy(int index);   // 0 = Random, 1 = Smart
    void setRounds(int r);
    void startNewGame();
    void playRound(Move humanMove);

    // Getters for UI display
    int getCurrentRound() const;
    Move getLastComputerMove() const;
    std::string getRoundResult() const;
    int getHumanScore() const;
    int getComputerScore() const;
    int getTies() const;

    // NEW: Getter for the last predicted human move.
    Move getLastPredictedHumanMove() const;

    // NEW: Check if a valid prediction exists.
    bool isPredictionValid() const;

    // NEW: Expose the current strategy name.
    std::string getStrategyName() const;

private:
    int chosenStrategy;
    int totalRounds;
    int currentRound;
    int humanScore;
    int computerScore;
    int tieCount;

    Move lastComputerMove;
    std::string lastRoundResult;

    std::unique_ptr<HumanPlayer> humanPlayer;
    std::unique_ptr<ComputerPlayer> computerPlayer;
    std::unique_ptr<Game> game;
};

#endif // RPSGAMEMANAGER_H
