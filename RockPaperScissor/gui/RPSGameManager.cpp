#include "RPSGameManager.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "RandomStrategy.h"
#include "SmartStrategy.h"
#include "Game.h"
#include "Move.h"
#include <iostream>

RPSGameManager::RPSGameManager()
    : chosenStrategy(0), totalRounds(5), currentRound(0),
      humanScore(0), computerScore(0), tieCount(0), lastComputerMove(Move::ROCK)
{
}

void RPSGameManager::setStrategy(int index)
{
    chosenStrategy = index;
}

void RPSGameManager::setRounds(int r)
{
    totalRounds = r;
}

void RPSGameManager::startNewGame()
{
    currentRound = 0;
    humanScore = 0;
    computerScore = 0;
    tieCount = 0;
    lastRoundResult.clear();

    // Create new players.
    humanPlayer = std::make_unique<HumanPlayer>();
    if (chosenStrategy == 0)
        computerPlayer = std::make_unique<ComputerPlayer>(std::make_unique<RandomStrategy>());
    else
        computerPlayer = std::make_unique<ComputerPlayer>(std::make_unique<SmartStrategy>());

    // Create a new Game instance with the specified rounds.
    game = std::make_unique<Game>(
        std::move(humanPlayer),
        std::move(computerPlayer),
        totalRounds
    );
}

void RPSGameManager::playRound(Move humanMove)
{
    if (!game || currentRound >= totalRounds)
    {
        // Before indicating game over, save the strategy state.
        if (game) {
            game->getComputerPlayer()->saveState();
        }
        lastRoundResult = "Game over!";
        return;
    }

    currentRound++;

    // Retrieve players using getters from Game (ensure these getters exist in Game.h)
    auto* cPlayer = game->getComputerPlayer();
    auto* hPlayer = game->getHumanPlayer();

    // Get computer move
    Move computerMove = cPlayer->makeMove();
    lastComputerMove = computerMove;

    // NEW: Retrieve the predicted human move from the computer player's strategy.
    // This works only if the strategy is Smart.
    // (If not, the getter will return a default value.)
    // We do not store it separately here; we'll fetch it via getLastPredictedHumanMove().
    
    int result = determineWinner(humanMove, computerMove);
    if (result > 0)
    {
        humanScore++;
        lastRoundResult = "You win this round!";
    }
    else if (result < 0)
    {
        computerScore++;
        lastRoundResult = "Computer wins this round!";
    }
    else
    {
        tieCount++;
        lastRoundResult = "It's a tie!";
    }

    hPlayer->recordResult(humanMove, computerMove);
    cPlayer->recordResult(humanMove, computerMove);
}

int RPSGameManager::getCurrentRound() const { return currentRound; }
Move RPSGameManager::getLastComputerMove() const { return lastComputerMove; }
std::string RPSGameManager::getRoundResult() const { return lastRoundResult; }
int RPSGameManager::getHumanScore() const { return humanScore; }
int RPSGameManager::getComputerScore() const { return computerScore; }
int RPSGameManager::getTies() const { return tieCount; }

std::string RPSGameManager::getStrategyName() const {
    if (game && game->getComputerPlayer())
        return game->getComputerPlayer()->getStrategyName();
    return "";
}

Move RPSGameManager::getLastPredictedHumanMove() const {
    if (game && game->getComputerPlayer())
        return game->getComputerPlayer()->getLastPredictedHumanMove();
    return Move::ROCK;
}

bool RPSGameManager::isPredictionValid() const {
    // If the current strategy is Smart, check its prediction validity.
    if (game && game->getComputerPlayer()) {
        auto *smart = dynamic_cast<SmartStrategy*>(game->getComputerPlayer()->getStrategy());
        if (smart)
            return smart->isPredictionValid();
    }
    return false;
}
