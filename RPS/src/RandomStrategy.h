#ifndef RANDOM_STRATEGY_H
#define RANDOM_STRATEGY_H

#include "Strategy.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>

class RandomStrategy : public Strategy {
private:
    std::ofstream outputFile;
    int roundNumber;
    int humanWins;
    int computerWins;
    int ties;

public:
    RandomStrategy() {
        // Seed the random number generator
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        
        // Open output file in the same directory as freq.txt (build folder)
        outputFile.open("output-random.txt");
        if (!outputFile.is_open()) {
            std::cerr << "Failed to open output-random.txt for writing." << std::endl;
        }
        
        // Initialize counters
        roundNumber = 0;
        humanWins = 0;
        computerWins = 0;
        ties = 0;
    }
    
    ~RandomStrategy() {
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }
    
    Move makeMove(const std::vector<std::pair<Move, Move>>& history) override {
        // Generate a random number between 0 and 2
        int randomNum = std::rand() % 3;
        
        switch (randomNum) {
            case 0:
                return Move::ROCK;
            case 1:
                return Move::PAPER;
            case 2:
                return Move::SCISSORS;
            default:
                return Move::ROCK; // Should never happen
        }
    }
    
    void updateFrequencies(const std::vector<std::pair<Move, Move>>& history) override {
        // No frequencies to update for random strategy
        
        // If we have at least one move in history, record the round
        if (!history.empty()) {
            // Get the last move pair
            const auto& lastMove = history.back();
            Move humanMove = lastMove.first;
            Move computerMove = lastMove.second;
            
            // Increment round number
            roundNumber++;
            
            // Determine winner
            int result = determineWinner(humanMove, computerMove);
            std::string winner;
            
            if (result > 0) {
                winner = "HUMAN";
                humanWins++;
            } else if (result < 0) {
                winner = "COMPUTER";
                computerWins++;
            } else {
                winner = "TIE";
                ties++;
            }
            
            // Output round information to file
            if (outputFile.is_open()) {
                outputFile << "Round " << roundNumber << std::endl;
                outputFile << "  HUMAN's choice? " << (humanMove == Move::ROCK ? "r" : 
                                                     (humanMove == Move::PAPER ? "p" : "s")) << std::endl;
                // Convert move strings to uppercase
                std::string humanMoveStr = moveToString(humanMove);
                std::string computerMoveStr = moveToString(computerMove);
                for (char& c : humanMoveStr) c = std::toupper(c);
                for (char& c : computerMoveStr) c = std::toupper(c);
                
                outputFile << "  HUMAN chose " << humanMoveStr << std::endl;
                outputFile << "  COMPUTER chose " << computerMoveStr << std::endl;
                outputFile << "  The winner is: " << winner << std::endl << std::endl;
            }
        }
    }
    
    void saveState() override {
        // No state to save for random strategy
        
        // Output match statistics to file
        if (outputFile.is_open()) {
            int totalRounds = humanWins + computerWins + ties;
            
            outputFile << "Match stats" << std::endl;
            outputFile << "-----------" << std::endl;
            outputFile << "   Human wins: " << std::setw(5) << humanWins << "  " 
                      << std::setw(3) << (totalRounds > 0 ? (humanWins * 100 / totalRounds) : 0) << "%" << std::endl;
            outputFile << "Computer wins: " << std::setw(5) << computerWins << "  " 
                      << std::setw(3) << (totalRounds > 0 ? (computerWins * 100 / totalRounds) : 0) << "%" << std::endl;
            outputFile << "         Ties: " << std::setw(5) << ties << "  " 
                      << std::setw(3) << (totalRounds > 0 ? (ties * 100 / totalRounds) : 0) << "%" << std::endl;
        }
    }
    
    void loadState() override {
        // No state to load for random strategy
    }
    
    std::string getName() const override {
        return "Random";
    }
};

#endif
