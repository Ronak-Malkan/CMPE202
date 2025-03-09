#ifndef SMART_STRATEGY_H
#define SMART_STRATEGY_H

#include "Strategy.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>

class SmartStrategy : public Strategy {
private:
    // Map to store frequencies of sequences
    std::map<std::string, std::map<Move, int>> frequencies;
    
    // Output file for detailed logging
    std::ofstream outputFile;
    
    // Round counter (initialized to 0)
    int roundNumber;
    
    // Score counters
    int humanWins;
    int computerWins;
    int ties;
    
    // Length of sequence to consider
    static const int SEQUENCE_LENGTH = 6;
    
    // Convert a sequence of moves to a string key
    std::string movesToKey(const std::vector<std::pair<Move, Move>>& history, int start, int length) {
        std::string key;
        for (int i = start; i < start + length; ++i) {
            if (i < history.size()) {
                // Add both player and computer moves to the key
                key += std::to_string(static_cast<int>(history[i].first));
                key += std::to_string(static_cast<int>(history[i].second));
            }
        }
        return key;
    }
    
    // Predict the next move based on frequencies
    Move predictNextMove(const std::string& key) {
        if (frequencies.find(key) == frequencies.end() || frequencies[key].empty()) {
            // If no data, return a random move
            return Move(std::rand() % 3);
        }
        
        // Find the move with the highest frequency
        Move predictedMove = Move::ROCK;
        int maxFreq = 0;
        
        for (const auto& pair : frequencies[key]) {
            if (pair.second > maxFreq) {
                maxFreq = pair.second;
                predictedMove = pair.first;
            }
        }
        
        return predictedMove;
    }
    
    // Choose a move that beats the predicted move
    Move chooseCounterMove(Move predictedMove) {
        switch (predictedMove) {
            case Move::ROCK:
                return Move::PAPER;
            case Move::PAPER:
                return Move::SCISSORS;
            case Move::SCISSORS:
                return Move::ROCK;
            default:
                return Move::ROCK; // Should never happen
        }
    }

public:
    SmartStrategy() {
        // Seed the random number generator
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        
        // Open output file in the same directory as freq.txt (build folder)
        outputFile.open("output-smart.txt");
        if (!outputFile.is_open()) {
            std::cerr << "Failed to open output-smart.txt for writing." << std::endl;
        }
        
        // Initialize counters
        roundNumber = 0;
        humanWins = 0;
        computerWins = 0;
        ties = 0;
        
        // Load frequencies from file
        loadState();
        
        // Log the number of records loaded
        if (outputFile.is_open()) {
            outputFile << "Reading file freq.txt: " << frequencies.size() << " records." << std::endl << std::endl;
        }
    }
    
    ~SmartStrategy() {
        // Close the file if it's open
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }
    
    Move makeMove(const std::vector<std::pair<Move, Move>>& history) override {
        // Increment round number for each move (including the first one)
        roundNumber++;
        
        // If we have at least one move in history, log the round with human's previous move
        if (!history.empty() && outputFile.is_open()) {
            // Get the last move pair
            const auto& lastMove = history.back();
            Move humanMove = lastMove.first;
            
            // Log round information
            outputFile << "Round " << roundNumber << std::endl;
            outputFile << "  HUMAN's choice? " << (humanMove == Move::ROCK ? "r" : 
                                                 (humanMove == Move::PAPER ? "p" : "s")) << std::endl;
            
            // Convert move string to uppercase
            std::string humanMoveStr = moveToString(humanMove);
            for (char& c : humanMoveStr) c = std::toupper(c);
            
            outputFile << "  HUMAN chose " << humanMoveStr << std::endl;
        } else if (outputFile.is_open()) {
            // For the first round, just log the round number
            outputFile << "Round " << roundNumber << std::endl;
        }
        
        if (history.size() < SEQUENCE_LENGTH - 1) {
            // Not enough history, make a random move
            if (outputFile.is_open()) {
                outputFile << "    Insufficient history to predict." << std::endl;
                outputFile << "    Computer will choose randomly." << std::endl;
            }
            
            // Generate a random move
            int randomNum = std::rand() % 3;
            Move computerMove;
            
            switch (randomNum) {
                case 0: computerMove = Move::ROCK; break;
                case 1: computerMove = Move::PAPER; break;
                case 2: computerMove = Move::SCISSORS; break;
                default: computerMove = Move::ROCK; // Should never happen
            }
            
            // Log the computer's move and determine winner if we have history
            if (!history.empty() && outputFile.is_open()) {
                // Convert move string to uppercase
                std::string computerMoveStr = moveToString(computerMove);
                for (char& c : computerMoveStr) c = std::toupper(c);
                
                outputFile << "  COMPUTER chose " << computerMoveStr << std::endl;
                
                // Determine winner
                const auto& lastMove = history.back();
                Move humanMove = lastMove.first;
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
                
                outputFile << "  The winner is: " << winner << std::endl << std::endl;
            }
            
            return computerMove;
        }
        
        // Get the key for the current sequence
        std::string key = movesToKey(history, history.size() - (SEQUENCE_LENGTH - 1), SEQUENCE_LENGTH - 1);
        
        // Log the history analysis
        if (outputFile.is_open()) {
            for (int i = history.size() - (SEQUENCE_LENGTH - 1); i < history.size(); ++i) {
                char moveChar = (history[i].first == Move::ROCK ? 'R' : 
                               (history[i].first == Move::PAPER ? 'P' : 'S'));
                
                outputFile << "    Appended " << moveChar << std::endl;
                
                // Create a partial history string for logging
                std::string historyStr;
                for (int j = history.size() - (SEQUENCE_LENGTH - 1); j <= i; ++j) {
                    historyStr += (history[j].first == Move::ROCK ? 'R' : 
                                 (history[j].first == Move::PAPER ? 'P' : 'S'));
                }
                
                outputFile << "    History " << historyStr << std::endl;
            }
        }
        
        // Predict the player's next move
        Move predictedMove = predictNextMove(key);
        
        // Log the prediction process
        if (outputFile.is_open()) {
            if (frequencies.find(key) != frequencies.end()) {
                for (const auto& pair : frequencies[key]) {
                    std::string moveStr = (pair.first == Move::ROCK ? "R" : 
                                         (pair.first == Move::PAPER ? "P" : "S"));
                    
                    outputFile << "    Checked: " << key << moveStr << ":" << pair.second << std::endl;
                }
                
                std::string predictedMoveStr = (predictedMove == Move::ROCK ? "ROCK" : 
                                              (predictedMove == Move::PAPER ? "PAPER" : "SCISSORS"));
                
                outputFile << "    Predicted human choice: " << predictedMoveStr << std::endl;
            } else {
                outputFile << "    No matching pattern found. Using random prediction." << std::endl;
            }
        }
        
        // Choose a move that beats the predicted move
        Move computerMove = chooseCounterMove(predictedMove);
        
        // Log the computer's move and determine winner
        if (outputFile.is_open()) {
            // Convert move string to uppercase
            std::string computerMoveStr = moveToString(computerMove);
            for (char& c : computerMoveStr) c = std::toupper(c);
            
            outputFile << "  COMPUTER chose " << computerMoveStr << std::endl;
            
            // Determine winner
            const auto& lastMove = history.back();
            Move humanMove = lastMove.first;
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
            
            outputFile << "  The winner is: " << winner << std::endl << std::endl;
        }
        
        return computerMove;
    }
    
    void updateFrequencies(const std::vector<std::pair<Move, Move>>& history) override {
        if (history.size() < SEQUENCE_LENGTH) {
            return;
        }
        
        // Get the key for the sequence leading up to the player's last move
        std::string key = movesToKey(history, history.size() - SEQUENCE_LENGTH, SEQUENCE_LENGTH - 1);
        
        // Update the frequency for the player's last move
        frequencies[key][history.back().first]++;
    }
    
    void saveState() override {
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
            outputFile << std::endl;
        }
        
        // Save frequencies to file
        std::ofstream file("freq.txt");
        
        if (!file.is_open()) {
            std::cerr << "Failed to open file for saving strategy data." << std::endl;
            return;
        }
        
        // Write a legend to explain the key format
        file << "# Legend:" << std::endl;
        file << "# Each key represents a sequence of moves in the format:" << std::endl;
        file << "# HCHCHCHCHCHC where H=Human move, C=Computer move" << std::endl;
        file << "# 0=ROCK, 1=PAPER, 2=SCISSORS" << std::endl;
        file << "# For example, 0102211201 means:" << std::endl;
        file << "# Human: ROCK, Computer: PAPER, Human: SCISSORS, Computer: SCISSORS, Human: ROCK, Computer: ROCK, etc." << std::endl;
        file << "# The number after each key is the count of different moves that followed this sequence" << std::endl;
        file << "# The next lines show which move (0=ROCK, 1=PAPER, 2=SCISSORS) followed this sequence and how many times" << std::endl;
        file << std::endl;
        
        // Write the number of entries
        file << frequencies.size() << std::endl;
        
        // Write each entry
        for (const auto& entry : frequencies) {
            // Convert the key to a more readable format for the comment
            std::string readableKey;
            for (size_t i = 0; i < entry.first.length(); i += 2) {
                if (i > 0) readableKey += ", ";
                
                // Human move
                if (i < entry.first.length()) {
                    char humanMove = entry.first[i];
                    readableKey += "H:";
                    readableKey += (humanMove == '0' ? "R" : (humanMove == '1' ? "P" : "S"));
                }
                
                // Computer move
                if (i + 1 < entry.first.length()) {
                    char computerMove = entry.first[i + 1];
                    readableKey += " C:";
                    readableKey += (computerMove == '0' ? "R" : (computerMove == '1' ? "P" : "S"));
                }
            }
            
            // Write the key, number of moves, and a comment explaining the key
            file << entry.first << " " << entry.second.size() << " # " << readableKey << std::endl;
            
            for (const auto& moveFreq : entry.second) {
                char moveChar = (moveFreq.first == Move::ROCK ? 'R' : 
                               (moveFreq.first == Move::PAPER ? 'P' : 'S'));
                file << static_cast<int>(moveFreq.first) << " " << moveFreq.second << " # " << moveChar << std::endl;
            }
        }
        
        file.close();
        
        // Log the frequency file writing
        if (outputFile.is_open()) {
            outputFile << "Writing frequency file freq.txt: " << frequencies.size() << " records." << std::endl;
        }
    }
    
    void loadState() override {
        std::ifstream file("freq.txt");
        
        if (!file.is_open()) {
            std::cerr << "No previous strategy data found. Starting fresh." << std::endl;
            return;
        }
        
        frequencies.clear();
        
        std::string line;
        int numEntries = 0;
        bool foundNumEntries = false;
        
        // Skip comments and find the number of entries
        while (std::getline(file, line)) {
            // Skip empty lines
            if (line.empty()) {
                continue;
            }
            
            // Skip comment lines
            if (line[0] == '#') {
                continue;
            }
            
            // Found the number of entries
            std::istringstream iss(line);
            iss >> numEntries;
            foundNumEntries = true;
            break;
        }
        
        if (!foundNumEntries) {
            std::cerr << "Invalid frequency file format." << std::endl;
            return;
        }
        
        // Read each entry
        for (int i = 0; i < numEntries; ++i) {
            std::string key;
            int numMoves;
            
            // Read the key and number of moves
            if (!std::getline(file, line)) {
                std::cerr << "Unexpected end of file." << std::endl;
                break;
            }
            
            // Skip comment lines
            while (line.empty() || line[0] == '#') {
                if (!std::getline(file, line)) {
                    std::cerr << "Unexpected end of file." << std::endl;
                    return;
                }
            }
            
            // Extract key and numMoves, ignoring comments
            std::istringstream iss(line);
            iss >> key >> numMoves;
            
            for (int j = 0; j < numMoves; ++j) {
                int moveInt, freq;
                
                // Read the move and frequency
                if (!std::getline(file, line)) {
                    std::cerr << "Unexpected end of file." << std::endl;
                    break;
                }
                
                // Skip comment lines
                while (line.empty() || line[0] == '#') {
                    if (!std::getline(file, line)) {
                        std::cerr << "Unexpected end of file." << std::endl;
                        return;
                    }
                }
                
                // Extract moveInt and freq, ignoring comments
                std::istringstream moveIss(line);
                moveIss >> moveInt >> freq;
                
                frequencies[key][static_cast<Move>(moveInt)] = freq;
            }
        }
        
        file.close();
    }
    
    std::string getName() const override {
        return "Smart";
    }
};

#endif
