#include <gtest/gtest.h>
#include "include/GameOfLife.h"
#include <fstream>
#include <string>

// Тесты на функции endsWith и isInteger
TEST(GameOfLifeTest, func_endsWithTest) {
    EXPECT_TRUE(endsWith("example.life", ".life"));
    EXPECT_FALSE(endsWith("example.txt", ".life"));
}

TEST(GameOfLifeTest, func_isIntegerTest) {
        // Пример теста с объектом Game
    Game game("Test Game", 5, 5);

    EXPECT_TRUE(game.isInteger("42"));
    EXPECT_FALSE(game.isInteger("42a"));
    EXPECT_FALSE(game.isInteger(""));

}

// Тест конструктора
TEST(GameOfLifeTest, ConstructorTest) {
    Game game("Test Game", 10, 10);
    EXPECT_EQ(game.numRows, 10);
    EXPECT_EQ(game.numCols, 10);
    EXPECT_EQ(game.gameName, "Test Game");
    EXPECT_EQ(game.curIteration, 1);
    EXPECT_FALSE(game.survivalRules.empty());
    EXPECT_FALSE(game.birthRules.empty());
}

// Тест парсинга правил
TEST(GameOfLifeTest, ParseRulesTest) {
    Game game("Test Game", 10, 10);
    game.parseRules("B3/S23");
    EXPECT_EQ(game.birthRules, std::vector<int>({3}));
    EXPECT_EQ(game.survivalRules, std::vector<int>({2, 3}));
}

// Тест загрузки шаблона из файла
TEST(GameOfLifeTest, LoadTemplateTest) {
    Game game("Test Game", 10, 10);

    // Создаем тестовый шаблон
    std::ofstream templateFile("templates/test_template.txt");
    templateFile << "...\n.O.\n...";  // Пример простого шаблона
    templateFile.close();

    // Теперь файл существует, и можно его загрузить
    ASSERT_NO_THROW(game.loadTemplate("test_template.txt", 1, 1));
    EXPECT_TRUE(game.field[2][2].isAlive);
    EXPECT_FALSE(game.field[1][1].isAlive);

    // Удаляем тестовый файл
    std::remove("templates/test_template.txt");
}



// Тест загрузки несуществующего шаблона
TEST(GameOfLifeTest, LoadNonexistentTemplateTest) {
    Game game("Test Game", 10, 10);
    EXPECT_THROW(game.loadTemplate("templates/nonexistent.txt", 0, 0), std::runtime_error);
}

// Тест на корректность подсчета соседей
TEST(GameOfLifeTest, CountNeighborsTest) {
    Game game("Test Game", 5, 5);
    game.field[1][1].isAlive = true;
    game.field[1][2].isAlive = true;
    game.field[2][1].isAlive = true;
    EXPECT_EQ(game.countNeighbors(2, 2), 3);
    EXPECT_EQ(game.countNeighbors(1, 1), 2);
}

// Тест на выполнение следующего шага игры
TEST(GameOfLifeTest, CalculateNextStateTest) {
    Game game("Test Game", 3, 3);
    game.field[0][1].isAlive = true;
    game.field[1][1].isAlive = true;
    game.field[2][1].isAlive = true;

    game.calculateNextState();

    EXPECT_TRUE(game.field[1][0].isAlive); 
    EXPECT_TRUE(game.field[1][1].isAlive);  
    EXPECT_TRUE(game.field[1][2].isAlive);  
    EXPECT_TRUE(game.field[0][1].isAlive);  
    EXPECT_TRUE(game.field[2][1].isAlive);  
}


// Тест стабильного паттерна (блок)
TEST(GameOfLifeTest, StablePatternTest) {
    Game game("Test Game", 4, 4);
    game.field[1][1].isAlive = true;
    game.field[1][2].isAlive = true;
    game.field[2][1].isAlive = true;
    game.field[2][2].isAlive = true;

    game.calculateNextState();

    EXPECT_TRUE(game.field[1][1].isAlive);
    EXPECT_TRUE(game.field[1][2].isAlive);
    EXPECT_TRUE(game.field[2][1].isAlive);
    EXPECT_TRUE(game.field[2][2].isAlive);
}

// Тест осциллирующего паттерна (мигалка)
TEST(GameOfLifeTest, OscillatingPatternTest) {
    Game game("Test Game", 5, 5);
    game.field[2][1].isAlive = true;
    game.field[2][2].isAlive = true;
    game.field[2][3].isAlive = true;

    game.calculateNextState();

    EXPECT_TRUE(game.field[1][2].isAlive);
    EXPECT_TRUE(game.field[2][2].isAlive);
    EXPECT_TRUE(game.field[3][2].isAlive);

    game.calculateNextState();

    EXPECT_TRUE(game.field[2][1].isAlive);
    EXPECT_TRUE(game.field[2][2].isAlive);
    EXPECT_TRUE(game.field[2][3].isAlive);
}

// Тест сохранения и загрузки из файла
TEST(GameOfLifeTest, SaveAndLoadFileTest) {
    Game game("Test Game", 5, 5);
    game.field[1][1].isAlive = true;
    game.field[2][2].isAlive = true;

    game.saveToFile("test_save.life");

    Game loadedGame("Loaded Game", 5, 5);
    loadedGame.readFromFile("test_save.life");

    EXPECT_EQ(loadedGame.field[1][1].isAlive, true);
    EXPECT_EQ(loadedGame.field[2][2].isAlive, true);
    EXPECT_EQ(loadedGame.numRows, 5);
    EXPECT_EQ(loadedGame.numCols, 5);

    std::remove("test_save.life");
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
