#include "stdafx.h"
#include "GameObject.h"
#include "Math.h"
#include "SFML/Graphics/RenderWindow.hpp"

CGameObject::CGameObject()
{
}


CGameObject::~CGameObject()
{
}

void CGameObject::SetSprite(const sf::Sprite & aSprite)
{
	mySprite = aSprite;
}

void CGameObject::SetTargetPosition(const sf::Vector2f & aTargetPos)
{
	myTargetPosition = aTargetPos;
}

void CGameObject::Update(float aDT)
{
	mySprite.setPosition(Math::Lerp(mySprite.getPosition(), myTargetPosition, aDT / 0.1f));
}

void CGameObject::Render(sf::RenderWindow * aRenderWindow)
{
	aRenderWindow->draw(mySprite);
}
