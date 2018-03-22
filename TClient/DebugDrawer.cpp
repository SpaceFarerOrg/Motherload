#include "stdafx.h"
#include "SFML/Graphics/RenderWindow.hpp"

#include "DebugDrawer.h"
#include "Math.h"


CDebugDrawer::CDebugDrawer()
{
	myDefaultTextSize = 30;
	myFont.loadFromFile("Fonts/Roboto-Medium.ttf");
	myText.setFont(myFont);

}


CDebugDrawer::~CDebugDrawer()
{
}

CDebugDrawer & CDebugDrawer::GetInstance()
{
	static CDebugDrawer instance;
	return instance;
}

void CDebugDrawer::DrawTimedText(const sf::String & aText, const sf::Vector2f & aPosition, float aDuration, const sf::Color & aColor, sf::Uint8 aCharacterSize, EPivot aPivot)
{
	STimedText newText;
	newText.myText = aText;
	newText.myPosition = aPosition;
	newText.myDuration = aDuration;
	newText.myColor = aColor;
	newText.myCharacterSize = aCharacterSize;
	newText.myPivot = aPivot;

	myTimedTexts.push_back(newText);
}

void CDebugDrawer::Update(float aDT)
{
	for (int i = 0; i < myTimedTexts.size(); ++i)
	{
		STimedText& text = myTimedTexts[i];

		text.myDuration -= aDT;
		text.myColor.a = 255.f * Math::Clamp(text.myDuration, 0.f, 1.f);

		if (text.myDuration < 0)
		{
			myTimedTexts[i] = myTimedTexts.back();
			myTimedTexts.pop_back();
		}
	}
}

void CDebugDrawer::Render(sf::RenderWindow * aRenderWindow)
{
	sf::Vector2f origin;

	for (STimedText& text : myTimedTexts)
	{
		myText.setPosition(text.myPosition);
		myText.setCharacterSize(text.myCharacterSize);
		myText.setColor(text.myColor);
		myText.setString(text.myText);

		switch (text.myPivot)
		{
		case EPivot::Center:
			origin = sf::Vector2f(0.5f, 0.5f);
		default:
			break;
		}

		origin = sf::Vector2f(origin.x * myText.getLocalBounds().width, origin.y * myText.getLocalBounds().height);
		myText.setOrigin(origin);

		aRenderWindow->draw(myText);
	}

	myText.setOrigin(sf::Vector2f());
}
