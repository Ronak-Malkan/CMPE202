#include "Game.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "RandomStrategy.h"
#include "SmartStrategy.h"
#include <iostream>
#include <memory>
#include <string>

int main() {
    std::cout << "Welcome to Rock-Paper-Scissors Game!" << std::endl;
    
    // Ask user which strategy the computer should use
    std::cout << "Choose computer strategy:" << std::endl;
    std::cout << "1. Random (computer makes random choices)" << std::endl;
    std::cout << "2. Smart (computer learns from your patterns)" << std::endl;
    std::cout << "Enter choice (1 or 2): ";
    
    int strategyChoice;
    std::cin >> strategyChoice;
    
    // Create players
    auto humanPlayer = std::make_unique<HumanPlayer>();
    std::unique_ptr<ComputerPlayer> computerPlayer;
    
    // Create appropriate strategy based on user choice
    if (strategyChoice == 2) {
        computerPlayer = std::make_unique<ComputerPlayer>(std::make_unique<SmartStrategy>());
    } else {
        // Default to random strategy
        computerPlayer = std::make_unique<ComputerPlayer>(std::make_unique<RandomStrategy>());
    }
    
    // Create and play the game
    Game game(std::move(humanPlayer), std::move(computerPlayer));
    game.play();
    
    return 0;
}
