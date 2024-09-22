#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include <sstream>
#include <iostream>

using namespace sf;


int main() {

	TextureHolder holder;


	enum class State {PAUSED, LEVELING_UP, GAME_OVER, PLAYING};
	State state = State::GAME_OVER;
	
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	//главное окно
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Game");

	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	//время игры
	Clock clock;
	Time gameTimeTotal;

	//определение позиции мыши
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	//игрок
	Player player;

	IntRect arena;

	//задний фон
	VertexArray background;
	Texture textureBackground = TextureHolder::GetTexture("graphics/texture.png");

	// переменные для волны зомби
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	//пуля
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;

	//когда было последнее нажатие на кнопки стрелять
	Time lastPressed;

	//прицел 
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");

	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	//пикапы
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	//игровая информация
	int score = 0;
	int hiscore = 0;

	//окно окончания игры
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/gameMenu.jpg");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);


	//панель информации
	View hudView(FloatRect(0,0, resolution.x, resolution.y));

	//спрайт иконки патрон
	Sprite spriteAmmoIcon;
	Texture textureAmmo = TextureHolder::GetTexture("graphics/ammo.png");

	spriteAmmoIcon.setTexture(textureAmmo);
	spriteAmmoIcon.setPosition(20, 980);

	//загрузка шрифта
	Font font;
	font.loadFromFile("font/zombiecontrol.ttf");

	//пауза
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("press enter to continue");

	// Окончание игры
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");

	// LEVELING up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	// патроны
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiscore;
	hiScoreText.setString(s.str());

	// осталось зомби
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");

	// номер волны
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	// жизни
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	//когда было сделано последнее обновление
	int framesSinceLastHUDUpdate = 0;

	// как часто мы должны обновлять иконки
	int fpsMeasurementFrameInterval = 1000;

	while (window.isOpen()) {
		
		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::KeyPressed) {
				//паузим игру
				if (event.key.code == Keyboard::Return && state == State::PLAYING) {
					state = State::PAUSED;
				}

				//возвращаемся в игру после паузы
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

					// перезарядка
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							// Plenty of bullets. Reload.
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if (bulletsSpare > 0)
						{
							// Only few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else
						{
							// More here soon?!
						}
					}

				}
			}
			if (event.type == Event::Closed) {
				window.close();
			}

		}//end of poll events

		if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();

		//контроль над персонажем ходьба
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

			//нажатие левой кнопки мыши для стрельбы 
			if (Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - 
					lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					
					bullets[currentBullet].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;

					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}
			}// End fire a bullet


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
				
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);


				//создаем орду зомби
				numZombies = 10;

				// удаляем предыдущую выделенную память (если она есть)
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				clock.restart();
			}
			
		}//end leveling up

		//update
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

			spriteCrosshair.setPosition(mouseWorldPosition);

			//update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			Vector2f PlayerPosition = player.getCenter();

			mainView.setCenter(player.getCenter());

			//updating zombies 
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), PlayerPosition);
				}
			}

			for (int i = 0; i < 100; i++) {
				if(bullets[i].isInFlight()){
					bullets[i].update(dtAsSeconds);
				}

			}
			//вызов апдейт у предметов
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			//проверка попали ли мы в зомбака
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < numZombies; j++) {
					if (bullets[i].isInFlight() && zombies[j].isAlive()) {
						if (bullets[i].getPosition().intersects(zombies[j].getPosition())){
							//если пересекли, то останавливаем пулю
							bullets[i].stop();
							if (zombies[j].hit()) {
								//если уже убили
								score += 10;
								score = max(hiscore, score);
								numZombiesAlive--;
								//когда все сдохнут, обновляем уровень
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}

							}
						}

					}
				}
			}

			//проверка бьет ли зомби игрока
			for (int i = 0; i < numZombies; i++) {

				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive()) {

					if (player.hit(gameTimeTotal)) {

					}
					if (player.getHealth() <= 0) {
						state = State::GAME_OVER;
					}


				}

			}

			//подобрал ли игрок предмет
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {

				bulletsSpare += ammoPickup.gotIt();
				
			}

			// size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			// Increment the number of frames since the previous update
			framesSinceLastHUDUpdate++;
			// re-calculate every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				// Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				// Update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());
				// Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());
				// Update the high score text
				ssHiScore << "Hi Score:" << hiscore;
				hiScoreText.setString(ssHiScore.str());
				// Update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());
				// Update the high score text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());
				framesSinceLastHUDUpdate = 0;
			}

			if (player.getPosition().intersects(healthPickup.getPosition())&& healthPickup.isSpawned()) {

				player.increaseHealthLevel(healthPickup.gotIt());

			}


		}//end updating scene 

		//отрисовка кадров
		if (state == State::PLAYING) {

			//отчистка карты
			window.clear();

			window.setView(mainView);

			//отрисовка карты
			window.draw(background, &textureBackground);

			//отрисовка зомби
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}

			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight()) {
					window.draw(bullets[i].getShape());
				}
			}

			//отрисовка предметов
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}

			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}

			//отрисовка игрока
			window.draw(player.getSprite());

			//отрисовка прицела
			window.draw(spriteCrosshair);

			// Switch to the HUD view
			window.setView(hudView);
			// отрисовка иконок
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);

		}
		if(state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if(state == State::PAUSED)
		{
			window.draw(pausedText);
		}
		if(state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}


		window.display();
	}//основной цикл игры

	//удаляем зомби
	delete[] zombies;

	return 0;
}
