cmake_minimum_required(VERSION 3.10)
project(GameOfLifeProject)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Включаем директорию для заголовочных файлов
include_directories(${CMAKE_SOURCE_DIR})

# Создаем статическую библиотеку для игры
add_library(gameOfLife STATIC GameOfLife.cpp)

# Добавляем основной исполняемый файл для игры
add_executable(game Main.cpp)

# Линкуем основной исполняемый файл с библиотекой gameOfLife
target_link_libraries(game gameOfLife)

# Добавляем Google Test для тестов
find_package(GTest REQUIRED)

if(GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Добавляем тесты
    add_executable(runTests test/test_main.cpp)

    # Линкуем тесты с библиотекой Google Test, pthread и gameOfLife
    target_link_libraries(runTests ${GTEST_LIBRARIES} pthread gameOfLife)
else()
    message(WARNING "Google Test не найден. Тесты не будут собраны.")
endif()

# Упрощаем выбор целей для пользователей
add_custom_target(
    runGame ALL
    COMMAND ./game
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Запуск игры"
)

add_custom_target(
    testGame
    COMMAND ./runTests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Запуск тестов"
)



