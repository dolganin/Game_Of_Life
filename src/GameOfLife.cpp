#include "include/GameOfLife.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <filesystem>

// Функция для проверки, заканчивается ли строка на заданный суффикс
bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.substr(str.length() - suffix.length()) == suffix;
}

namespace fs = std::filesystem;

void Game::loadTemplate(const std::string& filename, int startX, int startY) {
    std::ifstream file("templates/" + filename);
    if (!file.is_open()) {
        throw std::runtime_error("Template file not found: " + filename);
    }

    std::string line;
    int y = startY;
    while (std::getline(file, line)) {
        for (size_t x = 0; x < line.size(); ++x) {
            if (line[x] == 'O') {
                field[y][x + startX].isAlive = true;
            }
        }
        ++y;
    }
    file.close();
}

void Game::generateRandomState() {
    const std::string templatesDir = "templates";
    std::vector<std::string> templates;

    if (fs::exists(templatesDir) && fs::is_directory(templatesDir)) {
        for (const auto& entry : fs::directory_iterator(templatesDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                templates.push_back(entry.path().filename().string());
            }
        }
    } else {
        std::cerr << "Templates directory not found!" << std::endl;
        return;
    }

    if (templates.empty()) {
        std::cerr << "No templates found in the templates directory!" << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, templates.size() - 1);

    std::string selectedTemplate = templates[dis(gen)];
    int startX = std::uniform_int_distribution<>(0, numCols - 1)(gen);
    int startY = std::uniform_int_distribution<>(0, numRows - 1)(gen);

    loadTemplate(selectedTemplate, startX, startY);

    std::cout << "Loaded template: " << selectedTemplate << " at position (" << startX << ", " << startY << ")" << std::endl;
}

bool Game::isInteger(const std::string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

void Game::parseRules(const std::string& ruleString) {
    std::string birth, survival;
    std::string delimiter = "/";
    size_t pos = ruleString.find(delimiter);

    if (pos != std::string::npos) {
        // Разделяем строку по "/"
        birth = ruleString.substr(1, pos - 1);  // Правила рождения: после "B"
        survival = ruleString.substr(pos + 1);  // Правила выживания: после "S"

        // Очищаем векторы перед добавлением новых значений
        birthRules.clear();
        survivalRules.clear();

        // Добавляем правила для рождения
        for (char c : birth) {
            if (isdigit(c)) {
                int rule = c - '0';
                if (rule >= 0 && rule <= 8 && std::find(birthRules.begin(), birthRules.end(), rule) == birthRules.end()) {
                    birthRules.push_back(rule);
                }
            }
        }

        // Добавляем правила для выживания
        for (char c : survival) {
            if (isdigit(c)) {
                int rule = c - '0';
                if (rule >= 0 && rule <= 8 && std::find(survivalRules.begin(), survivalRules.end(), rule) == survivalRules.end()) {
                    survivalRules.push_back(rule);
                }
            }
        }
    }
}




void Game::readFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file '" << filename << "'" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        int x, y;
        if (iss >> x >> y) {
            if (x >= 0 && x < numCols && y >= 0 && y < numRows) {
                field[y][x].isAlive = true;
            }
        }
    }

    inputFile.close();
    std::cout << "Game state loaded from file '" << filename << "'" << std::endl;
}

void Game::saveToFile(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file '" << filename << "'" << std::endl;
        return;
    }

    outputFile << "#Life 1.06" << std::endl;
    outputFile << "#N " << gameName << std::endl;
    outputFile << "#R B";

    for (int i : birthRules) {
        outputFile << i;
    }
    outputFile << "/S";

    for (int i : survivalRules) {
        outputFile << i;
    }
    outputFile << std::endl;

    outputFile << "#S " << numRows << " " << numCols << std::endl;

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            if (field[i][j].isAlive) {
                outputFile << j << " " << i << std::endl;
            }
        }
    }

    outputFile.close();
    std::cout << "Game state saved to file '" << filename << "'" << std::endl;
}

void Game::calculateNextState() {
    std::vector<std::vector<Cell>> nextState(numRows, std::vector<Cell>(numCols));

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            int neighbors = countNeighbors(row, col);

            if (field[row][col].isAlive) {
                // Если клетка жива, она должна выжить только если количество соседей соответствует survivalRules
                nextState[row][col].isAlive = std::find(survivalRules.begin(), survivalRules.end(), neighbors) != survivalRules.end();
            } else {
                // Если клетка мертва, она должна воскреснуть только если количество соседей соответствует birthRules
                nextState[row][col].isAlive = std::find(birthRules.begin(), birthRules.end(), neighbors) != birthRules.end();
            }
        }
    }

    // Переносим новое состояние в поле
    field = std::move(nextState);
}



int Game::countNeighbors(int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int neighborRow = (row + i + numRows) % numRows;
            int neighborCol = (col + j + numCols) % numCols;
            if (field[neighborRow][neighborCol].isAlive) {
                ++count;
            }
        }
    }
    return count;
}

void Game::printState() {
    system("clear");
    std::cout << "#Life v1.0" << std::endl;
    std::cout << "#N " << gameName << std::endl;
    std::cout << "#R B";

    for (int i : birthRules) {
        std::cout << i;
    }
    std::cout << "/S";

    for (int i : survivalRules) {
        std::cout << i;
    }
    std::cout << std::endl;

    std::cout << curIteration << std::endl;
    std::cout << "┌" << std::string(field[0].size(), '-') << "┐" << std::endl;

    for (const auto& row : field) {
        std::cout << "│";
        for (const auto& cell : row) {
            std::cout << (cell.isAlive ? 'X' : ' ');
        }
        std::cout << "│" << std::endl;
    }

    std::cout << "└" << std::string(field[0].size(), '-') << "┘" << std::endl;
}


void Game::loadRulesFromFile(const std::string& filename) {
    std::ifstream file("rules/" + filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open rules file '" << filename << "'" << std::endl;
        return;
    }

    std::string ruleLine;
    if (std::getline(file, ruleLine)) {
        parseRules(ruleLine);
        std::cout << "Loaded rules: " << ruleLine << std::endl;
    } else {
        std::cerr << "Error: Empty rules file!" << std::endl;
    }

    file.close();
}

void Game::generateRandomRules() {
    const std::string rulesDir = "rules";
    std::vector<std::string> ruleFiles;

    if (fs::exists(rulesDir) && fs::is_directory(rulesDir)) {
        for (const auto& entry : fs::directory_iterator(rulesDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                ruleFiles.push_back(entry.path().filename().string());
            }
        }
    } else {
        std::cerr << "Rules directory not found!" << std::endl;
        return;
    }

    if (ruleFiles.empty()) {
        std::cerr << "No rules files found in the rules directory!" << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, ruleFiles.size() - 1);

    std::string selectedRuleFile = ruleFiles[dis(gen)];
    loadRulesFromFile(selectedRuleFile);
    std::cout << "Loaded random rule: " << selectedRuleFile << std::endl;
}
