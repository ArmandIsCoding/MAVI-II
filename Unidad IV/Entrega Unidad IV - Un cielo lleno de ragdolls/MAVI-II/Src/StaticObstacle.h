// StaticObstacle.h
#pragma once
#include "Obstacle.h"

class StaticObstacle : public Obstacle {
public:
    StaticObstacle(b2World* world, float x, float y, float width, float height);
    void Update() override;
    void Draw(sf::RenderWindow& window) override;
};
