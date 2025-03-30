#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>
#include <vector> //originalmente tenia solo 2 random bodies, pero queria un vector para agregar MUCHOS y tener mucha accion de colisiones

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

	// Cuerpo de box2d
	b2Body* controlBody;

	std::vector<b2Body*> randomBodies;  // Vector de random bodies (obstáculos)
	int numRandomBodies = 30;  // Cantidad

public:

	// Constructores, destructores e inicializadores
	Game(int ancho, int alto,std::string titulo);
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};

