#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;


void Zombie::spawn(float startX, float startY, int type, int seed)
{
	switch (type)
	{
	case 0:
		//Bloater
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		m_Speed = BLOATER_SPEED;
		m_Health = BLOATER_HEALTH;
		break;

	case 1:
		//Chaser
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/ok.png"));
		m_Speed = CHASER_SPEED;
		m_Health = CHASER_HEALTH;
		break;

	case 2:
		//Crawler
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		m_Speed = CRAWLER_SPEED;
		m_Health = CRAWLER_HEALTH;
		break;
	}
	//��� ������������ ������� ����� ������� ����������� ��������, ����� ��� �� ������������ � �����
	srand((int)time(0)*seed);

	//������� ���������� ����� 80-100
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET;

	modifier /= 100;

	//���������� ��� ��� ���������
	m_Position.x = startX;
	m_Position.y = startY;

	m_Sprite.setOrigin(25, 25);

	m_Sprite.setPosition(m_Position);
}


bool Zombie::hit()
{
	m_Health--;
	if (m_Health < 0)
	{
		// dead
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}
	// 
	return false;
}


bool Zombie::isAlive()
{
	return m_Alive;
}

FloatRect Zombie::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Zombie::getSprite()
{
	return m_Sprite;
}


void Zombie::update(float elapsedTime,
	Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	// Update the zombie position variables
	//����� ��������� ����� ������ � ���� �������
	if (playerX > m_Position.x)
	{
		m_Position.x = m_Position.x +
			m_Speed * elapsedTime;
	}
	if (playerY > m_Position.y)
	{
		m_Position.y = m_Position.y +
			m_Speed * elapsedTime;
	}

	if (playerX < m_Position.x)
	{
		m_Position.x = m_Position.x -
			m_Speed * elapsedTime;
	}
	if (playerY < m_Position.y)
	{
		m_Position.y = m_Position.y -
			m_Speed * elapsedTime;
	}

	// Move the sprite
	m_Sprite.setPosition(m_Position);

	// Face the sprite in the correct direction
	float angle = (atan2(playerY - m_Position.y, playerX - m_Position.x) * 180) / 3.141;
	m_Sprite.setRotation(angle);
}
