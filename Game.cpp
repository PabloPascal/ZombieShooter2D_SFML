#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"

using namespace sf;


int main() {

	enum class State {PAUSED, LEVELING_UP, GAME_OVER, PLAYING};
	State state = State::LEVELING_UP;
	
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Game");

	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	Clock clock;

	Time gameTimeTotal;

	Vector2f mouseWorldPosition;

	Vector2i mouseScreenPosition;

	//игрок
	Player player;

	IntRect arena;

	//vertexes
	VertexArray background;
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/texture.png");


	while (window.isOpen()) {

		Event event;
		while (window.pollEvent(event)) {

			if (event.key.code == Keyboard::Return && state == State::PLAYING) {
				state = State::PAUSED;
			}

			if (event.key.code == Keyboard::Return && state == State::PAUSED) {
				state = State::PLAYING;
			}

			else if (event.key.code == Keyboard::Return && state == State::PAUSED) {

				state = State::PLAYING;
				clock.restart();

			}

			else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
				state = State::LEVELING_UP;
			}

			if (state == State::PLAYING) {



			}

			if (event.type == Event::Closed) {
				window.close();
			}
		}//end of poll events

		if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();

		//контроль над персонажем
		if (state == State::PLAYING) {

			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.moveUp();
			}
			else 
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.moveDown();
			}
			else 
			{
				player.stopDown();
			}
			
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.moveLeft();
			}
			else 
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.moveRight();
			}
			else 
			{
				player.stopRight();
			}


		}//end of WASD keys


		if (state == State::LEVELING_UP) {

			if (event.key.code == Keyboard::Num1) {
				state == State::PLAYING;
			}
			if (event.key.code == Keyboard::Num2) {
				state == State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3) {
				state == State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4) {
				state == State::PLAYING;
			}
			if (Keyboard::isKeyPressed(Keyboard::X)) {
				state = State::PLAYING;
			}

			if (state == State::PLAYING) {

				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);

				player.spawn(arena, resolution, tileSize);
				

				clock.restart();
			}
			
		}//end leveling up


		if (state == State::PLAYING) {
			arena.width = 500;
			arena.height = 500;
			arena.left = 0;
			arena.top = 0;

			int tileSize = createBackground(background, arena);

			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();
			
			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			//update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			Vector2f PlayerPosition(player.getCenter());

			mainView.setCenter(player.getCenter());

		}//end updating scene 

		if (state == State::PLAYING) {

			window.clear();

			window.setView(mainView);
			//draw map
			window.draw(background, &textureBackground);
			//draw player
			window.draw(player.getSprite());
		}
		if(state == State::LEVELING_UP)
		{
		}
		if(state == State::PAUSED)
		{
		}
		if(state == State::GAME_OVER)
		{
		}


		window.display();
	}//main loop of game

	return 0;
}
