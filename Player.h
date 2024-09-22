#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Player {
	const int START_SPEED = 200;
	const int START_HEALTH = 1000;

	Vector2f m_Position; //wheres the player

	Image m_Image;

	Sprite m_Sprite; //player sprite

	Texture m_Texture;

	Vector2f m_Resolution; //разрешение экрана

	IntRect m_Arena;

	int m_TileSize;

	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;
	
	//how much has health player got
	int m_Health;

	// What is the maximum health the player can have
	int m_MaxHealth;

	Time m_LastHit;

	float m_Speed;

protected:

public:
	Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	// Call this at the end of every game
	void resetPlayerStats();

	// Handle the player getting hit by a zombie
	bool hit(Time timeHit);

	// How long ago was the player last hit
	Time getLastHitTime();

	// Where is the player
	FloatRect getPosition();

	// Where is the center of the player
	Vector2f getCenter();

	// What angle is the player facing
	float getRotation();

	// Send a copy of the sprite to the main function
	Sprite getSprite();

	// The next four functions move the player
	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	// Stop the player moving in a specific direction
	void stopLeft();

	void stopRight();

	void stopUp();

	void stopDown();

	// We will call this function once every frame
	void update(float elapsedTime, Vector2i mousePosition);

	// Give the player a speed boost
	void upgradeSpeed();

	// Give the player some health
	void upgradeHealth();

	// Increase the maximum amount of health the player can have
	void increaseHealthLevel(int amount);

	// How much health has the player currently got?
	int getHealth();

};

