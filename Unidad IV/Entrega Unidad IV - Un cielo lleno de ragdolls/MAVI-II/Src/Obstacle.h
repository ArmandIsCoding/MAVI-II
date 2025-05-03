#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    virtual ~Obstacle() = default;
    virtual void Update() = 0;
    virtual void Draw(sf::RenderWindow& window) = 0;

protected:
    b2Body* body = nullptr;
    sf::RectangleShape shape;
};
