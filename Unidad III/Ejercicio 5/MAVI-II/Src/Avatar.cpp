// Fuente y autor original: Emanuel Rojas Fredini en youtube.
// https://www.youtube.com/watch?v=QKT2UEm0uqQ
// material teórico de la cátedra.
#pragma once

//#include "stdafx.h" //no anda, investigar qué es.
#include "Avatar.h"

Avatar::Avatar(b2Body* body, sf::Sprite* sprite)
	:m_body(body), m_sprite(sprite)
{
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);

	// "hasta que f" porque eventualmente f va a ser null
	// y esa es la condicion de corte
	for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
	{
		aabb.Combine(f->GetAABB(0));
	}

	// dimension: tamanio de imagen por una cierta escala
	// => dimension/tamanioimagen=escala
	float sizeX = aabb.GetExtents().x * 2.0 / m_sprite->getTexture()->getSize().x;
	float sizeY = aabb.GetExtents().y * 2.0 / m_sprite->getTexture()->getSize().y;

	// Ya tengo la escala, ahora la aplico al sprite.
	m_sprite->setScale(sizeX, sizeY);

	// y queremos que el origen del sprite no sea la esquina superior izquierda sino el medio
	m_sprite->setOrigin(sf::Vector2f(m_sprite->getTexture()->getSize().x / 2.0, m_sprite->getTexture()->getSize().y / 2.0));

	b2Vec2 pos = m_body->GetPosition();
	m_sprite->setPosition(sf::Vector2f(pos.x, pos.y));
}

void Avatar::Actualizar()
{
	// actualizamos la posicion del sprite con el objeto fisico
	b2Vec2 pos = m_body->GetPosition();
	m_sprite->setPosition(pos.x, pos.y);

	// actualizamos la rotacion
	float ang = m_body->GetAngle();

	// box2d nos da ang en radianes pero sfml lo pide en grados.
	// regla de 3 para convertir de RAD a GRA

	// pi ____ 180
	// ang ___ x = ang*180/pi

	m_sprite->setRotation(ang * 180 / 3.1415f);
}

void Avatar::Dibujar(sf::RenderWindow& win)
{
	win.draw(*m_sprite);
}