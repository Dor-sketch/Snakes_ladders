#include "tm.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>


std::map<std::string, std::map<char, std::pair<std::string, std::pair<char, std::string>>>> load_transitions_from_csv(const std::string& filename) {
    std::map<std::string, std::map<char, std::pair<std::string, std::pair<char, std::string>>>> transitions;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string item;
        std::vector<std::string> row;

        while (std::getline(ss, item, ',')) {
            // Remove comments
            item = item.substr(0, item.find('#'));
            item = item.substr(0, item.find("//"));
            // Remove leading and trailing whitespace
            item.erase(0, item.find_first_not_of(' '));
            item.erase(item.find_last_not_of(' ') + 1);
            row.push_back(item);
        }

        if (row.size() >= 5) {
            std::string current_state = row[0];
            char input_symbol = row[1][0];
            std::string new_state = row[2];
            char new_symbol = row[3][0];
            std::string move = row[4];

            transitions[current_state][input_symbol] = std::make_pair(new_state, std::make_pair(new_symbol, move));
        }
    }

    return transitions;
}


TuringGame::TuringGame(){
    transitionFunction = load_transitions_from_csv("pal.csv");
    for(const auto &state : transitionFunction) {
        for(const auto &symbol : state.second) {
            std::cout << "State: " << state.first << ", Symbol: " << symbol.first << ", New State: " << symbol.second.first << ", New Symbol: " << symbol.second.second.first << ", Move: " << symbol.second.second.second << std::endl;
        }
    }
    // Set the board's state according to the transition function
    setBoardFromTransitionFunction();
}

void TuringGame::movePlayer(int state, char readSymbol) {
    int playerPos = this->playerPos[0][0];
    char symbol = this->board[playerPos / this->n][playerPos % this->n];
    std::pair<std::string, std::pair<char, std::string>> nextStateSymbol = this->transitionFunction[std::to_string(state)][symbol];

    if (nextStateSymbol.second.second == "R") {
        playerPos++;
    } else if (nextStateSymbol.second.second == "L") {
        playerPos--;
    }

    this->board[playerPos / this->n][playerPos % this->n] = nextStateSymbol.second.first;
    this->playerPos[0][0] = playerPos; // Update the player position
}


void TuringGame::setBoardFromTransitionFunction() {
    // Iterate over the transition function and set the board's state
    for (const auto& state : transitionFunction) {
        for (const auto& symbol : state.second) {
            // Set the board's state here
            // This is a placeholder implementation, replace with your actual code
            this->board[0][0] = symbol.second.second.first;
        }
    }
}

void TuringGame::processInputWord(const std::string& word) {
    // Iterate over the characters in the word
    for (char c : word) {
        // Process the character according to the transition function
        // This is a placeholder implementation, replace with your actual code
        this->movePlayer(0, c);
    }
}