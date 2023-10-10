#include"Game.h"
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>



int main(){
	srand(time(NULL));
	RenderWindow window(VideoMode(1920, 1080), "202", Style::Default);
	Clock clock;
	float dt = 0.f;

	Game game(&window);



	// Game loop
	while (window.isOpen()){
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();		
		}
		dt = clock.restart().asSeconds();

			game.UPDATE(dt);
			game.RENDER();

		
	}
	return 0;
}