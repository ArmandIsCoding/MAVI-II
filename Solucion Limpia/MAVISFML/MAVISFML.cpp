//////Bibliotecas//////
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
///Punto de entrada a la aplicación 2 ///


int main() {
	//Creamos la ventana
	sf::RenderWindow App(sf::VideoMode(1920, 1080, 32),
		"Que ventana horrible");
	// Loop principal
	while (App.isOpen())
	{
		// Limpiamos la ventana
		App.clear();
		//Dibujamos la escena
		 // Mostramos la ventana
		App.display();
	}
	return 0;
}
