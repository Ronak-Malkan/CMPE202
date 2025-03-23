#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <stdexcept>

enum class Move {
    ROCK,
    PAPER,
    SCISSORS
};

// Convert a character input to a Move
inline Move charToMove(char c) {
    switch (c) {
        case 'R':
        case 'r':
            return Move::ROCK;
        case 'P':
        case 'p':
            return Move::PAPER;
        case 'S':
        case 's':
            return Move::SCISSORS;
        default:
            throw std::invalid_argument("Invalid move character");
    }
}

// Convert a Move to a string for display
inline std::string moveToString(Move move) {
    switch (move) {
        case Move::ROCK:
            return "Rock";
        case Move::PAPER:
            return "Paper";
        case Move::SCISSORS:
            return "Scissors";
        default:
            return "Unknown";
    }
}

// Determine the winner given two moves
inline int determineWinner(Move playerMove, Move computerMove) {
    if (playerMove == computerMove) {
        return 0; // Tie
    }
    
    if ((playerMove == Move::ROCK && computerMove == Move::SCISSORS) ||
        (playerMove == Move::PAPER && computerMove == Move::ROCK) ||
        (playerMove == Move::SCISSORS && computerMove == Move::PAPER)) {
        return 1; // Player wins
    }
    
    return -1; // Computer wins
}

#endif
