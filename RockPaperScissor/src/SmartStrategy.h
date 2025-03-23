#ifndef SMART_STRATEGY_H
#define SMART_STRATEGY_H

#include "Strategy.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <ctime>

// Updated SmartStrategy that records multiple sequence lengths simultaneously.
class SmartStrategy : public Strategy {
private:
    // For each sequence length (N), we store a frequency table.
    // Each frequency table maps a key (constructed from the last (N-1) rounds)
    // to a map that counts how many times each human move followed that sequence.
    std::map<int, std::map<std::string, std::map<Move, int>>> frequenciesByLength;
    
    // List of sequence lengths to record (for example, 3, 4, 5, 6, 7)
    std::vector<int> seqLengths = {3, 4, 5, 6, 7};
    
    // Output file for detailed logging
    std::ofstream outputFile;
    
    // Round counter and score counters
    int roundNumber;
    int humanWins;
    int computerWins;
    int ties;
    
    // Convert a sequence of moves to a string key using a given number of rounds.
    // Here, 'length' refers to the number of rounds considered (which is N-1).
    std::string movesToKey(const std::vector<std::pair<Move, Move>>& history, int start, int length) {
        std::string key;
        for (int i = start; i < start + length; ++i) {
            if (i < history.size()) {
                // Append both human and computer moves as integers.
                key += std::to_string(static_cast<int>(history[i].first));
                key += std::to_string(static_cast<int>(history[i].second));
            }
        }
        return key;
    }
    
    // For a given sequence length and key, predict the next human move using its frequency table.
    // If no data exists for that key, return a random move.
    Move predictNextMoveForLength(int seqLen, const std::string& key) {
        auto& freqMap = frequenciesByLength[seqLen];
        if (freqMap.find(key) == freqMap.end() || freqMap[key].empty()) {
            return Move(std::rand() % 3);
        }
        
        Move predictedMove = Move::ROCK;
        int maxFreq = 0;
        for (const auto& pair : freqMap[key]) {
            if (pair.second > maxFreq) {
                maxFreq = pair.second;
                predictedMove = pair.first;
            }
        }
        return predictedMove;
    }
    
    // Choose a move that beats the predicted human move.
    Move chooseCounterMove(Move predictedMove) {
        switch (predictedMove) {
            case Move::ROCK:
                return Move::PAPER;
            case Move::PAPER:
                return Move::SCISSORS;
            case Move::SCISSORS:
                return Move::ROCK;
            default:
                return Move::ROCK; // Fallback
        }
    }
    
    // Aggregate predictions from all sequence lengths.
    // We sum up the frequencies for each move across all available sequence lengths.
    Move aggregatePredictions(const std::vector<std::pair<Move, Move>>& history) {
        std::map<Move, int> aggregated;
        bool anyData = false;
        
        for (int seqLen : seqLengths) {
            // Need at least (seqLen - 1) rounds of history
            if (history.size() < static_cast<size_t>(seqLen - 1)) {
                continue;
            }
            
            int start = history.size() - (seqLen - 1);
            std::string key = movesToKey(history, start, seqLen - 1);
            
            auto& freqMap = frequenciesByLength[seqLen];
            if (freqMap.find(key) == freqMap.end()) {
                continue;
            }
            anyData = true;
            for (const auto& pair : freqMap[key]) {
                aggregated[pair.first] += pair.second;
            }
            
            // Log details for this sequence length
            if (outputFile.is_open()) {
                outputFile << "SeqLen " << seqLen << " key: " << key << std::endl;
                for (const auto& pair : freqMap[key]) {
                    std::string moveStr = (pair.first == Move::ROCK ? "R" : 
                                           (pair.first == Move::PAPER ? "P" : "S"));
                    outputFile << "    " << moveStr << " : " << pair.second << std::endl;
                }
            }
        }
        
        // If no aggregated data, return a random move.
        if (!anyData || aggregated.empty()) {
            return Move(std::rand() % 3);
        }
        
        // Pick the move with the highest total frequency.
        Move predictedMove = Move::ROCK;
        int maxFreq = 0;
        for (const auto& entry : aggregated) {
            if (entry.second > maxFreq) {
                maxFreq = entry.second;
                predictedMove = entry.first;
            }
        }
        return predictedMove;
    }
    
public:
    SmartStrategy() {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        outputFile.open("output-smart.txt");
        if (!outputFile.is_open()) {
            std::cerr << "Failed to open output-smart.txt for writing." << std::endl;
        }
        
        roundNumber = 0;
        humanWins = 0;
        computerWins = 0;
        ties = 0;
        
        // Load frequencies from file
        loadState();
        if (outputFile.is_open()) {
            outputFile << "Reading file freq.txt: " 
                       << "Records across " << seqLengths.size() << " sequence lengths." 
                       << std::endl << std::endl;
        }
    }
    
    ~SmartStrategy() {
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }
    
    Move makeMove(const std::vector<std::pair<Move, Move>>& history) override {
        roundNumber++;
        
        if (!history.empty() && outputFile.is_open()) {
            const auto& lastMove = history.back();
            Move humanMove = lastMove.first;
            outputFile << "Round " << roundNumber << std::endl;
            outputFile << "  HUMAN's choice? " 
                       << (humanMove == Move::ROCK ? "r" : (humanMove == Move::PAPER ? "p" : "s"))
                       << std::endl;
            std::string humanMoveStr = moveToString(humanMove);
            for (char& c : humanMoveStr) c = std::toupper(c);
            outputFile << "  HUMAN chose " << humanMoveStr << std::endl;
        } else if (outputFile.is_open()) {
            outputFile << "Round " << roundNumber << std::endl;
        }
        
        // If insufficient history for any sequence length, choose random.
        bool sufficientHistory = false;
        for (int seqLen : seqLengths) {
            if (history.size() >= static_cast<size_t>(seqLen - 1)) {
                sufficientHistory = true;
                break;
            }
        }
        if (!sufficientHistory) {
            if (outputFile.is_open()) {
                outputFile << "    Insufficient history to predict across any sequence length." << std::endl;
                outputFile << "    Computer will choose randomly." << std::endl;
            }
            int randomNum = std::rand() % 3;
            Move computerMove = static_cast<Move>(randomNum);
            
            // Log and determine winner if possible
            if (!history.empty() && outputFile.is_open()) {
                std::string computerMoveStr = moveToString(computerMove);
                for (char& c : computerMoveStr) c = std::toupper(c);
                outputFile << "  COMPUTER chose " << computerMoveStr << std::endl;
                const auto& lastMove = history.back();
                Move humanMove = lastMove.first;
                int result = determineWinner(humanMove, computerMove);
                std::string winner = (result > 0 ? "HUMAN" : (result < 0 ? "COMPUTER" : "TIE"));
                if (result > 0) humanWins++;
                else if (result < 0) computerWins++;
                else ties++;
                outputFile << "  The winner is: " << winner << std::endl << std::endl;
            }
            return computerMove;
        }
        
        // Aggregate predictions from all sequence lengths
        Move predictedMove = aggregatePredictions(history);
        
        if (outputFile.is_open()) {
            std::string predictedMoveStr = (predictedMove == Move::ROCK ? "ROCK" :
                                             (predictedMove == Move::PAPER ? "PAPER" : "SCISSORS"));
            outputFile << "    Aggregated predicted human choice: " << predictedMoveStr << std::endl;
        }
        
        // Choose the move that beats the aggregated prediction
        Move computerMove = chooseCounterMove(predictedMove);
        
        if (outputFile.is_open()) {
            std::string computerMoveStr = moveToString(computerMove);
            for (char& c : computerMoveStr) c = std::toupper(c);
            outputFile << "  COMPUTER chose " << computerMoveStr << std::endl;
            
            // Determine round winner using the last human move (if available)
            const auto& lastMove = history.back();
            Move humanMove = lastMove.first;
            int result = determineWinner(humanMove, computerMove);
            std::string winner = (result > 0 ? "HUMAN" : (result < 0 ? "COMPUTER" : "TIE"));
            if (result > 0) humanWins++;
            else if (result < 0) computerWins++;
            else ties++;
            outputFile << "  The winner is: " << winner << std::endl << std::endl;
        }
        
        return computerMove;
    }
    
    void updateFrequencies(const std::vector<std::pair<Move, Move>>& history) override {
        // Update each frequency table for every sequence length.
        for (int seqLen : seqLengths) {
            if (history.size() < static_cast<size_t>(seqLen)) {
                continue; // Not enough rounds for this sequence length
            }
            int start = history.size() - seqLen;
            std::string key = movesToKey(history, start, seqLen - 1);
            frequenciesByLength[seqLen][key][history.back().first]++;
        }
    }
    
    void saveState() override {
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
        
        // Save all frequency tables to file "freq.txt"
        std::ofstream file("freq.txt");
        if (!file.is_open()) {
            std::cerr << "Failed to open file for saving strategy data." << std::endl;
            return;
        }
        
        // Write a legend
        file << "# Legend:" << std::endl;
        file << "# Each block corresponds to a sequence length (N) frequency table." << std::endl;
        file << "# For a given sequence length N, keys are constructed from the last (N-1) rounds," << std::endl;
        file << "# and the following lines show the frequencies for each human move that followed that sequence." << std::endl;
        file << std::endl;
        
        // Write frequency data for each sequence length
        file << frequenciesByLength.size() << std::endl;
        for (const auto& lengthEntry : frequenciesByLength) {
            int seqLen = lengthEntry.first;
            file << "# Sequence length: " << seqLen << std::endl;
            file << lengthEntry.second.size() << std::endl;
            for (const auto& entry : lengthEntry.second) {
                std::string key = entry.first;
                file << key << " " << entry.second.size() << " # Key for N=" << seqLen << std::endl;
                for (const auto& moveFreq : entry.second) {
                    file << static_cast<int>(moveFreq.first) << " " << moveFreq.second << " # " 
                         << (moveFreq.first == Move::ROCK ? "R" : (moveFreq.first == Move::PAPER ? "P" : "S")) << std::endl;
                }
            }
        }
        
        file.close();
        
        if (outputFile.is_open()) {
            outputFile << "Writing frequency file freq.txt: Frequency data for " 
                       << frequenciesByLength.size() << " sequence lengths." << std::endl;
        }
    }
    
    void loadState() override {
        std::ifstream file("freq.txt");
        if (!file.is_open()) {
            std::cerr << "No previous strategy data found. Starting fresh." << std::endl;
            return;
        }
        
        frequenciesByLength.clear();
        std::string line;
        int numBlocks = 0;
        bool foundNumBlocks = false;
        
        // Skip comments to find the number of sequence blocks
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::istringstream iss(line);
            iss >> numBlocks;
            foundNumBlocks = true;
            break;
        }
        
        if (!foundNumBlocks) {
            std::cerr << "Invalid frequency file format." << std::endl;
            return;
        }
        
        // For each block, read its frequency table
        for (int b = 0; b < numBlocks; ++b) {
            int seqLen = 0;
            // Skip comments until we get the sequence length header
            while (std::getline(file, line)) {
                if (!line.empty() && line[0] == '#') {
                    if (line.find("Sequence length:") != std::string::npos) {
                        std::istringstream iss(line.substr(line.find(":") + 1));
                        iss >> seqLen;
                        break;
                    }
                }
            }
            if (seqLen == 0) continue;
            
            int numEntries = 0;
            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#') continue;
                std::istringstream iss(line);
                iss >> numEntries;
                break;
            }
            
            for (int i = 0; i < numEntries; ++i) {
                std::string key;
                int numMoves = 0;
                while (std::getline(file, line)) {
                    if (line.empty() || line[0] == '#') continue;
                    std::istringstream iss(line);
                    iss >> key >> numMoves;
                    break;
                }
                for (int j = 0; j < numMoves; ++j) {
                    int moveInt, freq;
                    while (std::getline(file, line)) {
                        if (line.empty() || line[0] == '#') continue;
                        std::istringstream iss(line);
                        iss >> moveInt >> freq;
                        frequenciesByLength[seqLen][key][static_cast<Move>(moveInt)] = freq;
                        break;
                    }
                }
            }
        }
        file.close();
    }
    
    std::string getName() const override {
        return "Smart";
    }
};

#endif
