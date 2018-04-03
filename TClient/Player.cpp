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
	//mySprite.setOrigin(myTexture.getSize().x / 2, myTexture.getSize().y / 2);

	mySpeed = 250.f;
	myYVelocity = 0.f;
	myIsGrounded = true;
}

void CPlayer::Update(float aDT)
{
	aDT;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		mySprite.setPosition(768, 0);
		myYVelocity = 0.f;
		myIsGrounded = false;
	}
}

void CPlayer::UpdateX(float aDT)
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

	myLatestMovement.x = direction.x * mySpeed * aDT;
	mySprite.move(myLatestMovement.x, 0);
}

void CPlayer::UpdateY(float aDT)
{
	sf::Vector2f direction;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && myIsGrounded)
	{
		myYVelocity = -3.f;
		myIsGrounded = false;
	}

	myYVelocity += 9.81f * aDT;
	myLatestMovement.y = myYVelocity + direction.y * mySpeed * aDT;
	mySprite.move(0, myLatestMovement.y);
}

void CPlayer::Render(sf::RenderWindow * aRenderWindow)
{
	aRenderWindow->draw(mySprite);
}

sf::Vector2f CPlayer::GetPosition()
{
	return mySprite.getPosition();
}

void CPlayer::SetPosition(const sf::Vector2f & aPosition)
{
	mySprite.setPosition(aPosition);
}

void CPlayer::SetIsGrounded(bool aIsGrounded)
{
	myIsGrounded = aIsGrounded;
	myYVelocity = 0.f;
}

short CPlayer::GetFacingBlock()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		return 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		return 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		return 2;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		return 3;

	return -1;
}

bool CPlayer::Intersects(const sf::FloatRect & aRect)
{
	sf::FloatRect offset = aRect;
	//offset.top += mySprite.getGlobalBounds().width / 2;
	//offset.left += mySprite.getGlobalBounds().height / 2;

	sf::FloatRect playerRect = mySprite.getGlobalBounds();

	playerRect.left += 10;
	playerRect.width -= 20;

	return playerRect.intersects(offset);
}

void CPlayer::RevertXMovement()
{
	mySprite.move(-myLatestMovement.x, 0);
}

void CPlayer::RevertYMovement()
{
	mySprite.move(0, -myLatestMovement.y);
}
