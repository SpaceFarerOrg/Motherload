#include "stdafx.h"
#include "Player.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "Math.h"

CPlayer::CPlayer()
{
	myFuelAmount = 1.f;
	myHeldOres = 0;
	myScore = 0;
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
	myIsHoldingDownJump = false;

	myCanDig = true;
	myDigTimer = 0.f;
	myDigCooldown = 0.3f;

	myUIFont.loadFromFile("Fonts/Roboto-Medium.ttf");
	myUIText.setFont(myUIFont);
	myUIText.setCharacterSize(30);
}

void CPlayer::Update(float aDT)
{
	aDT;
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	//{
	//	mySprite.setPosition(768, 0);
	//	myYVelocity = 0.f;
	//	myIsGrounded = false;
	//}

	myDigTimer += aDT;
	if (myDigTimer >= myDigCooldown)
	{
		myDigTimer -= myDigCooldown;
		myCanDig = true;
	}

	if (myFuelAmount <= 0.f)
	{
		myCanDig = false;
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

	if (!myIsHoldingDownJump && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && myIsGrounded)
	{
		myYVelocity = -3.f;
		myIsGrounded = false;
	}

	myYVelocity += 9.81f * aDT;

	myYVelocity = Math::Clamp(myYVelocity, -10.f, 10.f);

	myLatestMovement.y = myYVelocity;
	mySprite.move(0, myLatestMovement.y);

	myIsHoldingDownJump = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
}

void CPlayer::Render(sf::RenderWindow * aRenderWindow)
{
	aRenderWindow->draw(mySprite);

	//Fuel range 0-1
	//Drain of 0.05 for every time dug

	myFuelAmount = Math::Clamp(myFuelAmount, 0.f, 1.f);

	sf::RectangleShape fuelbar;
	sf::RectangleShape fbbg;

	float maxFuelBarW = 64.f;
	float fuelBarH = 16.f;

	float actualFuelBarW = maxFuelBarW * myFuelAmount;

	sf::Color fuelBarColor = sf::Color::Green;

	if (myFuelAmount < 0.6f)
	{
		fuelBarColor = sf::Color::Yellow;
	}

	if (myFuelAmount < 0.3f)
	{
		fuelBarColor = sf::Color::Red;
	}

	fbbg.setSize({maxFuelBarW, fuelBarH});
	fbbg.setFillColor({ 75,75,75 });
	fbbg.setOutlineColor({10,10,10});
	fbbg.setOrigin(0.f, fbbg.getGlobalBounds().height / 2.f);
	fbbg.setPosition(mySprite.getPosition().x , mySprite.getPosition().y - mySprite.getGlobalBounds().height / 1.8f);
	aRenderWindow->draw(fbbg);

	fuelbar.setSize({ actualFuelBarW, fuelBarH });
	fuelbar.setFillColor(fuelBarColor);
	fuelbar.setOutlineThickness(0.f);
	
	fuelbar.setOrigin(0.f, fuelBarH / 2.f);
	fuelbar.setPosition(fbbg.getPosition());

	aRenderWindow->draw(fuelbar);



}

void CPlayer::SetScore(int aScore)
{
	myScore = aScore;
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

void CPlayer::SetFuelAmount(float aFuelAmount)
{
	myFuelAmount = aFuelAmount;
}

void CPlayer::GiveFuel(float aGive)
{
	myFuelAmount += aGive;
}


int CPlayer::EmptyOres()
{
	int score = 0;
	score += myHeldOres * 5;
	myHeldOres = 0;

	return score;
}

size_t CPlayer::GetHeldOres() const
{
	return myHeldOres;
}

bool CPlayer::GetCanHoldMoreOres() const
{
	return myHeldOres < myMaxHeldOres;
}

void CPlayer::GiveOre()
{
	myHeldOres++;

	myHeldOres = Math::Clamp(myHeldOres, 0u, myMaxHeldOres);
}

void CPlayer::RevertXMovement()
{
	mySprite.move(-myLatestMovement.x, 0);
}

void CPlayer::RevertYMovement()
{
	mySprite.move(0, -myLatestMovement.y);
}

void CPlayer::ResetVelocity()
{
	myYVelocity = 0.f;
}

bool CPlayer::CanDig()
{
	return myCanDig;
}

void CPlayer::Dig()
{
	myCanDig = false;
	myDigTimer = 0.f;
}

void CPlayer::RenderUI(sf::RenderWindow & aRenderWindow)
{
	myUIText.setString("Gold:" + std::to_string(myScore));
	myUIText.setPosition(5, 80);
	myUIText.setColor(sf::Color::Yellow);

	aRenderWindow.draw(myUIText);

	myUIText.setString("Ores:" + std::to_string(myHeldOres));
	myUIText.move(0, 35);
	myUIText.setColor(sf::Color::Magenta);

	aRenderWindow.draw(myUIText);
}
