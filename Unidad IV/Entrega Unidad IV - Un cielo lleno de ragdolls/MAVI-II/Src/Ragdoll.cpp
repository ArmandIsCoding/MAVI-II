// Ragdoll.cpp
#include "Ragdoll.h"
#include <SFML/Graphics.hpp>

const float SCALE = 20.0f;  // Escala para pasar de metros (Box2D) a píxeles (SFML)

void Ragdoll::ApplyImpulse(const b2Vec2& impulse)
{
    head->ApplyLinearImpulse(impulse, head->GetWorldCenter(), true);
}

/// <summary>
/// Version de applyimpulse que depende de la distancia al mouse
/// </summary>
/// <param name="mousePosition"></param>
/// <param name="cannonBody"></param>
void Ragdoll::ApplyImpulseBasedOnMouse(const sf::Vector2i& mousePosition, b2Body* cannonBody)
{
    b2Vec2 basePosition = cannonBody->GetPosition(); // posición de la base del cañón

    float mouseX = static_cast<float>(mousePosition.x) / SCALE;
    float mouseY = static_cast<float>(mousePosition.y) / SCALE;
    b2Vec2 mouseWorldPos(mouseX, mouseY);

    float dx = mouseWorldPos.x - basePosition.x;
    float dy = mouseWorldPos.y - basePosition.y;
    float distance = sqrt(dx * dx + dy * dy); // Pitágoras

    // Aumentar la sensibilidad de la fuerza en función de la distancia
    float forceMagnitude = (std::pow(distance, 3.0f) * 100.0f / 10000.0f);
    // Exponente 3.0 para exagerar el efecto de la distancia al mouse porque no se nota mucho
	// Divido por 10000 porque fui agregando 0s para ir atenuando el impulso
    // porque sino al salir disparado con demasiado impulso se frenan en seco, como si chocaran con el aire mismo

    float angle = cannonBody->GetAngle();
    b2Vec2 direction(cos(angle), sin(angle));

    b2Vec2 impulse = direction;
    impulse *= forceMagnitude;

    ApplyImpulse(impulse);
}


Ragdoll::Ragdoll(b2World* world, const b2Vec2& position)
    : world(world)
{
    float scale = 0.6f;
    // Aumento de tamaño para evitar problemas. Experimental.
    // Por algun motivo los ragdolls si son muy grandes no se disparan rápido.

    // Creación de partes del cuerpo con escala
    head = CreateCircle(position + b2Vec2(10.0f * scale, 10.0f * scale), 2.2f * scale);
    body = CreateBox(position + b2Vec2(10.0f * scale, 20.0f * scale), 2.4f * scale, 10.0f * scale);
    leftArm = CreateBox(position + b2Vec2(7.0f * scale, 20.0f * scale), 1.0f * scale, 10.0f * scale);
    rightArm = CreateBox(position + b2Vec2(13.0f * scale, 20.0f * scale), 1.0f * scale, 10.0f * scale);
    leftLeg = CreateBox(position + b2Vec2(9.0f * scale, 32.0f * scale), 1.0f * scale, 10.0f * scale);
    rightLeg = CreateBox(position + b2Vec2(11.0f * scale, 32.0f * scale), 1.0f * scale, 10.0f * scale);

    body->SetAngularDamping(5.0f);

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
    bodyDef.fixedRotation = false; // Permitir rotación para la cabeza y otras partes

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
    float scale = 0.5f;

    // Dimensiones reales de cada parte
    float bodyWidth = 4.4f * scale;
    float bodyHeight = 10.0f * scale;
    float armWidth = 1.0f * scale;
    float armHeight = 10.0f * scale;
    float legWidth = 1.0f * scale;
    float legHeight = 10.0f * scale;

    // Cuello
    {
        b2RevoluteJointDef jointDef;
        jointDef.Initialize(head, body, head->GetWorldCenter() + b2Vec2(0.0f, 0.2f * scale));
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.9f;
        jointDef.upperAngle = 0.9f;
        joints.push_back(world->CreateJoint(&jointDef));
    }

    // Hombro izquierdo
    {
        b2Vec2 anchor = body->GetWorldCenter() + b2Vec2(-bodyWidth / 2.0f, -bodyHeight); // esquina superior izquierda del cuerpo
        anchor.y += 1.0f * scale; // pequeño ajuste hacia abajo (opcional)
        b2RevoluteJointDef jointDef;
        jointDef.Initialize(body, leftArm, anchor);
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -2.0f;
        jointDef.upperAngle = 2.0f;
        joints.push_back(world->CreateJoint(&jointDef));
    }

    // Hombro derecho
    {
        b2Vec2 anchor = body->GetWorldCenter() + b2Vec2(bodyWidth / 2.0f, -bodyHeight); // esquina superior derecha del cuerpo
        anchor.y += 1.0f * scale; // pequeño ajuste hacia abajo
        b2RevoluteJointDef jointDef;
        jointDef.Initialize(body, rightArm, anchor);
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -2.0f;
        jointDef.upperAngle = 2.0f;
        joints.push_back(world->CreateJoint(&jointDef));
    }

    // Pierna izquierda
    {
        b2Vec2 anchor = body->GetWorldCenter() + b2Vec2(-bodyWidth / 4.0f, bodyHeight / 2.0f); // cadera izquierda
        b2RevoluteJointDef jointDef;
        jointDef.Initialize(body, leftLeg, anchor);
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.5f;
        jointDef.upperAngle = 1.0f;
        joints.push_back(world->CreateJoint(&jointDef));
    }

    // Pierna derecha
    {
        b2Vec2 anchor = body->GetWorldCenter() + b2Vec2(bodyWidth / 4.0f, bodyHeight / 2.0f); // cadera derecha
        b2RevoluteJointDef jointDef;
        jointDef.Initialize(body, rightLeg, anchor);
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.5f;
        jointDef.upperAngle = 1.0f;
        joints.push_back(world->CreateJoint(&jointDef));
    }
}


void Ragdoll::Draw(sf::RenderWindow& window)
{
    // CABEZA
    sf::CircleShape cabeza(2.2f * SCALE); // radio * escala
    cabeza.setFillColor(sf::Color::Red);
    cabeza.setOrigin(2.2f * SCALE, 2.2f * SCALE);
    b2Vec2 posCabeza = head->GetPosition();
    cabeza.setPosition(posCabeza.x * SCALE, posCabeza.y * SCALE);
    cabeza.setRotation(head->GetAngle() * 180.0f / b2_pi);
    window.draw(cabeza);

    // TORSO
    sf::RectangleShape torso(sf::Vector2f(2.4f * SCALE, 10.0f * SCALE));
    torso.setFillColor(sf::Color::Blue);
    torso.setOrigin(1.2f * SCALE, 5.0f * SCALE);
    b2Vec2 posTorso = body->GetPosition();
    torso.setPosition(posTorso.x * SCALE, posTorso.y * SCALE);
    torso.setRotation(body->GetAngle() * 180.0f / b2_pi);
    window.draw(torso);

    // BRAZO IZQUIERDO
    sf::RectangleShape brazoIzq(sf::Vector2f(1.0f * SCALE, 10.0f * SCALE));
    brazoIzq.setFillColor(sf::Color::Red);
    brazoIzq.setOrigin(0.5f * SCALE, 5.0f * SCALE);
    b2Vec2 posBrazoIzq = leftArm->GetPosition();
    brazoIzq.setPosition(posBrazoIzq.x * SCALE, posBrazoIzq.y * SCALE);
    brazoIzq.setRotation(leftArm->GetAngle() * 180.0f / b2_pi);
    window.draw(brazoIzq);

    // BRAZO DERECHO
    sf::RectangleShape brazoDer(sf::Vector2f(1.0f * SCALE, 10.0f * SCALE));
    brazoDer.setFillColor(sf::Color::Red);
    brazoDer.setOrigin(0.5f * SCALE, 5.0f * SCALE);
    b2Vec2 posBrazoDer = rightArm->GetPosition();
    brazoDer.setPosition(posBrazoDer.x * SCALE, posBrazoDer.y * SCALE);
    brazoDer.setRotation(rightArm->GetAngle() * 180.0f / b2_pi);
    window.draw(brazoDer);

    // PIERNA IZQUIERDA
    sf::RectangleShape piernaIzq(sf::Vector2f(1.0f * SCALE, 10.0f * SCALE));
    piernaIzq.setFillColor(sf::Color::Green);
    piernaIzq.setOrigin(0.5f * SCALE, 5.0f * SCALE);
    b2Vec2 posPiernaIzq = leftLeg->GetPosition();
    piernaIzq.setPosition(posPiernaIzq.x * SCALE, posPiernaIzq.y * SCALE);
    piernaIzq.setRotation(leftLeg->GetAngle() * 180.0f / b2_pi);
    window.draw(piernaIzq);

    // PIERNA DERECHA
    sf::RectangleShape piernaDer(sf::Vector2f(1.0f * SCALE, 10.0f * SCALE));
    piernaDer.setFillColor(sf::Color::Green);
    piernaDer.setOrigin(0.5f * SCALE, 5.0f * SCALE);
    b2Vec2 posPiernaDer = rightLeg->GetPosition();
    piernaDer.setPosition(posPiernaDer.x * SCALE, posPiernaDer.y * SCALE);
    piernaDer.setRotation(rightLeg->GetAngle() * 180.0f / b2_pi);
    window.draw(piernaDer);
}

b2Fixture* Ragdoll::GetHeadFixture()
{
    return head->GetFixtureList();
}
