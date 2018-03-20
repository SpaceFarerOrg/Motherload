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
	void Render(sf::RenderWindow* aRenderWindow);

	sf::Vector2f GetPosition();

private:
	
	float mySpeed;

	sf::Texture myTexture;
	sf::Sprite mySprite;
};

