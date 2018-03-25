#pragma once
#include "SFML/System/String.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Text.hpp"

#include <vector>

namespace sf
{
	class RenderWindow;
}

enum class EPivot
{
	TopRight,
	TopLeft,
	BottomRight,
	BottomLeft,
	Center
};

class CDebugDrawer
{
public:
	~CDebugDrawer();

	static CDebugDrawer& GetInstance();

	void DrawTimedText(const sf::String& aText, const sf::Vector2f& aPosition, float aDuration, const sf::Color& aColor, sf::Uint8 aCharacterSize = 30, EPivot aPivot = EPivot::TopLeft);

	void Update(float aDT);
	void Render(sf::RenderWindow* aRenderWindow);

private:
	CDebugDrawer();

	struct STimedText
	{
		sf::String myText;
		sf::Vector2f myPosition;
		float myDuration;
		sf::Color myColor;
		sf::Uint8 myCharacterSize;
		EPivot myPivot;
	};

	std::vector<STimedText> myTimedTexts;
	sf::Font myFont;
	sf::Text myText;
	sf::Uint8 myDefaultTextSize;
};

