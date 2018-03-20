#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Text.hpp>

class CGame
{
public:
	void Init();
	void Update();

	void SetIsConnected(bool aIsConnected);
	bool GetShouldRun() const;
private:
	void HandleWindowEvents();

	bool myShouldRun;

	sf::RenderWindow myWindow;

	sf::Font myDebugFont;
	sf::Text myConnectedStatus;
};
