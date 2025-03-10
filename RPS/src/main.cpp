#include "Game.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "RandomStrategy.h"
#include "SmartStrategy.h"
#include <iostream>
#include <memory>
#include <string>
#include <limits>


void getChoice(int& choice) {

     std::cout << "Choose computer strategy:" << std::endl;
     std::cout << "1. Random (computer makes random choices)" << std::endl;
     std::cout << "2. Smart (computer learns from your patterns)" << std::endl;
     std::cout << "Enter choice (1 or 2): ";
     
     std::cin >> choice;

     if (std::cin.fail()) {
        std::cin.clear(); // clear error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
     }
}

int main() {
    std::cout << "Welcome to Rock-Paper-Scissors Game!" << std::endl;

    char continueGame = 'c';
    while (continueGame != 'q' && continueGame != 'Q') {

        // Create players
        auto humanPlayer = std::make_unique<HumanPlayer>();
        std::unique_ptr<ComputerPlayer> computerPlayer;
        int strategyChoice;

        bool selected = false;
        
        while (!selected) {

            getChoice(strategyChoice);

            // if user not selected a proper strategy
            if ( strategyChoice != 1 && strategyChoice != 2 ) {
                std::cout << "ERROR: Please either select 1 or 2!" << "\n\n\n";
                continue;
            }
            else if (strategyChoice == 1) {
                computerPlayer = std::make_unique<ComputerPlayer>(std::make_unique<RandomStrategy>());
                
            }
            else if (strategyChoice == 2) {
                computerPlayer = std::make_unique<ComputerPlayer>(std::make_unique<SmartStrategy>());
                
            } 
            selected = true;
            
        }
        // Create and play the game
        Game game(std::move(humanPlayer), std::move(computerPlayer));
        game.play();

        std::cout << "Enter 'q' to quit, or any other key to play again: ";
        std::cin >> continueGame;
        std::cout << "\n";
    }
    
    return 0;
}
