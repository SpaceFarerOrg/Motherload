#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\System\Clock.hpp>

#include <unordered_map>

#include "Player.h"

class CGame
{
public:
	void Init();
	void Update();
	void Render();

	void UpdateOtherPlayer(int aID, const sf::Vector2f& aPos);
	void AddPlayer(size_t aID);
	void RemovePlayer(size_t aID);

	void AddObject(short aID, const sf::Vector2f& aPosition);
	void UpdateObject(short aID, const sf::Vector2f& aPosition);
	void RemoveObject(short aID);

	void SetIsConnected(bool aIsConnected);
	void SetKbPerSecond(float aKbPerSecond);
	bool GetShouldRun() const;

	sf::Vector2f GetPlayerPosition();

private:
	void HandleWindowEvents();

	bool myShouldRun;

	sf::RenderWindow myWindow;

	sf::Font myDebugFont;
	sf::Text myConnectedStatus;
	sf::Text myKbPerSecond;

	sf::Clock myClock;

	sf::Texture myPlayerTexture;
	sf::Texture myCircleTexture;
	CPlayer myPlayer;
	std::unordered_map<int, sf::Sprite> myOtherPlayers;
	std::unordered_map<short, sf::Sprite> myGameObjects;

};
