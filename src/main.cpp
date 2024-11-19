#include <include/GameOfLife.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

// Функция для вывода исторической справки о Game of Life
void printHistory() {
    std::cout << "=== The Game of Life ===\n"
              << "The Game of Life is a cellular automaton devised by the British mathematician John Horton Conway in 1970.\n"
              << "It is a zero-player game, meaning its evolution is determined by its initial state, requiring no further input.\n"
              << "Despite its simple rules, the Game of Life is Turing complete and can simulate a universal constructor or computer.\n"
              << "Some famous patterns include:\n"
              << "  - Gliders: small structures that move diagonally across the board.\n"
              << "  - Oscillators: patterns that repeat in fixed cycles (e.g., Blinker, Pulsar).\n"
              << "  - Spaceships: patterns that translate across the board.\n"
              << "The game demonstrates how complex systems can emerge from simple rules.\n"
              << "=============================================\n";
}

int main(int argc, char* argv[]) {
    // Вывод исторической справки
    printHistory();

    std::string inputFilename;   // Имя входного файла
    std::string outputFilename;  // Имя выходного файла
    int numIterations = 0;       // Количество итераций
    int mode = 2;                // Режим работы программы: 2 - случайное состояние (по умолчанию)

    // Разбор аргументов командной строки
    if (argc > 1) {
        inputFilename = argv[1];
        if (argc == 2) {
            mode = 1; // Только входной файл
        } else {
            for (int i = 2; i < argc; ++i) {
                if (std::strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
                    numIterations = std::atoi(argv[++i]);
                } else if (std::strncmp(argv[i], "--iterations=", 13) == 0) {
                    numIterations = std::atoi(argv[i] + 13);
                } else if (std::strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                    outputFilename = argv[++i];
                } else if (std::strncmp(argv[i], "--output=", 9) == 0) {
                    outputFilename = argv[i] + 9;
                } else {
                    outputFilename = argv[i];
                }
            }
            if (!inputFilename.empty() && !outputFilename.empty() && numIterations > 0) {
                mode = 3; // Полный режим: входной файл, итерации, выходной файл
            }
        }
    }

    // Проверка входного файла, если он указан
    if ((mode == 1 || mode == 3) && !(endsWith(inputFilename, ".lif") || endsWith(inputFilename, ".life"))) {
        std::cerr << "Invalid input filename format!" << std::endl;
        return 1;
    }

    // Создаем объект игры с заданными размерами поля
    Game game("My Game of Life", 25, 50);

    // Инициализация игры в зависимости от режима
    switch (mode) {
        case 1:
        case 3:
            game.readFromFile(inputFilename);
            break;
        case 2:
            game.generateRandomState();
            break;
        default:
            std::cerr << "Invalid mode!" << std::endl;
            return 1;
    }

    // Выполняем итерации, если указано
    if ((mode == 1 || mode == 3) && numIterations > 0) {
        for (int i = 0; i < numIterations; ++i) {
            game.calculateNextState();
        }
        if (mode == 3) {
            game.saveToFile(outputFilename);
        }
    }

    // Основной цикл обработки пользовательских команд
    while (true) {
        std::string command;
        std::cout << "Enter command (type 'help' for available commands): ";
        std::cin >> command;

        if (command == "dump") {
            std::cin >> outputFilename;
            game.saveToFile(outputFilename);
            std::cout << "Game state saved to " << outputFilename << std::endl;
        } else if (command == "exit") {
            std::cout << "Thank you for exploring the Game of Life! Goodbye!" << std::endl;
            return 0;
        } else if (command == "help") {
            std::cout << "Available commands:\n"
                      << "  dump <filename>     - Save the current game state to the specified file.\n"
                      << "  exit                - Exit the program.\n"
                      << "  help                - Show this help message.\n"
                      << "  tick <n=1>          - Calculate n iterations (default: 1) and display the game field.\n"
                      << "  random              - Load a new random template.\n"
                      << "  template <name>     - Load a predefined template (e.g., glider, pulsar).\n";
        } else if (command == "tick") {
            int ticks = 1;
            if (std::cin.peek() != '\n') { // Проверяем, есть ли дополнительный аргумент
                std::cin >> ticks;
            }
            for (int i = 0; i < ticks; ++i) {
                game.calculateNextState();
            }
            game.printState();
        } else if (command == "random") {
            game.generateRandomState();
            game.printState();
        } else if (command == "template") {
        std::string templateName;
        std::cin >> templateName;

        // Генерация случайных координат для начала шаблона
        int startX = rand() % game.numCols;
        int startY = rand() % game.numRows;

        // Загружаем шаблон, передавая его имя и сгенерированные координаты
        game.loadTemplate(templateName, startX, startY);

        game.printState();
    }
 else {
            std::cerr << "Invalid command! Type 'help' for available commands." << std::endl;
        }
    }

    return 0;
}
