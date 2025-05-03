#include "StaticObstacle.h"

StaticObstacle::StaticObstacle(b2World* world, float x, float y, float width, float height) {
    // Crear el cuerpo físico estático en Box2D
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = b2_staticBody;
    body = world->CreateBody(&bodyDef);

    // Crear la forma del obstáculo en Box2D (recuerda: SetAsBox usa la mitad)
    b2PolygonShape box;
    box.SetAsBox(width / 2.0f, height / 2.0f);
    body->CreateFixture(&box, 0.0f);  // masa 0.0f porque es estático

    // Crear el rectángulo visual en SFML con el mismo tamaño físico
    shape.setSize(sf::Vector2f(width, height));  // SFML espera tamaño completo
    shape.setOrigin(width / 2.0f, height / 2.0f); // origen centrado como Box2D
    shape.setFillColor(sf::Color::Blue);
}

void StaticObstacle::Update() {
    // No hace nada porque es estático
}

void StaticObstacle::Draw(sf::RenderWindow& window) {
    // Sincronizar posición y rotación desde el cuerpo físico
    b2Vec2 pos = body->GetPosition();
    shape.setPosition(pos.x, pos.y);
    shape.setRotation(body->GetAngle() * 180.0f / b2_pi);
    window.draw(shape);
}
