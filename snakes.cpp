#include <vector>
#include <queue>
#include <unordered_set>
#include "snakes.h"


Snakes::Snakes() {}

int Snakes::snakesAndLadders(std::vector<std::vector<int>>& board) {
    int n = board.size();
    // Convert a cell number to its (row, col) position
    auto getPos = [n](int num) -> std::pair<int, int> {
        int row = n - (num - 1) / n - 1;
        int col = (num - 1) % n;
        if ((n - row) % 2 == 0) col = n - 1 - col;
        return {row, col};
    };

    std::queue<std::pair<int, int>> q; // Stores {cell number, moves}
    q.push({1, 0}); // Start at cell 1 with 0 moves
    std::unordered_set<int> visited; // Visited cell numbers
    visited.insert(1);

    while (!q.empty()) {
        auto [num, moves] = q.front();
        q.pop();

        for (int i = 1; i <= 6; i++) {
            int nextNum = num + i;
            if (nextNum > n * n) break; // Beyond board bounds

            auto [row, col] = getPos(nextNum);
            if (board[row][col] != -1) nextNum = board[row][col]; // Ladder or snake

            if (nextNum == n * n) return moves + 1; // Reached the goal
            if (visited.find(nextNum) == visited.end()) {
                visited.insert(nextNum);
                q.push({nextNum, moves + 1});
            }
        }
    }

    return -1; // Goal not reachable
}