#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

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

	void SetScore(int aScore);

	sf::Vector2f GetPosition();
	void SetPosition(const sf::Vector2f& aPosition);

	void SetIsGrounded(bool aIsGrounded);
	short GetFacingBlock();

	bool Intersects(const sf::FloatRect& aRect);

	void SetFuelAmount(float aFuelAmount);
	void GiveFuel(float aGive);

	int EmptyOres();
	size_t GetHeldOres() const;
	bool GetCanHoldMoreOres() const;
	void GiveOre();

	void RevertXMovement();
	void RevertYMovement();

	void ResetVelocity();

	bool CanDig();
	void Dig();

	void RenderUI(sf::RenderWindow& aRenderWindow);

private:
	
	float mySpeed;


	size_t myScore;
	size_t myHeldOres;
	const size_t myMaxHeldOres = 10;

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

	sf::Text myUIText;
	sf::Font myUIFont;
};

