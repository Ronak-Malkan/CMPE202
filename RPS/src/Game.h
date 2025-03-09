#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "ComputerPlayer.h"
#include <memory>
#include <iostream>
#include <iomanip>

class Game {
private:
    std::unique_ptr<Player> humanPlayer;
    std::unique_ptr<ComputerPlayer> computerPlayer;
    int humanScore;
    int computerScore;
    int ties;
    const int rounds;

public:
    Game(std::unique_ptr<Player> human, std::unique_ptr<ComputerPlayer> computer, int numRounds = 20)
        : humanPlayer(std::move(human)), 
          computerPlayer(std::move(computer)), 
          humanScore(0), 
          computerScore(0), 
          ties(0),
          rounds(numRounds) {}
    
    void play() {
        std::cout << "Starting a new game with " << rounds << " rounds." << std::endl;
        std::cout << "Computer is using " << computerPlayer->getStrategyName() << " strategy." << std::endl;
        std::cout << "------------------------------" << std::endl;
        
        for (int round = 1; round <= rounds; ++round) {
            std::cout << "\nRound " << round << " of " << rounds << std::endl;
            
            // Get moves from players
            Move humanMove = humanPlayer->makeMove();
            Move computerMove = computerPlayer->makeMove();
            
            // Display moves
            std::cout << "You chose: " << moveToString(humanMove) << std::endl;
            std::cout << "Computer chose: " << moveToString(computerMove) << std::endl;
            
            // Determine winner
            int result = determineWinner(humanMove, computerMove);
            
            // Update scores and display result
            if (result > 0) {
                humanScore++;
                std::cout << "You win this round!" << std::endl;
            } else if (result < 0) {
                computerScore++;
                std::cout << "Computer wins this round!" << std::endl;
            } else {
                ties++;
                std::cout << "It's a tie!" << std::endl;
            }
            
            // Display current score
            std::cout << "Current score - You: " << humanScore 
                      << ", Computer: " << computerScore 
                      << ", Ties: " << ties << std::endl;
            
            // Record the result for both players
            humanPlayer->recordResult(humanMove, computerMove);
            computerPlayer->recordResult(humanMove, computerMove);
        }
        
        // Display final results
        std::cout << "\n------------------------------" << std::endl;
        std::cout << "Game Over! " << std::endl;    
        std::cout << "   Human wins: " << std::setw(5) << humanScore << "  " 
                  << std::setw(3) << (rounds > 0 ? (humanScore * 100 / rounds) : 0) << "%" << std::endl;
        std::cout << "Computer wins: " << std::setw(5) << computerScore << "  " 
                  << std::setw(3) << (rounds > 0 ? (computerScore * 100 / rounds) : 0) << "%" << std::endl;
        std::cout << "         Ties: " << std::setw(5) << ties << "  " 
                  << std::setw(3) << (rounds > 0 ? (ties * 100 / rounds) : 0) << "%" << std::endl;
        
        if (humanScore > computerScore) {
            std::cout << "You win!" << std::endl;
        } else if (computerScore > humanScore) {
            std::cout << "Computer wins!" << std::endl;
        } else {
            std::cout << "It's a tie!" << std::endl;
        }
        
        // Save the computer's strategy state
        computerPlayer->saveState();
    }
};

#endif
