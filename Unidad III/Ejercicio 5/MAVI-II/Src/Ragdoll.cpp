#include "Ragdoll.h"
#include <SFML/Graphics.hpp>

Ragdoll::Ragdoll(b2World* world, const b2Vec2& position)
    : world(world)
{
	float factor = 5.0f;

    // Crear las partes del cuerpo
    // Cada magic number es el resultado de experimentar muchiiiiiisimo
    head = CreateCircle(position + b2Vec2(10.0f, 10.0f), 2.2f);
    body = CreateBox(position + b2Vec2(10,20), 2.4f, 10.0f);
    leftArm = CreateBox(position + b2Vec2(7.0f, 20.0f), 1.0f, 10.0f);
    rightArm = CreateBox(position + b2Vec2(13.0f, 20.0f), 1.0f, 10.0f);
    leftLeg = CreateBox(position + b2Vec2(9.0f, 32.0f), 1.0f, 10.0f);
    rightLeg = CreateBox(position + b2Vec2(11.0f, 32.0f), 1.0f, 10.0f);

    // Crear los joints para unir todo
    CreateJoints();
}

b2Body* Ragdoll::CreateBox(const b2Vec2& pos, float width, float height)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = pos;

    b2Body* body = world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(width / 2.0f, height / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    return body;
}

b2Body* Ragdoll::CreateCircle(const b2Vec2& pos, float radius)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = pos;
    bodyDef.fixedRotation = true;

    b2Body* body = world->CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    return body;
}

void Ragdoll::CreateJoints()
{
    // Cuello a cuerpo
    {
        b2RevoluteJointDef jointDef;
        jointDef.Initialize(head, body, head->GetWorldCenter() + b2Vec2(0.0f, 0.2f));
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.9f;
        jointDef.upperAngle = 0.9f;
        joints.push_back(world->CreateJoint(&jointDef));
    }

    // Hombro izquierdo
    {
        b2Vec2 anchor = leftArm->GetPosition();
        anchor.y -= 5.0f; // Ir al borde superior del brazo

        b2RevoluteJointDef jointDef;
        jointDef.Initialize(body, leftArm, anchor);
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -1.0f;
        jointDef.upperAngle = 1.0f;
        joints.push_back(world->CreateJoint(&jointDef));
    }

    // Hombro derecho
    {
        b2Vec2 anchor = rightArm->GetPosition();
        anchor.y -= 5.0f; // Ir al borde superior del brazo

        b2RevoluteJointDef jointDef;
        jointDef.Initialize(body, rightArm, anchor);
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -1.0f;
        jointDef.upperAngle = 1.0f;
        joints.push_back(world->CreateJoint(&jointDef));
    }

    // Pierna izquierda
    {
        b2Vec2 anchor = leftLeg->GetPosition();
        anchor.y -= 5.0f; // Ir al borde superior del brazo

        b2RevoluteJointDef jointDef;
        jointDef.Initialize(body, leftLeg, anchor);
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -1.0f;
        jointDef.upperAngle = 1.0f;
        joints.push_back(world->CreateJoint(&jointDef));
    }

    // Pierna dercha
    {
        b2Vec2 anchor = rightLeg->GetPosition();
        anchor.y -= 5.0f; // Ir al borde superior del brazo

        b2RevoluteJointDef jointDef;
        jointDef.Initialize(body, rightLeg, anchor);
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -1.0f;
        jointDef.upperAngle = 1.0f;
        joints.push_back(world->CreateJoint(&jointDef));
    }
}

/// <summary>
/// Igual lo está ignorando O_o
/// </summary>
void Ragdoll::Draw(sf::RenderWindow& window)
{
    // DIBUJAR CABEZA (círculo amarillo)
    sf::CircleShape cabeza(20.0f);
    cabeza.setFillColor(sf::Color::Red);
    cabeza.setOrigin(20.0f, 20.0f);
    b2Vec2 posCabeza = head->GetPosition();
    cabeza.setPosition(posCabeza.x * 100.0f, posCabeza.y * 100.0f);
    cabeza.setRotation(head->GetAngle() * 180.0f / b2_pi);
    window.draw(cabeza);

    // DIBUJAR CUERPO (rectángulo azul)
    sf::RectangleShape torsoShape(sf::Vector2f(40.0f, 80.0f));
    torsoShape.setFillColor(sf::Color::Blue);
    torsoShape.setOrigin(20.0f, 40.0f);
    b2Vec2 posTorso = body->GetPosition();
    torsoShape.setPosition(posTorso.x * 100.0f, posTorso.y * 100.0f);
    torsoShape.setRotation(body->GetAngle() * 180.0f / b2_pi);
    window.draw(torsoShape);

    // BRAZO IZQUIERDO (rojo)
    sf::RectangleShape brazoIzq(sf::Vector2f(30.0f, 10.0f));
    brazoIzq.setFillColor(sf::Color::Red);
    brazoIzq.setOrigin(15.0f, 5.0f);
    b2Vec2 posBrazoIzq = leftArm->GetPosition();
    brazoIzq.setPosition(posBrazoIzq.x * 100.0f, posBrazoIzq.y * 100.0f);
    brazoIzq.setRotation(leftArm->GetAngle() * 180.0f / b2_pi);
    window.draw(brazoIzq);

    // BRAZO DERECHO (rojo)
    sf::RectangleShape brazoDer(sf::Vector2f(30.0f, 10.0f));
    brazoDer.setFillColor(sf::Color::Red);
    brazoDer.setOrigin(15.0f, 5.0f);
    b2Vec2 posBrazoDer = rightArm->GetPosition();
    brazoDer.setPosition(posBrazoDer.x * 100.0f, posBrazoDer.y * 100.0f);
    brazoDer.setRotation(rightArm->GetAngle() * 180.0f / b2_pi);
    window.draw(brazoDer);

    // PIERNA IZQUIERDA (verde)
    sf::RectangleShape piernaIzq(sf::Vector2f(15.0f, 50.0f));
    piernaIzq.setFillColor(sf::Color::Green);
    piernaIzq.setOrigin(7.5f, 25.0f);
    b2Vec2 posPiernaIzq = leftLeg->GetPosition();
    piernaIzq.setPosition(posPiernaIzq.x * 100.0f, posPiernaIzq.y * 100.0f);
    piernaIzq.setRotation(leftLeg->GetAngle() * 180.0f / b2_pi);
    window.draw(piernaIzq);

    // PIERNA DERECHA (verde)
    sf::RectangleShape piernaDer(sf::Vector2f(15.0f, 50.0f));
    piernaDer.setFillColor(sf::Color::Green);
    piernaDer.setOrigin(7.5f, 25.0f);
    b2Vec2 posPiernaDer = rightLeg->GetPosition();
    piernaDer.setPosition(posPiernaDer.x * 100.0f, posPiernaDer.y * 100.0f);
    piernaDer.setRotation(rightLeg->GetAngle() * 180.0f / b2_pi);
    window.draw(piernaDer);
}

/// <summary>
/// Auxiliar para obtener el fixture de la cabeza,
/// con esto obtengo el body y le aplico una fuerza.
/// En definitiva una forma rebuscada de arrastrar el ragdoll por la pantalla.
/// </summary>
/// <returns></returns>
b2Fixture* Ragdoll::GetHeadFixture()
{
    return head->GetFixtureList();
}
