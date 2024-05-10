#pragma once
#include "game.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

class TuringGame : public Game {
public:
    TuringGame();

    // Override the movePlayer function
    void movePlayer(int state, char readSymbol);

    // New function to set the board's state according to the transition function
    void setBoardFromTransitionFunction();

    // New function to process an input word
    void processInputWord(const std::string& word);

private:
    // The transition function of the Turing Machine
    std::map<std::string, std::map<char, std::pair<std::string, std::pair<char, std::string>>>> transitionFunction;
};