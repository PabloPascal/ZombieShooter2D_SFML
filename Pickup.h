#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup
{
private:
	//начальные значения
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	Sprite m_Sprite;

	IntRect m_Arena;

	int m_Value;

	//тип предмета (1 == жизнь, 2 == патроны)
	int m_Type;
	
	//управление появлением и исчезновением
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

public:
	Pickup(int type);
	// подготовка к новому сбору

	void setArena(IntRect arena);

	void spawn();

	// позиция предмета
	FloatRect getPosition();

	// спрайт
	Sprite getSprite();

	//ф. апдейт
	void update(float elapsedTime);

	// предмет был запванен?
	bool isSpawned();

	//подобрал ли игрок
	int gotIt();

	//улучшение
	void upgrade();

};
