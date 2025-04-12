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
		CheckCollitions(); // Comprobar colisiones
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

	// Dibujar las formas renderizables encima de los bodys fisicos.
	// A:
	sf::RectangleShape controlShapeA(sf::Vector2f(5, 5));
	controlShapeA.setFillColor(sf::Color::Magenta);
	controlShapeA.setPosition(controlBodyA->GetPosition().x - 2.5f, controlBodyA->GetPosition().y - 2.5f);
	wnd->draw(controlShapeA);

	//B:
	sf::RectangleShape controlShapeB(sf::Vector2f(5, 5));
	controlShapeB.setFillColor(sf::Color::Red);
	controlShapeB.setPosition(controlBodyB->GetPosition().x - 2.5f, controlBodyB->GetPosition().y - 2.5f);
	wnd->draw(controlShapeB);


	// Dibujar el avatar
	m_avatar->Dibujar(*wnd);
	m_avatar->Actualizar(); // Actualizar la posición del avatar
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

		case Event::KeyPressed:
			// Detectar las flechas del teclado y mover controlBodyA
			if (evt.key.code == Keyboard::Key::Up)
			{
				controlBodyA->SetLinearVelocity(b2Vec2(controlBodyA->GetLinearVelocity().x, -25.0f));
			}
			else if (evt.key.code == Keyboard::Key::Down)
			{
				controlBodyA->SetLinearVelocity(b2Vec2(controlBodyA->GetLinearVelocity().x, 25.0f));
			}
			else if (evt.key.code == Keyboard::Key::Left)
			{
				controlBodyA->SetLinearVelocity(b2Vec2(-25.0f, controlBodyA->GetLinearVelocity().y));
			}
			else if (evt.key.code == Keyboard::Key::Right)
			{
				controlBodyA->SetLinearVelocity(b2Vec2(25.0f, controlBodyA->GetLinearVelocity().y));
			}
			break;

		case Event::KeyReleased:
			// Detener el movimiento cuando se suelta la tecla
			if (evt.key.code == Keyboard::Key::Up || evt.key.code == Keyboard::Key::Down)
			{
				controlBodyA->SetLinearVelocity(b2Vec2(controlBodyA->GetLinearVelocity().x, 0.0f));
			}
			else if (evt.key.code == Keyboard::Key::Left || evt.key.code == Keyboard::Key::Right)
			{
				controlBodyA->SetLinearVelocity(b2Vec2(0.0f, controlBodyA->GetLinearVelocity().y));
			}
			break;
		}
	}
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
	}
}

// Comprobación de colisiones (a implementar más adelante)
void Game::CheckCollitions()
{
	// Implementación de la comprobación de colisiones
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

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics()
{
	// Inicializar el mundo físico con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// Crear un renderer de debug para visualizar el mundo físico
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Crear el suelo y las paredes estáticas del mundo físico
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	// Crear dos cuerpos fisicos para unir con un joint.
	//A:
	controlBodyA = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 5.0f, 5.0f, 1.5, 0.1f, 0.5f);
	controlBodyA->SetTransform(b2Vec2(30.0f, 50.0f), 0.0f);
	//B:
	controlBodyB = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 5.0f, 5.0f, 1.5, 0.1f, 0.5f);
	controlBodyB->SetTransform(b2Vec2(60.0f, 40.0f), 0.0f);

	//// Crear un joint para unir controlBodyA y controlBodyB
	//b2DistanceJointDef jointDef;
	////jointDef.length = 1.0; jointDef.frequencyHz = 10; jointDef.dampingRatio = 0.5;
	//jointDef.bodyA = controlBodyA;
	//jointDef.bodyB = controlBodyB;
	//jointDef.length = 10.0f; // Longitud deseada del joint
	//jointDef.localAnchorA.Set(0.0f, 0.0f); // Anclaje local en A
	//jointDef.localAnchorB.Set(0.0f, 0.0f); // Anclaje local en B
	//jointDef.collideConnected = true; // Evitar colisiones entre los cuerpos conectados
	//phyWorld->CreateJoint(&jointDef);
	//jointDef.Initialize(controlBodyA, controlBodyB, controlBodyA->GetPosition(), controlBodyB->GetPosition());

	Box2DHelper::CreateDistanceJoint(
		phyWorld,
		controlBodyA, controlBodyA->GetWorldCenter(),
		controlBodyB, controlBodyB->GetWorldCenter(),
		10.0f,
		1.0f,
		0.1f);

	// Crear un avatar para el cuerpo controlBodyA
	t.loadFromFile("Pelota.png");
	sf::Sprite* sprite = new sf::Sprite(t);
	m_avatar = new Avatar(controlBodyA, sprite);
}

// Destructor de la clase

Game::~Game(void)
{
}