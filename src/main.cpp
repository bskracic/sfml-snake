#include <SFML/Graphics.hpp>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

const int WINDOW_HEIGHT = 700;
const int WINDOW_WIDTH = 700;
const int BODY_CHUNK_HEIGHT = 25;
const int BODY_CHUNK_WIDTH = 25;
const int FRUIT_RADIUS = 10;

const sf::Vector2f
    UP = sf::Vector2f(0, -1),
    DOWN = sf::Vector2f(0, 1),
    RIGHT = sf::Vector2f(1, 0),
    LEFT = sf::Vector2f(-1, 0);

int r = 255;

sf::RectangleShape createChunk()
{
    sf::RectangleShape chunk(sf::Vector2f(BODY_CHUNK_WIDTH, BODY_CHUNK_HEIGHT));
    chunk.setFillColor(sf::Color(0, 255, 0));
    r -= 10;
    return chunk;
}

sf::CircleShape createFruit()
{
    float x = rand() % (WINDOW_WIDTH - FRUIT_RADIUS - 10) + FRUIT_RADIUS + 10;
    float y = rand() % (WINDOW_HEIGHT - FRUIT_RADIUS - 10) +  FRUIT_RADIUS + 10;
    sf::CircleShape fruit(FRUIT_RADIUS);
    fruit.setFillColor(sf::Color(200, 200, 200));
    fruit.setPosition(sf::Vector2f(x, y));
    return fruit;
}

bool assertInsideBounds(sf::Vector2f movement)
{
    return !(movement.x < 0 || movement.x > WINDOW_WIDTH - BODY_CHUNK_WIDTH ||
             movement.y < 0 || movement.y > WINDOW_HEIGHT - BODY_CHUNK_HEIGHT);
}

int main()
{

    srand(time(nullptr));
    // Setup snake
    sf::Vector2f direction = RIGHT, prevDirection;

    sf::RectangleShape head(sf::Vector2f(BODY_CHUNK_WIDTH, BODY_CHUNK_HEIGHT));
    head.setFillColor(sf::Color(0, 255, 0));
    head.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
    std::vector<sf::RectangleShape> body({head});
    sf::CircleShape fruit = createFruit();

    sf::Text pointsText;
    pointsText.setPosition(15, 5);
    pointsText.setCharacterSize(50);
    pointsText.setFillColor(sf::Color::White);
    sf::Font arial;
    if(!arial.loadFromFile("dDicapslock.ttf")){
        exit(-1);
    }

    pointsText.setFont(arial);

    float fpsLimit = 15.0;
    
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake");
    window.setFramerateLimit(fpsLimit);
    
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::W:
                    direction = UP;
                    break;
                case sf::Keyboard::S:
                    direction = DOWN;
                    break;
                case sf::Keyboard::A:
                    direction = LEFT;
                    break;
                case sf::Keyboard::D:
                    direction = RIGHT;
                    break;
                }
            }
        }

        window.clear();
        window.draw(fruit);

        for (int i = 0; i < body.size(); i++)
        {
            auto chunk = body[i];
            if (!assertInsideBounds(chunk.getPosition()))
            {
                body.clear();
                sf::RectangleShape head(sf::Vector2f(BODY_CHUNK_WIDTH, BODY_CHUNK_HEIGHT));
                head.setFillColor(sf::Color(255, 255, 255));
                head.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
                body.push_back(head);
                fpsLimit = 15.0;
                window.setFramerateLimit(fpsLimit);
                break;
            }
            window.draw(chunk);
        }
        pointsText.setString(std::to_string(body.size()-1));
        window.draw(pointsText);
        
        // std::for_each(body.begin(), body.end(), [&window, &body](sf::RectangleShape &chunk){
        //     window.draw(chunk);
        // });
        window.display();

        std::optional<sf::Vector2f> prevPos;
        sf::Vector2f currPos;
        for (auto &chunk : body)
        {
            currPos = chunk.getPosition();
            if (prevPos.has_value() == false)
            {
                chunk.move(direction * float(BODY_CHUNK_HEIGHT)); // head
            } else {
                chunk.setPosition(prevPos.value());
            }
            
            prevPos.emplace(currPos);
        }

        if(body[0].getGlobalBounds().intersects(fruit.getGlobalBounds())) {
            auto chunk = createChunk();
            sf::Vector2f pos(
                body[body.size() -1].getPosition().x - direction.x * BODY_CHUNK_WIDTH,
                body[body.size() -1].getPosition().y - direction.y * BODY_CHUNK_HEIGHT);
            chunk.setPosition(pos);
            body.push_back(chunk);
            fruit = createFruit();
            fpsLimit += 0.5;
            window.setFramerateLimit(fpsLimit);
        }

    }

    return 0;
}