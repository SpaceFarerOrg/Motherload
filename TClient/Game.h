#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\System\Clock.hpp>

#include <unordered_map>

#include "Player.h"
#include "GameObject.h"

#include "CommonNetworkIncludes.h"

#include "NetMessageWorldData.h"
#include "SFML\Audio\Music.hpp"


class CNetMessageManager;

class CGame
{
public:
	void SetMessageManager(CNetMessageManager& aMessageManager);

	void Init();
	void Update();
	void Render();

	void SetServerID(short aID);

	void LoadWorld(unsigned char aWidth, unsigned char aHeight, unsigned char aSkyCutOff, const std::array<unsigned short, MAX_ORE_COUNT>& aOres);

	void UpdateOtherPlayer(int aID, const sf::Vector2f& aPos);
	void UpdateOtherPlayer(int aID, float aFuelAmount);
	void AddPlayer(size_t aID);
	void RemovePlayer(size_t aID);

	void AddObject(short aID, const sf::Vector2f& aPosition);
	void UpdateObject(short aID, const sf::Vector2f& aPosition);
	void RemoveObject(short aID);

	void DestroyBlock(unsigned short aBlockID);

	void SetIsConnected(bool aIsConnected);
	void SetKbPerSecond(float aKbPerSecond);
	bool GetShouldRun() const;

	CPlayer& GetPlayer();

	sf::Vector2f GetPlayerPosition();

private:
	void HandleWindowEvents();
	void HandlePlayerCollision(float aDT);

	bool CheckCollisionWithNeighbour(unsigned short aIndex);

	bool myShouldRun;
	bool myWorldIsLoaded;

	short myServerID;

	sf::RenderWindow myWindow;
	sf::Music myMusic;


	sf::Font myDebugFont;
	sf::Text myConnectedStatus;
	sf::Text myKbPerSecond;

	sf::Clock myClock;

	sf::Texture myPlayerTexture;
	sf::Texture myCircleTexture;
	CPlayer myPlayer;
	std::unordered_map<int, sf::Sprite> myOtherPlayers;
	std::unordered_map<short, CGameObject> myGameObjects;
	std::vector<unsigned short> myDestroyBlockQueue;

	std::vector<STile> myTiles;
	unsigned char myWorldWidth;
	unsigned char myWorldHeight;

	sf::Texture myGroundTexture;
	sf::Texture myOreTexture;
	sf::Texture mySkyTexture;
	sf::Texture myDugGroundTexture;
	sf::Texture myRockTexture;
	sf::Sprite myTileSprite;

	sf::Texture myShopTexture;
	sf::Sprite myShopSprite;

	sf::Texture myGasTankTexture;
	sf::Sprite myGasTankSprite;

	CNetMessageManager* myMessageManager;
};
