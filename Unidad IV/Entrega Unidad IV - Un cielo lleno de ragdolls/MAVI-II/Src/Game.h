#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include "Avatar.h"
#include "Ragdoll.h"
#include <list>
#include "Obstacle.h"
#include "DynamicObstacle.h"
#include "StaticObstacle.h"

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int H;
	int W;
	RenderWindow *wnd;
	Color clearColor;

	// Obstáculos
	std::vector<Obstacle*> obstacles;

	// Objetos de box2d
	b2World *phyWorld;
	SFMLRenderer *debugRender;

	//tiempo de frame
	float frameTime;
	int fps;

	// Joints, en un vector porque necesito muchos.
	std::vector<Ragdoll> ragdolls;

	b2Body* groundBody;

	// Cuerpos de box2d
	b2Body* cannonBody;

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

