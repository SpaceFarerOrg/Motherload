#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

namespace sf
{
	class RenderWindow;
}

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	void Init();
	void Update(float aDT);
	void UpdateX(float aDT);
	void UpdateY(float aDT);
	void Render(sf::RenderWindow* aRenderWindow);

	sf::Vector2f GetPosition();
	void SetPosition(const sf::Vector2f& aPosition);

	void SetIsGrounded(bool aIsGrounded);
	short GetFacingBlock();

	bool Intersects(const sf::FloatRect& aRect);

	void SetFuelAmount(float aFuelAmount);
	void GiveFuel(float aGive);

	void RevertXMovement();
	void RevertYMovement();

	void ResetVelocity();

	bool CanDig();
	void Dig();

private:
	
	float mySpeed;

	sf::Texture myTexture;
	sf::Sprite mySprite;

	sf::Vector2f myLatestMovement;
	float myYVelocity;
	bool myIsGrounded;
	bool myIsHoldingDownJump;

	float myFuelAmount;

	bool myCanDig;
	float myDigTimer;
	float myDigCooldown;
};

