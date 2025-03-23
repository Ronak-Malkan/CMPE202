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
            // Call saveState() on the computer player's strategy.
            game->getComputerPlayer()->saveState();
        }
        lastRoundResult = "Game over!";
        return;
    }

    currentRound++;

    // Assuming you have added getters in Game to access players.
    // If not, you may need to refactor your Game class.
    auto* cPlayer = game->getComputerPlayer();  // Ensure you add such a getter in Game.h
    auto* hPlayer = game->getHumanPlayer();       // Ensure you add such a getter in Game.h

    // Get computer move
    Move computerMove = cPlayer->makeMove();
    lastComputerMove = computerMove;

    // Determine winner using your existing function determineWinner()
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

    // Record the round's moves
    hPlayer->recordResult(humanMove, computerMove);
    cPlayer->recordResult(humanMove, computerMove);
}

int RPSGameManager::getCurrentRound() const { return currentRound; }
Move RPSGameManager::getLastComputerMove() const { return lastComputerMove; }
std::string RPSGameManager::getRoundResult() const { return lastRoundResult; }
int RPSGameManager::getHumanScore() const { return humanScore; }
int RPSGameManager::getComputerScore() const { return computerScore; }
int RPSGameManager::getTies() const { return tieCount; }
