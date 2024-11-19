#include <gtest/gtest.h>
#include "GameOfLife.h"
#include <string>

TEST(GameOfLifeTest, func_endsWithTest) {
    std::string str1 = "asd.lif";
    std::string str2 = "asd.life";
    EXPECT_FALSE(endsWith(str1, ".life"));
    EXPECT_TRUE(endsWith(str1, ".lif"));
    EXPECT_FALSE(endsWith(str2, ".lif"));
    EXPECT_TRUE(endsWith(str2, ".life"));
}

TEST(GameOfLifeTest, func_isIntegerTest) {
    EXPECT_FALSE(isInteger("123a"));
    EXPECT_FALSE(isInteger("b123"));
    EXPECT_TRUE(isInteger("123"));
    EXPECT_TRUE(isInteger("1231233"));
}

TEST(GameOfLifeTest, GameConstructorTest) {
    Game game("Test Game", 2, 123);
    EXPECT_EQ(game.numCols, 123);
    EXPECT_EQ(game.numRows, 2);
    EXPECT_EQ(game.gameName, "Test Game");
    EXPECT_EQ(game.curIteration, 1);
    EXPECT_FALSE(game.survivalRules.empty());
    EXPECT_FALSE(game.birthRules.empty());
}

TEST(GameOfLifeTest, func_CalculateNextStateTest1) {
    Game game("Test Game", 2, 2);
    game.field[0][0].isAlive = true;
    game.field[0][1].isAlive = true;
    game.field[1][1].isAlive = true;
    game.calculateNextState();
    EXPECT_FALSE(game.field[0][0].isAlive);
    EXPECT_FALSE(game.field[0][1].isAlive);
    EXPECT_FALSE(game.field[1][0].isAlive);
    EXPECT_FALSE(game.field[1][1].isAlive);
}

TEST(GameOfLifeTest, func_CalculateNextStateTest2) {
    Game game("Test Game", 5, 5);
    game.field[0][0].isAlive = true;
    game.field[1][0].isAlive = true;
    game.field[0][1].isAlive = true;
    game.field[1][0].isAlive = true;
    game.calculateNextState();
    EXPECT_TRUE(game.field[0][0].isAlive);
    EXPECT_TRUE(game.field[0][1].isAlive);
    EXPECT_TRUE(game.field[1][0].isAlive);
    EXPECT_TRUE(game.field[1][1].isAlive);
}

TEST(GameTest, func_CountNeighborsTest) {
    Game game("Test Game", 5, 5);
    game.field[0][0].isAlive = true;
    game.field[0][1].isAlive = true;
    game.field[1][1].isAlive = true;
    int count = game.countNeighbors(1, 1);
    ASSERT_EQ(count, 2);
}

TEST(GameTest, func_ParseRulesTest) {
    Game game("Test Game", 10, 10);
    std::string ruleString = "B3/S23";
    game.parseRules(ruleString);
    std::vector<int> expectedBirthRules = {3};
    std::vector<int> expectedSurvivalRules = {2, 3};
    ASSERT_EQ(game.birthRules, expectedBirthRules);
    ASSERT_EQ(game.survivalRules, expectedSurvivalRules);
}

TEST(GameTest, func_parseSizeTestValidInput) {
    Game game("Test Game", 0, 0);
    game.parseSize("10 20");
    EXPECT_EQ(game.numRows, 10);
    EXPECT_EQ(game.numCols, 20);
}

TEST(GameTest, func_parseSizeTestInvalidInput) {
    Game game("Test Game", 0, 0);
    game.parseSize("invalid input");
    EXPECT_EQ(game.numRows, 0);
    EXPECT_EQ(game.numCols, 0);
}

TEST(GameTest, func_parseCellTestValidInput) {
    Game game("Test Game", 10, 20);
    game.parseCell("5 5");
    EXPECT_TRUE(game.field[5][5].isAlive);
}

TEST(GameTest, func_parseCellTestInvalidInput) {
    Game game("Test Game", 10, 20);
    game.parseCell("invalid input");
    EXPECT_FALSE(game.field[0][0].isAlive);
}

int main(int argc, char* argv[]) {
    std::cout << "Running tests..." << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}