#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;


class Bullet
{
private:
	Vector2f m_Position;

	RectangleShape m_BulletShape;

	//пуля в полете? 
	bool m_InFlight = false;

	// скорость пули?
	float m_BulletSpeed = 1000;

	// What fraction of 1 pixel does the bullet travel, 
	// Horizontally and vertically each frame?
	// These values will be derived from m_BulletSpeed
	float m_BulletDistanceX;
	float m_BulletDistanceY;

	//границы, после которых пуля перестает летать
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

public:
	// конструктор
	Bullet();

	// функция остановки пули
	void stop();

	// летит ли пуля?
	bool isInFlight();


	void shoot(float startX, float startY, float xTarget, float yTarget);

	// позиция пули
	FloatRect getPosition();


	RectangleShape getShape();

	void update(float elapsedTime);
};
