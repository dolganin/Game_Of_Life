#include <GameOfLife.h>

int main(int argc, char *argv[])
{
    //std::cout<<"LOL"<<std::endl;
    std::string inputFilename;  // Имя входного файла
    std::string outputFilename; // Имя выходного файла
    int numIterations = 0;      // Количество итераций
    int mode = 0;               // Режим работы программы: 0 - без аргументов, 1 - только входной файл, 2 - случайное состояние, 3 - входной файл, итерации и выходной файл
    if (argc == 1)
    {
        mode = 2; // Случайное состояние, если нет аргументов
    }
    else if (argc == 2)
    {
        mode = 1; // Чтение из файла, если один аргумент
        inputFilename = argv[1];
    }
    else
    {
        inputFilename = argv[1];
        for (int i = 2; i < argc; i++)
        {
            if (std::strcmp(argv[i], "-i") == 0)
            { // Обработка флага -i для указания количества итераций
                if (i + 1 < argc)
                {
                    numIterations = std::atoi(argv[i + 1]);
                    i++;
                }
                else
                {
                    std::cerr << "Invalid arguments!" << std::endl; // Выводим сообщение об ошибке, если аргументы некорректны
                    return 0;
                }
            }
            else if (std::strncmp(argv[i], "--iterations=", 13) == 0)
            { // Обработка флага --iterations= для указания количества итераций
                numIterations = std::atoi(argv[i] + 13);
            }
            else if (std::strcmp(argv[i], "-o") == 0)
            { // Обработка флага -o для указания выходного файла
                if (i + 1 < argc)
                {
                    outputFilename = argv[i + 1];
                    i++;
                }
                else
                {
                    std::cerr << "Invalid arguments!" << std::endl; // Выводим сообщение об ошибке, если аргументы некорректны
                    return 0;
                }
            }
            else if (std::strncmp(argv[i], "--output=", 9) == 0)
            { // Обработка флага --output= для указания выходного файла
                outputFilename = argv[i] + 9;
            }
            else
            {
                outputFilename = argv[i];
            }
        }
        if (numIterations > -1 && (endsWith(outputFilename, ".lif") || endsWith(outputFilename, ".life")) && (endsWith(inputFilename, ".lif") || endsWith(inputFilename, ".life")))
            mode = 3; // Устанавливаем режим 3, если указаны итерации, входной и выходной файлы
    }
    if (mode == 1 || mode == 3)
        if (!(endsWith(inputFilename, ".lif")) || endsWith(inputFilename, ".life"))
            std::cerr << "Invalid input filename!" << std::endl; // Выводим сообщение об ошибке, если имя входного файла некорректно
    Game game("My Game of Life", 25, 50);                        // Создаем объект игры с заданными размерами поля
    switch (mode)
    {
    case 1:
        game.readFromFile(inputFilename); // Читаем состояние игры из файла
        break;
    case 2:
        game.generateRandomState(); // Генерируем случайное начальное состояние
        break;
    case 3:
        game.readFromFile(inputFilename); // Читаем состояние игры из файла
        break;
    default:
        std::cerr << "Invalid mode!" << std::endl; // Выводим сообщение об ошибке, если режим некорректен
        return 0;
    }
    if (mode == 1 || mode == 3)
    {
        for (int i = 0; i < numIterations; i++)
            game.calculateNextState(); // Выполняем заданное количество итераций
        if (mode == 3)
            game.saveToFile(outputFilename); // Сохраняем состояние игры в файл
    }
    std::string command = "";
    std::string output_file = "";
    int num_iter = 0;
    while (true)
    {
        std::cin >> command; // Считываем команду от пользователя
        if (command == "dump")
        { // Команда для сохранения состояния игры в файл
            std::cin >> command;
            game.saveToFile(command);
        }
        else if (command == "exit")
        { // Команда для выхода из программы
            std::cout << "Bye bye!" << std::endl;
            return 0;
        }
        else if (command == "help")
        { // Команда для вывода справки
            std::cout << "dump <filename> - save game to the file 'filename'" << std::endl;
            std::cout << "exit - exit" << std::endl;
            std::cout << "help - help" << std::endl;
            std::cout << "t <n=1> (or tick <n=1>) - calculate n (by default n = 1) iterations and print game field" << std::endl;
        }
        else if (command == "t" || command == "tick")
        { // Команда для выполнения итераций и вывода состояния игры
            std::cin >> command;
            if (command == "dump")
            {
                game.calculateNextState();
                std::cin >> command;
                game.saveToFile(command);
            }
            else if (command == "exit")
            {
                game.calculateNextState();
                return 0;
            }
            else if (command == "help")
            {
                game.calculateNextState();
                std::cout << "dump <filename> - save game to the file 'filename'" << std::endl;
                std::cout << "exit - exit" << std::endl;
                std::cout << "help - help" << std::endl;
                std::cout << "t <n=1> (or tick <n=1>) - calculate n (by default n = 1) iterations and print game field" << std::endl;
            }
            else if (isInteger(command))
            {
                num_iter = std::stoi(command);
                for (int i = 0; i < num_iter; ++i)
                    game.calculateNextState();
                game.printState();
            }
            else
                std::cerr << "Wrong command!" << std::endl; // Выводим сообщение об ошибке, если команда некорректна
        }
        else
            std::cerr << "Wrong command!" << std::endl; // Выводим сообщение об ошибке, если команда некорректна
    }
    return 0;
}