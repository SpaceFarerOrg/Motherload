#include "stdafx.h"
#include "Player.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Keyboard.hpp"

CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
	myTexture.loadFromFile("Sprites/player.png");
	mySprite.setTexture(myTexture);

	mySpeed = 250.f;
}

void CPlayer::Update(float aDT)
{
	sf::Vector2f direction;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		direction.x += -1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		direction.x += 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		direction.y += -1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		direction.y += 1.f;
	}

	mySprite.move(direction * mySpeed * aDT);
}

void CPlayer::Render(sf::RenderWindow * aRenderWindow)
{
	aRenderWindow->draw(mySprite);
}

sf::Vector2f CPlayer::GetPosition()
{
	return mySprite.getPosition();
}
