#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "Player.h"
#include <iostream>

class HumanPlayer : public Player {
public:
    Move makeMove() override {
        char choice;
        
        while (true) {
            std::cout << "Enter your move (R for Rock, P for Paper, S for Scissors): ";
            std::cin >> choice;
            
            try {
                return charToMove(choice);  // If valid, return the move
            } catch (const std::invalid_argument&) {
                std::cout << "Invalid input. Please enter R, P, or S." << std::endl;
            }
        }
    }
    
    void recordResult(Move playerMove, Move opponentMove) override {
        // Human player doesn't need to record results
    }
};

#endif
