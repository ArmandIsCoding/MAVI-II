#pragma once
#include <Box2D/Box2D.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Ragdoll
{
public:
    Ragdoll(b2World* world, const b2Vec2& position);
    ~Ragdoll() = default;

    void Draw(sf::RenderWindow& window);
    b2Fixture* GetHeadFixture();

    void ApplyImpulse(const b2Vec2& impulse);
	void ApplyImpulseBasedOnMouse(const sf::Vector2i& mousePosition, b2Body* cannonBody);

private:
    b2World* world;

    // Partes del cuerpo
    b2Body* head;
    b2Body* body;
    b2Body* leftArm;
    b2Body* rightArm;
    b2Body* leftLeg;
    b2Body* rightLeg;

    // Joints del cuerpo
    std::vector<b2Joint*> joints;

    // Métodos auxiliares
    float GetBodyHeight(b2Body* body);
    b2Body* CreateBox(const b2Vec2& pos, float width, float height);
    b2Body* CreateCircle(const b2Vec2& pos, float radius);
    void CreateJoints();
};