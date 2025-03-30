#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>
#include <chrono>   // Para el random de las balas

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

}

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

    // Control de rotación del cañón
    float rotationSpeed = 1.5f;  // Velocidad de rotación
    float currentAngle = controlBody->GetAngle();

    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        controlBody->SetTransform(controlBody->GetPosition(), currentAngle - rotationSpeed * frameTime);
    }
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        controlBody->SetTransform(controlBody->GetPosition(), currentAngle + rotationSpeed * frameTime);
    }

    // Disparo...exceso de disparos! Se puede dejar apretada la spacebar.
    if (Keyboard::isKeyPressed(Keyboard::Space))
        this->Shoot();
}

void Game::Shoot()
{
    // Definir el cuerpo del proyectil
    b2BodyDef bulletBodyDef;
    bulletBodyDef.type = b2_dynamicBody;

    // Obtener la posición actual del cañón (controlBody)
    b2Vec2 cannonPosition = controlBody->GetPosition();
    float cannonAngle = controlBody->GetAngle();

    // Posicionar el proyectil en la punta del cañón
    float offset = 7.0f;  // Distancia desde el centro del cañón, sin esto es como que se dispara desde adentro del cañon y al no poder ocupar el mismo espacio se van para cualquier lado.
    // coseno del angulo del cañoón * el offset, lo mismo con seno. Le doy dirección al versor y despues le doy magnitud.
    b2Vec2 bulletPosition = cannonPosition + b2Vec2(
        cos(cannonAngle) * offset,
        sin(cannonAngle) * offset
    );
    bulletBodyDef.position = bulletPosition;

    // Crear el cuerpo del proyectil en el physic world
    b2Body* bullet = phyWorld->CreateBody(&bulletBodyDef);

    // Definir la forma del proyectil ( O )
    b2CircleShape bulletShape;
    bulletShape.m_radius = 1.5f;  // Tamaño de cada bala

    // Definir un fixture con las propiedades fisicas de las balas/proyectiles
    b2FixtureDef bulletFixture;
    bulletFixture.shape = &bulletShape;
    bulletFixture.density = 1.0f;
    bulletFixture.friction = 0.0f;
    bulletFixture.restitution = 0.1f;  // Rebotan apenitas

    bullet->CreateFixture(&bulletFixture);

    // Aplicar una velocidad en la dirección del cañón
    float bulletSpeed = 150.0f;  // Velocidad de las balas. Mucha.

    // A continuacion viene una sobre-edificacion exagerada e innecesaria de codigo solamente
    // para que las balas salgan de forma "ligeramente random" y parezca
    // una verdadera ametralladora de balas. Un bullet hell.
    // Para eso necesitaba afectar el vector de direccion (versor)
    // no solo por la direccion necesaria y la magnitud sino tambien
    // por un ligero factor random en cada caso.
    // 
    // Obtener el tiempo en milisegundos desde el inicio del programa (para la semilla)
    auto timeNow = std::chrono::high_resolution_clock::now();
    auto timeInMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow.time_since_epoch()).count();

    // Usar ese tiempo como semilla para llamar a srand
    srand(static_cast<unsigned>(timeInMilliseconds));

    float randomnessFactor = 0.9f;  // Ajusta cuánto ruido quiero
    float randomX = (static_cast<float>(rand()) / RAND_MAX) * 75.0f * randomnessFactor - randomnessFactor;
    float randomY = (static_cast<float>(rand()) / RAND_MAX) * 75.0f * randomnessFactor - randomnessFactor;

    // La velocidad (que no la rapidez) tambien es un vector asi que tengo que usar cos, sin
    // y aca es donde finalmente agrego randomX y randomY para alterar las trayectorias de las balas
    b2Vec2 bulletVelocity(
        cos(cannonAngle) * bulletSpeed + randomX,
        sin(cannonAngle) * bulletSpeed + randomY
    );
    bullet->SetLinearVelocity(bulletVelocity);
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

void Game::InitPhysics()
{
    // Inicializar el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo y las paredes estáticas
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 75.0f), .0f);

    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    // -------------------- CREAR EL CAÑÓN --------------------

    // Definir el cuerpo del cañón
    b2BodyDef cannonDef;
    cannonDef.type = b2_staticBody;  // No se mueve (pero si se puede rotar)
    cannonDef.position.Set(5.0f, 50.0f);  // Fijo a la izquierda
    cannonDef.angle = 0.0f;  // Inicialmente recto

    // Crear el cuerpo en el mundophy
    controlBody = phyWorld->CreateBody(&cannonDef);

    // Definir la forma del cañón (rectangulo)
    b2PolygonShape cannonShape;
    cannonShape.SetAsBox(5.0f, 1.5f);

    // Crear fixture para que sea sólido
    b2FixtureDef cannonFixture;
    cannonFixture.shape = &cannonShape;
    cannonFixture.density = 1.0f;
    cannonFixture.friction = 0.3f;

    controlBody->CreateFixture(&cannonFixture);
}


// Destructor de la clase

Game::~Game(void)
{
}