// DynamicObstacle.cpp
#include "DynamicObstacle.h"

DynamicObstacle::DynamicObstacle(b2World* world, float x, float y, float w, float h)
{
    // Crear el cuerpo dinámico (la caja roja)
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    body = world->CreateBody(&bodyDef);

    b2CircleShape circle;
    circle.m_radius = w / 2.0f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    fixtureDef.restitution = 0.3f;

    body->CreateFixture(&fixtureDef);

    // Crear un cuerpo estático pequeño arriba del dinámico como punto de anclaje
    b2BodyDef anchorDef;
    anchorDef.type = b2_staticBody;
    anchorDef.position.Set(x, y - 50); // posición más arriba
    b2Body* anchor = world->CreateBody(&anchorDef);

    // Crear una unión revoluta entre el cuerpo y el anclaje
    b2RevoluteJointDef jointDef;
    jointDef.Initialize(anchor, body, anchor->GetWorldCenter());
    world->CreateJoint(&jointDef);
}


void DynamicObstacle::Update() {
    // Se mueve por la física
}

void DynamicObstacle::Draw(sf::RenderWindow& window) {
    b2Vec2 pos = body->GetPosition();
    shape.setPosition(pos.x, pos.y);
    shape.setRotation(body->GetAngle() * 180.0f / b2_pi);
    window.draw(shape);
}
