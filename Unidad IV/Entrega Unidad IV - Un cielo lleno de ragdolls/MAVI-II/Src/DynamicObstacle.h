// DynamicObstacle.h
#pragma once
#include "Obstacle.h"

class DynamicObstacle : public Obstacle {
public:
    DynamicObstacle(b2World* world, float x, float y, float width, float height);
    void Update() override;
    void Draw(sf::RenderWindow& window) override;
};
