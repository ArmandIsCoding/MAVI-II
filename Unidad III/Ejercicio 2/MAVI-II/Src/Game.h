#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include "Avatar.h"
#include <list>

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow *wnd;
	Color clearColor;

	// Objetos de box2d
	b2World *phyWorld;
	SFMLRenderer *debugRender;

	//tiempo de frame
	float frameTime;
	int fps;

	// Cuerpos de box2d, los voy a unir con un joint.
	b2Body* controlBodyA;
	b2Body* controlBodyB;

	// Resorte
	b2DistanceJointDef jointDef;

	Avatar* pelotaA;
	Avatar* pelotaB;
	sf::Texture t;

	// Manejo del drag and drop
	bool MouseOverB2Body(b2Body* cuerpo, b2Vec2 mousePos);
	b2Body* dragBody = nullptr;
	bool dragging = false;

public:

	// Constructores, destructores e inicializadores
	Game(int ancho, int alto,std::string titulo);
	void CheckCollitions();
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};

