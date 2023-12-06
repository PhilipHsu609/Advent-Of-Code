#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

std::vector<std::string> readInput() {
    std::ifstream input{"input.txt"};

    std::string line;
    std::getline(input, line);

    int width = line.size();
    input.seekg(0, std::ios::beg);
    std::vector<std::string> board{std::string(width + 2, '.')};
    
    while(!input.eof()) {
        std::getline(input, line);

        std::string row(width + 2, '.');
        row.replace(1, width, line);
        board.push_back(row);
    }

    board.push_back(std::string(width + 2, '.'));
    return board;
}

bool isPart(const std::vector<std::string> &board, int i, int j, int digits) {
    /*
        @.....
        .1234.
        ......

        (i, j) is the position of @
    */
    int m = board.size(), n = board[0].size();

    for(int k = j; k < j + digits + 2; k++) {
        if(board[i][k] != '.' && !isdigit(board[i][k])) {
            return true;
        }
        if(board[i + 2][k] != '.' && !isdigit(board[i + 2][k])) {
            return true;
        }
    }

    if(board[i + 1][j] != '.' && !isdigit(board[i + 1][j])) {
        return true;
    }

    if(board[i + 1][j + digits + 1] != '.' && !isdigit(board[i + 1][j + digits + 1])) {
        return true;
    }

    return false;
}

void part1() {
    auto board{readInput()};
    int m = board.size(), n = board[0].size();

    int sum = 0;
    for(int i = 1; i < m - 1; i++) {
        int val = 0, digits = 0;

        for(int j = 1; j < n; j++) {
            if(!isdigit(board[i][j])) {
                if(digits != 0) {
                    if(isPart(board, i - 1, j - digits - 1, digits)) {
                        sum += val;
                    }
                    val = 0;
                    digits = 0;
                }
            } else {
                val = val * 10 + (board[i][j] - '0');
                digits++;
            }
        }
    }

    std::cout << "Part1: " << sum << std::endl;
}

void part2() {
    auto board{readInput()};
    int m = board.size(), n = board[0].size();

    // A gear at (i, j) has id = i * n + j.
    // gearsToParts[id] stores the adjacent parts of gear id.
    std::unordered_map<int, std::vector<int>> gearsToParts;

    int sum = 0, id = 0;
    for(int i = 1; i < m - 1; i++) {
        int val = 0, digits = 0;

        for(int j = 1; j < n; j++) {
            if(!isdigit(board[i][j])) {
                if(digits != 0) {
                    // search for adjacent gears

                    // top & bottom
                    for(int k = j - digits - 1; k <= j; k++) {
                        if(board[i - 1][k] == '*') {
                            gearsToParts[(i - 1) * n + k].push_back(val);
                        }
                        if(board[i + 1][k] == '*') {
                            gearsToParts[(i + 1) * n + k].push_back(val);
                        }
                    }

                    // left & right
                    if(board[i][j - digits - 1] == '*') {
                        gearsToParts[i * n + j - digits - 1].push_back(val);
                    }
                    if(board[i][j] == '*') {
                        gearsToParts[i * n + j].push_back(val);
                    }

                    val = 0;
                    digits = 0;
                }
            } else {
                val = val * 10 + (board[i][j] - '0');
                digits++;
                board[i][j] = id;
            }
        }
    }

    // find the gears that have only two adjacent parts
    for(auto &[id, parts] : gearsToParts) {
        if(parts.size() == 2) {
            sum += parts.front() * parts.back();
        }
    }

    std::cout << "Part2: " << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}