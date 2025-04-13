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
		switch (evt.type)
		{
		case sf::Event::Closed:
			wnd->close();
			break;

		case sf::Event::KeyPressed:
		{
			b2Fixture* r = ragdolls[0].GetHeadFixture();
			b2Vec2 impulse(0, 0);

			switch (evt.key.code)
			{
			case sf::Keyboard::Up:
				impulse.Set(0, -150);
				break;
			case sf::Keyboard::Down:
				impulse.Set(0, 150);
				break;
			case sf::Keyboard::Left:
				impulse.Set(-150, 0);
				break;
			case sf::Keyboard::Right:
				impulse.Set(150, 0);
				break;
			default:
				break;
			}

			if (impulse.LengthSquared() > 0.0f)
			{
				r->GetBody()->SetAwake(true);
				r->GetBody()->ApplyLinearImpulseToCenter(impulse, true);
			}

			break;
		}

		case sf::Event::MouseButtonReleased:
			// Podés dejarlo vacío o quitar este case si no se usa
			break;
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

	// Crear 5 ragdolls en distintas posiciones
	for (int i = 0; i < 5; ++i)
	{
		float x = 10.0f + i * 8.0f; // Separar los ragdolls horizontalmente
		float y = 50.0f;

		Ragdoll rag(phyWorld, b2Vec2(x, y));
		ragdolls.push_back(rag);
	}

	// Empujar un poquito la primer ragdoll para que se caigan en dominó
	// cuando arranca el programa
	b2Fixture* r = ragdolls[0].GetHeadFixture();
	b2Vec2 impulse(150, 0);
	r->GetBody()->SetAwake(true);
	r->GetBody()->ApplyLinearImpulseToCenter(impulse, true);
}

// Destructor de la clase

Game::~Game(void)
{
}