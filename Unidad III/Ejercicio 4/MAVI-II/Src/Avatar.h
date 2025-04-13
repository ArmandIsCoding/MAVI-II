// Fuente y autor original: Emanuel Rojas Fredini en youtube.
// https://www.youtube.com/watch?v=QKT2UEm0uqQ
// material teórico de la cátedra.

#pragma once

#include <box2d/Box2D.h>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Config.hpp"

class Avatar
{

private:
	sf::Sprite* m_sprite;
	b2Body* m_body;
	
public:
	Avatar(b2Body* body, sf::Sprite* sprite);
	void Actualizar();
	void Dibujar(sf::RenderWindow& win);
};

