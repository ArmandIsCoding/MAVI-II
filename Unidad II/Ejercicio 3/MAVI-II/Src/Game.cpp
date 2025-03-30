#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	// Inicialización de la ventana y configuración de propiedades
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom(); // Configuración de la vista del juego
	InitPhysics(); // Inicialización del motor de física
}

// Bucle principal del juego
void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor); // Limpiar la ventana
		DoEvents(); // Procesar eventos de entrada
		UpdatePhysics(); // Actualizar la simulación física
		DrawGame(); // Dibujar el juego
		wnd->display(); // Mostrar la ventana
	}
}

// Actualización de la simulación física
void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Simular el mundo físico
	phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
	phyWorld->DebugDraw(); // Dibujar el mundo físico para depuración
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
	// Dibujar el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	// Dibujar las paredes
	sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	leftWallShape.setFillColor(sf::Color::Blue);
	leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
	wnd->draw(leftWallShape);

	sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	rightWallShape.setFillColor(sf::Color::Cyan);
	rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
	wnd->draw(rightWallShape);

	// El techo que no estaba en el ejemplo original
	sf::RectangleShape ceilingWallShape(sf::Vector2f(90, 5));
	ceilingWallShape.setFillColor(sf::Color::Green);
	ceilingWallShape.setPosition(5, 0);
	wnd->draw(ceilingWallShape);

	// Dibujar el cuerpo de control (círculo)
	sf::CircleShape controlShape(5);
	controlShape.setFillColor(sf::Color::Magenta);

	// El magic number 5 que resto tanto en X como en Y corresponde al a ancho y el alto del cuerpo de control
	controlShape.setPosition(controlBody->GetPosition().x - 5, controlBody->GetPosition().y - 5);
	wnd->draw(controlShape);
}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
			break;
		}
	}

	controlBody->SetAwake(true);
}

// Configuración de la vista del juego
void Game::SetZoom()
{
	View camara;
	// Posicionamiento y tamaño de la vista
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); // Asignar la vista a la ventana
}

void Game::InitPhysics()
{
	srand(static_cast<unsigned>(time(0)));  // Semilla para números aleatorios

	// Inicializar el mundo físico sin gravedad
	phyWorld = new b2World(b2Vec2(10.0f, 10.0f));  // Desactivar la gravedad temporalmente

	// Crear un renderer de debug para visualizar el mundo físico
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Crear el suelo y las paredes estáticas del mundo físico
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	b2Body* ceilingWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 5);
	ceilingWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	// ------------------- CONTROL BODY -------------------
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(50.0f, 50.0f);

	b2CircleShape circleShape;
	circleShape.m_radius = 5.0f;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.2f;
	fixtureDef.restitution = 0.8f;

	controlBody = phyWorld->CreateBody(&bodyDef);
	controlBody->CreateFixture(&fixtureDef);
	controlBody->SetAwake(true);

	// ------------------- CUERPOS RANDOM -------------------
	randomBodies.clear();  // Limpiar la lista si se vuelve a inicializar

	for (int i = 0; i < numRandomBodies; ++i)
	{
		b2BodyDef randomBodyDef;
		randomBodyDef.type = b2_kinematicBody;

		// Posición aleatoria en el rango visible de la pantalla
		float randX = static_cast<float>(rand() % 90 + 5);
		float randY = static_cast<float>(rand() % 90 + 5);
		randomBodyDef.position.Set(randX, randY);

		b2CircleShape smallCircleShape;
		smallCircleShape.m_radius = static_cast<float>(rand() % 3 + 1);  // Cuerpos más chicos (1-3 de radio)

		b2FixtureDef randomFixture;
		randomFixture.shape = &smallCircleShape;
		randomFixture.density = static_cast<float>(rand() % 100) / 10.0f + 0.1f;
		randomFixture.friction = static_cast<float>(rand() % 100) / 100.0f;
		randomFixture.restitution = static_cast<float>(rand() % 100) / 50.0f;

		b2Body* randomBody = phyWorld->CreateBody(&randomBodyDef);
		randomBody->CreateFixture(&randomFixture);
		randomBody->SetAwake(true);

		randomBodies.push_back(randomBody);  // Guardar el cuerpo en la lista
	}
}



// Destructor de la clase

Game::~Game(void)
{
}