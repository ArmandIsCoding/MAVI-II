#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	// Inicialización de la ventana y configuración de propiedades
	this->H = alto;
	this->W = ancho;
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
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
	// Dibujar el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	// Dibujar todos los ragdolls
	//for (Ragdoll& rag : ragdolls)
	//{
	//	rag.Draw(*wnd);
	//}
}

// Eventos
void Game::DoEvents()
{
	sf::Event evt;
	while (wnd->pollEvent(evt))
	{
		// Control de rotación del cañón
		float rotationSpeed = 1.5f;  // Velocidad de rotación
		float currentAngle = cannonBody->GetAngle();

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			cannonBody->SetTransform(cannonBody->GetPosition(), currentAngle - rotationSpeed * frameTime);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			cannonBody->SetTransform(cannonBody->GetPosition(), currentAngle + rotationSpeed * frameTime);
		}

		// Update the problematic line in DoEvents method
		if (evt.type == Event::MouseMoved)
		{
			Vector2f mousePos = wnd->mapPixelToCoords(Mouse::getPosition(*wnd));
			Vector2f cannonPos = sf::Vector2f(cannonBody->GetPosition().x, cannonBody->GetPosition().y); // Use the helper function
			float angle = atan2(mousePos.y - cannonPos.y, mousePos.x - cannonPos.x);
			cannonBody->SetTransform(cannonBody->GetPosition(), angle);
		}

		// Cada vez que se hace click se agrega una nueva ragdoll al vector ragdolls y se dispara en el sentido del cañón
		if (evt.type == Event::MouseButtonPressed)
		{
			if (evt.mouseButton.button == sf::Mouse::Left)
			{
				float spawnDistance = 4.0f;  // Más alejado para evitar solapamiento con el cañón
				b2Vec2 direction(cos(cannonBody->GetAngle()), sin(cannonBody->GetAngle()));

				auto cp = cannonBody->GetPosition();
				b2Vec2 spawnPosition = b2Vec2(cp.x, cp.y - 13) + spawnDistance * direction;

				Ragdoll rag(phyWorld, spawnPosition);
				ragdolls.push_back(rag);

				sf::Vector2i mousePosition = sf::Mouse::getPosition(); // o el nombre de tu ventana
				rag.ApplyImpulseBasedOnMouse(mousePosition, cannonBody);
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

	//// Crear 5 ragdolls en distintas posiciones
	//for (int i = 0; i < 5; ++i)
	//{
	//	float x = 10.0f + i * 8.0f; // Separar los ragdolls horizontalmente
	//	float y = 50.0f;

	//	Ragdoll rag(phyWorld, b2Vec2(x, y));
	//	ragdolls.push_back(rag);
	//}

	//// Empujar un poquito la primer ragdoll para que se caigan en dominó
	//// cuando arranca el programa
	//b2Fixture* r = ragdolls[0].GetHeadFixture();
	//b2Vec2 impulse(150, 0);
	//r->GetBody()->SetAwake(true);
	//r->GetBody()->ApplyLinearImpulseToCenter(impulse, true);


	////
	// -------------------- CREAR EL CAÑÓN --------------------

	// Definir el cuerpo del cañón
	b2BodyDef cannonDef;
	cannonDef.type = b2_staticBody;  // No se mueve (pero si se puede rotar)
	cannonDef.position.Set(5.0f, 50.0f);  // Fijo a la izquierda
	cannonDef.angle = 0.0f;  // Inicialmente recto

	// Crear el cuerpo en el mundophy
	cannonBody = phyWorld->CreateBody(&cannonDef);
	// Definir la forma del cañón (rectangulo)
	b2PolygonShape cannonShape;
	cannonShape.SetAsBox(5.0f, 1.5f);

	// Crear fixture para que sea sólido
	b2FixtureDef cannonFixture;
	cannonFixture.shape = &cannonShape;
	cannonFixture.density = 1.0f;
	cannonFixture.friction = 0.3f;

	cannonBody->CreateFixture(&cannonFixture);
}

// Destructor de la clase

Game::~Game(void)
{
}