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

	// Joint
	//b2DistanceJointDef jointDef;

	Avatar* m_avatar;
	sf::Texture t;

public:

	// Constructores, destructores e inicializadores
	Game(int ancho, int alto,std::string titulo);
	void CheckCollitions();
	void CreateEnemy(int x, int y);
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};

