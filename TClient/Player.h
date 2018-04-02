#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

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

	void RevertXMovement();
	void RevertYMovement();

private:
	
	float mySpeed;

	sf::Texture myTexture;
	sf::Sprite mySprite;

	sf::Vector2f myLatestMovement;
	float myYVelocity;
	bool myIsGrounded;
};

