#include <GameOfLife.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cctype>

// Функция для проверки, заканчивается ли строка на заданный суффикс
bool endsWith(const std::string &str, const std::string &suffix) {
    if (str.length() < suffix.length()) { // Если длина строки меньше длины суффикса, возвращаем false
        return false;
    }
    return str.substr(str.length() - suffix.length()) == suffix; // Сравниваем суффикс с концом строки
}

// Функция для проверки, является ли строка целым числом
bool isInteger(const std::string &str) {
    for (char c : str) { // Проходим по каждому символу строки
        if (!std::isdigit(c)) { // Если символ не является цифрой, возвращаем false
            return false;
        }
    }
    return true; // Если все символы являются цифрами, возвращаем true
}

// Метод для чтения состояния игры из файла
void Game::readFromFile(std::string filename) {
    std::ifstream inputFile(filename); // Открываем файл для чтения
    std::string line;

    if (inputFile.is_open()) { // Проверяем, открыт ли файл
        std::getline(inputFile, line); // Пропускаем первую строку
        std::getline(inputFile, line); // Пропускаем вторую строку
        gameName = line.substr(3); // Считываем название игры
        std::getline(inputFile, line);
        parseRules(line.substr(3)); // Считываем правила
        std::getline(inputFile, line);
        parseSize(line.substr(3)); // Считываем размер поля

        while (std::getline(inputFile, line)) {
            parseCell(line); // Считываем координаты живых клеток
        }

        inputFile.close(); // Закрываем файл
    } else {
        std::cerr << "Unable to open input file!" << std::endl; // Выводим сообщение об ошибке, если файл не открыт
    }
}

// Метод для генерации "пульсара" на поле
void Game::generatePulsar() {
    std::vector<std::string> Pulsar = {
        "...OOO...OOO...",
        "...............",
        ".O....O.O....O.",
        ".O....O.O....O.",
        ".O....O.O....O.",
        "...OOO...OOO...",
        "...............",
        "...OOO...OOO...",
        ".O....O.O....O.",
        ".O....O.O....O.",
        ".O....O.O....O.",
        "...............",
        "...OOO...OOO..."
    };

    for (int i = 0; i < Pulsar.size(); i++) {
        for (int j = 0; j < Pulsar[0].size(); j++) {
            if (Pulsar[i][j] == 'O') {
                field[2 + i][2 + j].isAlive = true; // Устанавливаем живые клетки на поле
            }
        }
    }
}

// Метод для генерации "пушки Госпера" на поле
void Game::generateGliderGun() {
    std::vector<std::string> gosperGliderGun = {
        "........................O...........",
        "......................O.O...........",
        "............OO......OO............OO",
        "...........O...O....OO............OO",
        "OO........O.....O...OO..............",
        "OO........O...O.OO....O.O...........",
        "..........O.....O.......O...........",
        "...........O...O....................",
        "............OO......................"
    };

    for (int i = 0; i < gosperGliderGun.size(); i++) {
        for (int j = 0; j < gosperGliderGun[0].size(); j++) {
            if (gosperGliderGun[i][j] == 'O') {
                field[2 + i][2 + j].isAlive = true; // Устанавливаем живые клетки на поле
            }
        }
    }
}

// Метод для генерации "планера" на поле
void Game::generateGlider() {
    field[0][1].isAlive = true;
    field[1][2].isAlive = true;
    field[2][0].isAlive = true;
    field[2][1].isAlive = true;
    field[2][2].isAlive = true; // Устанавливаем живые клетки на поле
}

// Метод для генерации "мигающего" паттерна на поле
void Game::generateBlinker() {
    field[2][2].isAlive = true;
    field[2][3].isAlive = true;
    field[2][4].isAlive = true; // Устанавливаем живые клетки на поле
}

// Метод для генерации "пентадекалона" на поле
void Game::generatePentaDecathlon() {
    std::vector<std::string> PentaDecathlon = {
        ".O.",
        ".O.",
        "O.O",
        ".O.",
        ".O.",
        ".O.",
        ".O.",
        "O.O",
        ".O.",
        ".O."
    };

    for (int i = 0; i < PentaDecathlon.size(); i++) {
        for (int j = 0; j < PentaDecathlon[0].size(); j++) {
            if (PentaDecathlon[i][j] == 'O') {
                field[7 + i][21 + j].isAlive = true; // Устанавливаем живые клетки на поле
            }
        }
    }
}

// Метод для генерации случайного состояния игры
void Game::generateRandomState() {
    birthRules = {3};
    survivalRules = {2, 3};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);
    int variant = dis(gen);

    switch (variant) {
        case 0: generateGlider(); break;
        case 1: generateGliderGun(); break;
        case 2: generatePulsar(); break;
        case 3: generateBlinker(); break;
        case 4: generatePentaDecathlon(); break;
    }
}

// Метод для вывода текущего состояния игры на экран
void Game::printState() {
    system("clear"); // Очистка экрана
    std::cout << "#Life 1.06" << std::endl;
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

    for (const auto &row : field) {
        std::cout << "│";
        for (const auto &cell : row) {
            std::cout << (cell.isAlive ? 'X' : ' ');
        }
        std::cout << "│" << std::endl;
    }

    std::cout << "└" << std::string(field[0].size(), '-') << "┘" << std::endl;
}

// Метод для сохранения текущего состояния игры в файл
void Game::saveToFile(std::string filename) {
    std::ofstream outputFile(filename);

    if (outputFile.is_open()) {
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

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (field[i][j].isAlive) {
                    outputFile << j << " " << i << std::endl;
                }
            }
        }

        outputFile.close();
        std::cout << "Game saved to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open output file!" << std::endl;
    }
}
