#pragma once
#include "SFML/Graphics/Sprite.hpp"

namespace sf
{
	class RenderWindow;
}

class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	void SetSprite(const sf::Sprite& aSprite);
	void SetTargetPosition(const sf::Vector2f& aTargetPos);
	void Update(float aDT);
	void Render(sf::RenderWindow* aRenderWindow);

	sf::Sprite mySprite;
	sf::Vector2f myTargetPosition;
};

