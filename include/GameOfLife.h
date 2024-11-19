#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <thread>
#include <filesystem>
#include "Cell.h" // Подключаем заголовок Cell.h

// Функция для проверки, заканчивается ли строка на заданный суффикс
bool endsWith(const std::string& str, const std::string& suffix);

// Класс игры "Жизнь"
class Game {
public:
    std::string gameName;
    std::vector<std::vector<Cell>> field;
    std::vector<int> birthRules = {3};
    std::vector<int> survivalRules = {2, 3};
    int numRows = 0;
    int numCols = 0;
    int curIteration = 1;

    // Конструктор
    Game(std::string name, int rows, int cols)
        : gameName(std::move(name)), numRows(rows), numCols(cols) {
        field.resize(numRows, std::vector<Cell>(numCols));
    }

    // Методы
    void readFromFile(const std::string& filename);
    void generateRandomState();
    void printState();
    void saveToFile(const std::string& filename);
    void calculateNextState();
    int countNeighbors(int row, int col);
    void parseRules(const std::string& ruleString);
    void parseSize(const std::string& sizeString);
    void parseCell(const std::string& cellString);
    
    // Загрузить шаблон с координатами
    void loadTemplate(const std::string& filename, int startX, int startY);

    // Загрузить правила из файла
    void loadRulesFromFile(const std::string& filename);

    // Сгенерировать случайные правила
    void generateRandomRules();

    // Функция для проверки, является ли строка целым числом
    bool isInteger(const std::string& str);

};

#endif // GAMEOFLIFE_H
