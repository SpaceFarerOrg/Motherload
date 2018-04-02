#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\System\Clock.hpp>

#include <unordered_map>

#include "Player.h"
#include "GameObject.h"

#include "CommonNetworkIncludes.h"

#include "NetMessageWorldData.h"

class CNetMessageManager;

class CGame
{
public:
	void SetMessageManager(CNetMessageManager& aMessageManager);

	void Init();
	void Update();
	void Render();

	void LoadWorld(unsigned char aWidth, unsigned char aHeight, unsigned char aSkyCutOff, const std::array<unsigned short, MAX_ORE_COUNT>& aOres);

	void UpdateOtherPlayer(int aID, const sf::Vector2f& aPos);
	void AddPlayer(size_t aID);
	void RemovePlayer(size_t aID);

	void AddObject(short aID, const sf::Vector2f& aPosition);
	void UpdateObject(short aID, const sf::Vector2f& aPosition);
	void RemoveObject(short aID);

	void DestroyBlock(unsigned short aBlockID);

	void SetIsConnected(bool aIsConnected);
	void SetKbPerSecond(float aKbPerSecond);
	bool GetShouldRun() const;

	sf::Vector2f GetPlayerPosition();

private:
	void HandleWindowEvents();
	void HandlePlayerCollision(float aDT);

	bool CheckCollisionWithNeighbour(unsigned short aIndex);

	bool myShouldRun;
	bool myWorldIsLoaded;

	sf::RenderWindow myWindow;

	sf::Font myDebugFont;
	sf::Text myConnectedStatus;
	sf::Text myKbPerSecond;

	sf::Clock myClock;

	sf::Texture myPlayerTexture;
	sf::Texture myCircleTexture;
	CPlayer myPlayer;
	std::unordered_map<int, sf::Sprite> myOtherPlayers;
	std::unordered_map<short, CGameObject> myGameObjects;

	std::vector<STile> myTiles;
	unsigned char myWorldWidth;
	unsigned char myWorldHeight;

	sf::Texture myGroundTexture;
	sf::Texture myOreTexture;
	sf::Texture mySkyTexture;
	sf::Sprite myTileSprite;

	CNetMessageManager* myMessageManager;
};
