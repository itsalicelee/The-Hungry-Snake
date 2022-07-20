#include <random>

#include <iostream>
#include <functional>

#include "Game.h"
#include "CustomController.h"
#include "ISnakeController.h"
#include "DirectionType.h"
#include "Position.h"
#include "ConstantDirectionControllerA.h"
#include "ConstantDirectionControllerB.h"
#include "StraightForwardController.h"

#include "GUI.h"

Position CreateSafePosition(const Game& game, std::mt19937& rand, size_t margin) {
    const auto space = Game::kSnakeRadius * margin;
    return
            Position{
                    static_cast<float>(rand() % (game.FieldWidth() - 2 * space) + space),
                    static_cast<float>(rand() % (game.FieldHeight() - 2 * space) + space),
            };
}

Position CreateSafeFoodPosition(const Game& game, std::mt19937& rand) {
    return CreateSafePosition(game, rand, 2);
}

Position CreateSafeSnakePosition(const Game& game, std::mt19937& rand) {
    return CreateSafePosition(game, rand, 5);
}

void TestA0(); // Use Player Controller
void TestA1(); // Use Custom Controller
void TestA2(); // Use Custom Controller (Fast-forward)

void TestB0(); // Use Player Controller
void TestB1(); // Use Custom Controller
void TestB2(); // Use Custom Controller (Fast-forward)

void TestC0(); // Use Player Controller
void TestC1(); // Use Custom Controller
void TestC2(); // Use Custom Controller (Fast-forward)

enum class Mode {
    kPlayerController,
    kCustomController,
    kFastForwardingCustomController,
};

int main() {
    char ch;
    std::cout << "Type (A, B or C): "; 
    std::cin >> ch;
    std::cout << "Mode (0, 1 or 2): ";
    int mode;
    std::cin >> mode;
    void (*a[])() = { TestA0, TestA1, TestA2};
    void (*b[])() = { TestB0, TestB1, TestB2};
    void (*c[])() = { TestC0, TestC1, TestC2};

    if (ch == 'A') {
        a[mode]();
    } else if (ch == 'B') {
        b[mode]();
    } else if (ch == 'C') {
        c[mode]();
    }
}

void RunTest(std::function<Game(std::shared_ptr<ISnakeController>)> setup, Mode mode) {
    switch (mode)
    {
        case Mode::kPlayerController: {
#ifdef ENABLE_GUI
            auto controller = std::make_shared<PlayerController>();
            Game game = setup(controller);
            std::cout << "Scores: " << StartPlayerMode(game, controller) << std::endl;
#else
            std::cout << "GUI is not enabled." << std::endl;
#endif
            break;
        }
        case Mode::kCustomController: {
#ifdef ENABLE_GUI
            auto controller = std::make_shared<CustomController>();
            Game game = setup(controller);
            std::cout << "Scores: " << StartCustomMode(game, controller) << std::endl;
#else
            std::cout << "GUI is not enabled." << std::endl;
#endif
            break;
        }
        case Mode::kFastForwardingCustomController: {
            Game game = setup(std::make_shared<CustomController>());
            game.RunUntilTheEnd();
            std::cout << "Scores: " << game.Scores() << std::endl;
            break;
        }
    }
}

void TestA(Mode mode) {
    RunTest(
            [](const std::shared_ptr<ISnakeController> &controller) {
                Game game(25, 25, 20000);
                std::mt19937 rand{20};

                for (int i = 1; i <= 500; ++i) {
                    game.AddFood(CreateSafeFoodPosition(game, rand));
                }

                game.AddSnake(1, CreateSafeSnakePosition(game, rand), 0, 10);
                game.AddController(1, controller);
                return game;
            },
            mode);
}

void TestB(Mode mode) {
    RunTest(
            [](const std::shared_ptr<ISnakeController> &controller) {
		 		Game game(25, 25, 20000); 
		        std::mt19937 rand{25};
		        
		        const auto margin = Game::kSnakeRadius * 5;
		        float max_x = game.FieldWidth() - margin;
		        float max_y = game.FieldHeight() - margin;
		        float min_x = 0 + margin;
		        float min_y = 0 + margin;
		        float center_x = 0 + game.FieldWidth() / 2;
		        float center_y = 0 + game.FieldHeight() / 2;
		        Position pos{0, 0};
		
		        for (int i = 1; i <= 500; ++i) {
		            game.AddFood(CreateSafeFoodPosition(game, rand));
		        }
		
		        game.AddSnake(1, CreateSafeSnakePosition(game, rand), 0, 10);
		        game.AddController(1, controller);
		
				pos.x = min_x + 100;
		        pos.y = min_y + 100;
		        game.AddSnake(2, pos, 0, 15);
		        game.AddController(
			        2,
			        std::make_shared<ConstantDirectionControllerB>());
		
				pos.x = min_x + 200;
		        pos.y = max_y;
		        game.AddSnake(3, pos, 0, 15);
		        game.AddController(
			        3,
			        std::make_shared<ConstantDirectionControllerA>(DirectionType::kLeft));
		
				pos.x = max_x - 150;
		        pos.y = min_y + 150;
		        game.AddSnake(4, pos, 0, 10);
		        game.AddController(
			        4,
			        std::make_shared<ConstantDirectionControllerA>(DirectionType::kRight));
		
				pos.x = max_x;
		        pos.y = max_y;
		        game.AddSnake(5, pos, 0, 15);
		        game.AddController(
			        5,
			        std::make_shared<ConstantDirectionControllerA>(DirectionType::kLeft));
		
				pos.x = center_x;
		        pos.y = center_y;
		        game.AddSnake(6, pos, 0, 25);
		        game.AddController(
			        6,
			        std::make_shared<ConstantDirectionControllerB>());
		
		        return game;
            },
            mode);
}

void TestC(Mode mode) {
    RunTest(
            [](const std::shared_ptr<ISnakeController> &controller) {
		 		Game game(25, 25, 50000);
		        std::mt19937 rand{ 25 };
		
		        for (int i = 1; i <= 500; ++i) {
		            game.AddFood(CreateSafeFoodPosition(game, rand));
		        }
		
		        game.AddSnake(1, CreateSafeSnakePosition(game, rand), 0, 10);
		        game.AddController(1, controller);
		
		        const auto margin = Game::kSnakeRadius * 7;
		        float max_x = game.FieldWidth() - margin;
		        float max_y = game.FieldHeight() - margin;
		        float min_x = 0 + margin;
		        float min_y = 0 + margin;
		
		        Position pos{ 0, 0 };
		        // Top Left - Go right
		        pos.x = min_x + (rand() % 8) * 50; // random shift right
		        pos.y = min_y;
		
		        game.AddSnake(2, pos, 0, 15);
		        game.AddController(
		            2,
		            std::make_shared<StraightForwardController>(0, DirectionType::kLeft));
		
		        // Bottom Left - Go up
		        pos.x = min_x;
		        pos.y = max_y - (rand() % 5) * 50; // random shift up
		
		        game.AddSnake(3, pos, 270, 15);
		        game.AddController(
		            3,
		            std::make_shared<StraightForwardController>(270, DirectionType::kRight));
		
		        // Bottom Right - Go left
		        pos.x = max_x - (rand() % 9) * 30; // random shift left
		        pos.y = max_y;
		
		        game.AddSnake(4, pos, 180, 15);
		        game.AddController(
		            4,
		            std::make_shared<StraightForwardController>(180, DirectionType::kLeft));
		
		        // Top Right - Go down
		        pos.x = max_x;
		        pos.y = min_y + (rand() % 4) * 60; // random shift down
		
		        game.AddSnake(5, pos, 90, 15);
		        game.AddController(
		            5,
		            std::make_shared<StraightForwardController>(90, DirectionType::kRight));
		
		        return game;
            },
            mode);
}

void TestA0() { TestA(Mode::kPlayerController); }
void TestA1() { TestA(Mode::kCustomController); }
void TestA2() { TestA(Mode::kFastForwardingCustomController); }

void TestB0() { TestB(Mode::kPlayerController); }
void TestB1() { TestB(Mode::kCustomController); }
void TestB2() { TestB(Mode::kFastForwardingCustomController); }

void TestC0() { TestC(Mode::kPlayerController); }
void TestC1() { TestC(Mode::kCustomController); }
void TestC2() { TestC(Mode::kFastForwardingCustomController); }
