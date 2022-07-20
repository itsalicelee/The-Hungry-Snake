#include "Config.h"

#ifdef ENABLE_GUI

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include "PlayerController.h"

void DrawSnake(
        const Snake& snake,
        sf::CircleShape& bodyShape,
        sf::CircleShape& eye,
        sf::CircleShape& eyeBall,
        sf::Text textLabel,
        const sf::Vector2f& worldCenter,
        const Position& refPosition,
        sf::RenderWindow& window);

int SampleTriangleWave(int x, int a, int p);

void DrawField(sf::RenderWindow &window, const sf::Vector2f &worldCenter, const Game &game);

sf::Color GetSnakeColor(size_t id);

int StartCustomMode(Game& game, const std::shared_ptr<ISnakeController>& playerController)
{
    int playbackSpeed = 1;

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "DSAP Final Project", sf::Style::Titlebar | sf::Style::Close);

    sf::CircleShape foodShape(Game::kFoodRadius);
    foodShape.setOrigin(foodShape.getRadius(), foodShape.getRadius());
    foodShape.setFillColor(sf::Color(255, 255, 255));

    sf::CircleShape eyeShape(15);
    eyeShape.setOrigin(eyeShape.getRadius(), eyeShape.getRadius());
    eyeShape.setFillColor(sf::Color(255, 255, 255));

    sf::CircleShape eyeBallShape(5);
    eyeBallShape.setOrigin(eyeBallShape.getRadius(), eyeBallShape.getRadius());
    eyeBallShape.setFillColor(sf::Color(0, 0, 0));

    sf::CircleShape snakeShape;

    sf::Vector2f worldCenter(
            static_cast<float>(window.getSize().x)/2.f,
            static_cast<float>(window.getSize().y)/2.f);

    sf::Clock clock;

    sf::Font font;

    font.loadFromFile("arial.ttf");

    sf::Text textLabel;
    textLabel.setFont(font);
    textLabel.setCharacterSize(20);
    textLabel.setOrigin(15, 15);

    sf::Text scoresText;
    scoresText.setFont(font);
    scoresText.setCharacterSize(40);
    scoresText.setPosition(10, 50);

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(40);
    timeText.setPosition(10, 0);

    float elapsedTime = 0;

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Key::Num1) {
                    playbackSpeed = 1;
                }
                if (event.key.code == sf::Keyboard::Key::Num2) {
                    playbackSpeed = 2;
                }
                if (event.key.code == sf::Keyboard::Key::Num3) {
                    playbackSpeed = 4;
                }
                if (event.key.code == sf::Keyboard::Key::Num4) {
                    playbackSpeed = 8;
                }
                if (event.key.code == sf::Keyboard::Key::Num5) {
                    playbackSpeed = 16;
                }
            }
        }

        elapsedTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        while (elapsedTime > 1.f / 60.f) {
            elapsedTime -= 1.f / 60.f;
            for (size_t i = 0; i < playbackSpeed; ++i) {
                game.Step();
            }
        }

        if (game.IsOver()) {
            return game.Scores();
        }

        window.clear();

        DrawField(window, worldCenter, game);

        game.TraverseFoods([&](const Position& food) {
            foodShape.setPosition(
                    worldCenter.x + food.x - game.Center().x,
                    worldCenter.y + food.y - game.Center().y);
            window.draw(foodShape);
        });

        game.TraverseSnakes([&](const Snake& snake) {
            DrawSnake(
                    snake, snakeShape, eyeShape, eyeBallShape,
                    textLabel,
                    worldCenter, game.Center(), window);
        });

        timeText.setString("Time: " + std::to_string(game.Time()));
        window.draw(timeText);

        scoresText.setString("Scores: " + std::to_string(game.Scores()));
        window.draw(scoresText);

        window.display();
    }
    return -1;
}


int StartPlayerMode(Game& game, const std::shared_ptr<PlayerController>& playerController)
{
    int playbackSpeed = 1;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "DSAP Final Project", sf::Style::Titlebar | sf::Style::Close, settings);

    sf::CircleShape foodShape(Game::kFoodRadius);
    foodShape.setOrigin(foodShape.getRadius(), foodShape.getRadius());
    foodShape.setFillColor(sf::Color(255, 255, 255));

    sf::CircleShape eyeShape(15);
    eyeShape.setOrigin(eyeShape.getRadius(), eyeShape.getRadius());
    eyeShape.setFillColor(sf::Color(255, 255, 255));

    sf::CircleShape eyeBallShape(5);
    eyeBallShape.setOrigin(eyeBallShape.getRadius(), eyeBallShape.getRadius());
    eyeBallShape.setFillColor(sf::Color(0, 0, 0));

    sf::CircleShape snakeShape;

    sf::Vector2f worldCenter(
            static_cast<float>(window.getSize().x)/2.f,
            static_cast<float>(window.getSize().y)/2.f);

    sf::Clock clock;

    sf::Font font;

    font.loadFromFile("arial.ttf");

    sf::Text textLabel;
    textLabel.setFont(font);
    textLabel.setCharacterSize(20);
    textLabel.setOrigin(15, 15);

    sf::Text scoresText;
    scoresText.setFont(font);
    scoresText.setCharacterSize(40);
    scoresText.setPosition(10, 50);

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(40);
    timeText.setPosition(10, 0);

    float elapsedTime = 0;

    while (window.isOpen())
    {
        sf::Event event{};
        playerController->KeepForward();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Key::Num1) {
                    playbackSpeed = 1;
                }
                if (event.key.code == sf::Keyboard::Key::Num2) {
                    playbackSpeed = 2;
                }
                if (event.key.code == sf::Keyboard::Key::Num3) {
                    playbackSpeed = 4;
                }
                if (event.key.code == sf::Keyboard::Key::Num4) {
                    playbackSpeed = 8;
                }
                if (event.key.code == sf::Keyboard::Key::Num5) {
                    playbackSpeed = 16;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            playerController->TurnRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            playerController->TurnLeft();
        }

        elapsedTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        while (elapsedTime > 1.f / 60.f) {
            elapsedTime -= 1.f / 60.f;
            for (size_t i = 0; i < playbackSpeed; ++i) {
                game.Step();
            }
        }

        if (game.IsOver()) {
            return game.Scores();
        }

        window.clear();

        DrawField(window, worldCenter, game);

        game.TraverseFoods([&](const Position& food) {
            foodShape.setPosition(
                    worldCenter.x + food.x - game.Center().x,
                    worldCenter.y + food.y - game.Center().y);
            window.draw(foodShape);
        });

        game.TraverseSnakes([&](const Snake& snake) {
            DrawSnake(
                    snake, snakeShape, eyeShape, eyeBallShape,
                    textLabel,
                    worldCenter, game.Center(), window);
        });

        timeText.setString("Time: " + std::to_string(game.Time()));
        window.draw(timeText);

        scoresText.setString("Scores: " + std::to_string(game.Scores()));
        window.draw(scoresText);

        window.display();
    }
    return -1;
}

#include <iostream>
#include "ISnakeController.h"

void DrawField(sf::RenderWindow &window, const sf::Vector2f &worldCenter, const Game &game) {
    sf::VertexArray lines(sf::Lines);
    sf::Vector2f gameCenter{game.Center().x, game.Center().y};
    const int space = static_cast<int>(Game::kCellSize);
    const sf::Color& lineColor = sf::Color(150, 150, 150);
    for (int x = 0; x <= game.NumberOfCols(); x++) {
        auto px =  static_cast<float>(x * space);
        lines.append(
                sf::Vertex(
                        sf::Vector2f(
                                px + worldCenter.x - gameCenter.x,
                                worldCenter.y - gameCenter.y),
                        lineColor));
        lines.append(
                sf::Vertex(
                        sf::Vector2f(
                                px + worldCenter.x - gameCenter.x,
                                static_cast<float>(game.FieldHeight()) + worldCenter.y- gameCenter.y),
                        lineColor));
    }
    for (int y = 0; y <= game.NumberOfRows(); y++) {
        auto py =  static_cast<float>(y * space);
        lines.append(
                sf::Vertex(
                        sf::Vector2f(
                                worldCenter.x- gameCenter.x,
                                py + worldCenter.y- gameCenter.y),
                        lineColor));
        lines.append(
                sf::Vertex(
                        sf::Vector2f(
                                static_cast<float>(game.FieldWidth()) + worldCenter.x- gameCenter.x,
                                py + worldCenter.y- gameCenter.y),
                        lineColor));
    }
    window.draw(lines);
}

void DrawSnake(
        const Snake& snake,
        sf::CircleShape& bodyShape,
        sf::CircleShape& eye,
        sf::CircleShape& eyeBall,
        sf::Text textLabel,
        const sf::Vector2f& worldCenter,
        const Position& refPosition,
        sf::RenderWindow& window) {
    bodyShape.setRadius(Game::kSnakeRadius + 5);
    bodyShape.setOrigin(bodyShape.getRadius(), bodyShape.getRadius());

    int i = 0;
    for (const auto& s : snake.Body()) {
        ++i;
        if (i % 20 != 1) continue;
        Position d = s - refPosition;
        bodyShape.setPosition(worldCenter + sf::Vector2f(d.x, d.y));
        const sf::Color& bodyColor = GetSnakeColor(snake.Id());
        sf::Color bodyColor2{bodyColor.r, bodyColor.g, bodyColor.b};
        int added = SampleTriangleWave(i, 80, 240);

        bodyColor2.r = std::min(255, std::max(0, bodyColor2.r + added));
        bodyColor2.g = std::min(255, std::max(0, bodyColor2.g + added));
        bodyColor2.b = std::min(255, std::max(0, bodyColor2.b + added));

        bodyShape.setFillColor(bodyColor2);
        window.draw(bodyShape);
    }

    Position d = snake.Head() - refPosition;
    sf::Vector2f d2 = sf::Vector2f(d.x, d.y);

    float direction = snake.Direction();
    {
        eye.setPosition(worldCenter + d2 +
                        sf::Vector2f(
                                static_cast<float>(25. * std::cos((direction - 40) / 180.f * M_PI)),
                                static_cast<float>(25. * std::sin((direction - 40) / 180.f * M_PI))));
        window.draw(eye);

        eyeBall.setPosition(worldCenter + d2 +
                            sf::Vector2f(
                                    static_cast<float>(30 * std::cos((direction - 40 + (std::rand() % 10 / 4.-1.5)) / 180.f * M_PI)),
                                    static_cast<float>(30 * std::sin((direction - 40 + (std::rand() % 10 / 4.-1.5)) / 180.f * M_PI))));
        window.draw(eyeBall);
    }

    {
        eye.setPosition(worldCenter + d2 +
                        sf::Vector2f(static_cast<float>(25 * std::cos((direction + 40.) / 180.f * M_PI)),
                                     static_cast<float>(25 * std::sin((direction + 40.) / 180.f * M_PI))));
        window.draw(eye);

        eyeBall.setPosition(worldCenter + d2 +
                            sf::Vector2f(30 * std::cos((direction + 40 + (std::rand() % 10 / 4.-1.5)) / 180.f * M_PI),
                                         30 * std::sin((direction + 40 + (std::rand() % 10 / 4.-1.5)) / 180.f * M_PI)));
        window.draw(eyeBall);
    }

    {
        textLabel.setString(std::to_string(snake.Id()));
        const auto& forward = snake.Forward();
        const auto& bounds = textLabel.getLocalBounds();
        textLabel.setOrigin(bounds.width/2 + 5, bounds.height/2);
        textLabel.setRotation(direction - 90);
        textLabel.setPosition(worldCenter + d2 - 10.0f * sf::Vector2f(forward.x, forward.y));
        window.draw(textLabel);
    }
}

sf::Color GetSnakeColor(size_t id) {
    unsigned char primaryColor = (id + 17) * 97 % 150 + 60;
    unsigned char secondaryColor = (id + 13) * 29  % 150 + 60;

    return sf::Color{
            primaryColor, secondaryColor,
            static_cast<unsigned char>(
                    std::max(0, 150 - std::max(primaryColor, secondaryColor)) + 60)};
}

int SampleTriangleWave(int x, int a, int p) {
    return 4*a/p * std::abs((((x-p/4)%p)+p)%p - p/2) - a;
}
#endif
