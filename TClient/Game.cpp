#include "stdafx.h"
#include "Game.h"
#include "Math.h"
#include "DebugDrawer.h"
#include "NetMessageDestroyBlock.h"
#include "NetMessageManager.h"
#include "sfml/Window/Keyboard.hpp"
#include "sfml/Graphics/View.hpp"

void CGame::SetMessageManager(CNetMessageManager& aMessageManager)
{
	myMessageManager = &aMessageManager;
}

void CGame::Init()
{
	sf::VideoMode vm;
	vm = sf::VideoMode::getDesktopMode();

	vm.width = 1600;
	vm.height = 900;

	myWindow.create(vm, "Mother", sf::Style::Close);

	myDebugFont.loadFromFile("Fonts/Roboto-Medium.ttf");
	myConnectedStatus.setFont(myDebugFont);
	myConnectedStatus.setCharacterSize(30);
	myConnectedStatus.setPosition(5, 5);

	myKbPerSecond.setFont(myDebugFont);
	myKbPerSecond.setCharacterSize(30);
	myKbPerSecond.setPosition(5,40.f);


	myConnectedStatus.setString("Not Connected");

	myPlayer.Init();

	myShouldRun = true;
	myPlayerTexture.loadFromFile("sprites/player.png");
	myCircleTexture.loadFromFile("sprites/circle.png");

	myGroundTexture.loadFromFile("Sprites/ground.png");
	mySkyTexture.loadFromFile("Sprites/sky.png");
	myOreTexture.loadFromFile("Sprites/ore.png");
	myDugGroundTexture.loadFromFile("Sprites/background.png");
	myRockTexture.loadFromFile("Sprites/rock.png");

	myGasTankTexture.loadFromFile("Sprites/gas.png");
	myGasTankSprite.setTexture(myGasTankTexture);

	//myWindow.setFramerateLimit(60);
}

void CGame::Update()
{
	HandleWindowEvents();

	float dt = myClock.getElapsedTime().asSeconds();
	myClock.restart();

	sf::View view;
	view = myWindow.getDefaultView();
	sf::Vector2f newCenter = Math::Lerp(myWindow.getView().getCenter(), { 800.f, myPlayer.GetPosition().y }, dt / 0.5f);
	newCenter.y = Math::Clamp(newCenter.y, 450.f, myWorldHeight * 64.f - 450.f);
	view.setCenter(newCenter);
	myWindow.setView(view);


	if (myWorldIsLoaded)
	{
		HandlePlayerCollision(dt);
		myPlayer.Update(dt);

		if (myPlayer.CanDig() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			unsigned short index = (unsigned short)(myPlayer.GetPosition().x+32) / 64 + myWorldWidth * (unsigned short)((myPlayer.GetPosition().y + 32) / 64);

			bool shouldDestroy = true;
			short direction = myPlayer.GetFacingBlock();
			switch (direction)
			{
			case 0:
				direction = 1;
				break;
			case 1:
				direction = myWorldWidth;
				break;
			case 2:
				direction = -1;
				break;
			case 3:
				direction = -myWorldWidth;
				break;
			case -1:
				shouldDestroy = false;
				break;
			}

			direction += index;

			if (direction < 0 || direction >= myTiles.size())
				shouldDestroy = false;

			if (shouldDestroy)
			{
				myPlayer.Dig();

				CNetMessageDestroyBlock::SDestroyBlockData data;
				data.myTargetID = 1;
				data.myBlockID = direction;
				

				myMessageManager->CreateGuaranteedMessage<CNetMessageDestroyBlock>(data);
			}
		}

		for (auto& object : myGameObjects)
		{
			object.second.Update(dt);
		}

		CDebugDrawer::GetInstance().Update(dt);

		for (short id : myDestroyBlockQueue)
		{
			if (myTiles.size() > id)
				myTiles[id].myIsDestroyed = true;
		}

		Render();
	}
}

void CGame::Render()
{
	myWindow.clear(sf::Color(50, 150, 250));

	for (int i = 0; i < myTiles.size(); ++i)
	{
		unsigned char x = i % myWorldWidth;
		unsigned char y = i / myWorldWidth;

		sf::Texture* texture = nullptr;

		if (myTiles[i].myIsDestroyed == false)
		{
			switch (myTiles[i].myType)
			{
			case ETileType::Ground:
				texture = &myGroundTexture;
				break;
			case ETileType::Ore:
				texture = &myOreTexture;
				break;
			case ETileType::Sky:
				texture = &mySkyTexture;
				break;
			case ETileType::Rock:
				texture = &myRockTexture;
				break;
			}
		}
		else
		{
			texture = &myDugGroundTexture;
		}

		if (texture != nullptr)
		{
			myTileSprite.setTexture(*texture);
			myTileSprite.setPosition(x * 64, y * 64);

			myWindow.draw(myTileSprite);
		}
	}

	myWindow.draw(myGasTankSprite);

	for (auto& pair : myOtherPlayers)
	{
		myWindow.draw(pair.second);
	}

	for (auto& pair : myGameObjects)
	{
		pair.second.Render(&myWindow);
	}

	myPlayer.Render(&myWindow);

	myWindow.draw(myConnectedStatus);

	myWindow.draw(myKbPerSecond);

	CDebugDrawer::GetInstance().Render(&myWindow);

	myWindow.display();
}

void CGame::LoadWorld(unsigned char aWidth, unsigned char aHeight, unsigned char aSkyCutOff, const std::array<unsigned short, MAX_ORE_COUNT>& aOres)
{
	myWorldIsLoaded = true;

	myWorldWidth = aWidth;
	myWorldHeight = aHeight;

	STile defaultTile;
	defaultTile.myIsDestroyed = false;
	defaultTile.myType = ETileType::Ground;

	myGasTankSprite.setOrigin(myGasTankTexture.getSize().x / 2, myGasTankTexture.getSize().y);
	myGasTankSprite.setPosition(800.f, aSkyCutOff * 64);

	myTiles.clear();

	for (int i = 0; i < aWidth * aHeight; ++i)
	{
		myTiles.push_back(defaultTile);

		unsigned char x = i % aWidth;
		unsigned char y = i / aWidth;

		STile& currentTile = myTiles.back();

		if (y < aSkyCutOff)
		{
			currentTile.myType = ETileType::Sky;
		}
		else if (y == aSkyCutOff && x > 4 && x < myWorldWidth - 5)
		{
			currentTile.myType = ETileType::Rock;
		}
	}

	for (int i = 0; i < aOres.size(); ++i)
	{
		if (aOres[i] == 0)
			break;

		myTiles[aOres[i]].myType = ETileType::Ore;
	}
}

void CGame::UpdateOtherPlayer(int aID, const sf::Vector2f & aPos)
{
	myOtherPlayers[aID].setPosition(aPos);
}

void CGame::AddPlayer(size_t aID)
{
	sf::Sprite newSprite;
	newSprite.setTexture(myPlayerTexture);

	myOtherPlayers.insert(std::make_pair(aID, newSprite));
}

void CGame::RemovePlayer(size_t aID)
{
	CDebugDrawer::GetInstance().DrawTimedText("Player Disconnected :-(", myOtherPlayers[aID].getPosition(), 5.f, sf::Color::Red, 30, EPivot::Center);
	myOtherPlayers.erase(aID);
}

void CGame::AddObject(short aID, const sf::Vector2f & aPosition)
{
	sf::Sprite newSprite;
	newSprite.setOrigin(32, 32);
	newSprite.setTexture(myCircleTexture);
	newSprite.setPosition(aPosition);

	CGameObject newObject;
	newObject.SetTargetPosition(aPosition);
	newObject.SetSprite(newSprite);

	myGameObjects.insert(std::pair<short, CGameObject>(aID, newObject));
}

void CGame::UpdateObject(short aID, const sf::Vector2f & aPosition)
{
	myGameObjects[aID].SetTargetPosition(aPosition);
}

void CGame::RemoveObject(short aID)
{
	myGameObjects.erase(aID);
}

void CGame::DestroyBlock(unsigned short aBlockID)
{
	if (myTiles.size() < aBlockID)
	{
		myDestroyBlockQueue.push_back(aBlockID);
	}
	else
	{
		myTiles[aBlockID].myIsDestroyed = true;
	}
}

void CGame::SetIsConnected(bool aIsConnected)
{
	if (aIsConnected)
	{
		myConnectedStatus.setString("Connected");
		myConnectedStatus.setFillColor(sf::Color::Green);
	}
	else
	{
		myConnectedStatus.setString("Not Connected");
		myConnectedStatus.setFillColor(sf::Color::Red);
	}
}

void CGame::SetKbPerSecond(float aKbPerSecond)
{
	myKbPerSecond.setString("kb/s:" + std::to_string(aKbPerSecond));
}

bool CGame::GetShouldRun() const
{
	return myShouldRun;
}

CPlayer & CGame::GetPlayer()
{
	return myPlayer;
}

sf::Vector2f CGame::GetPlayerPosition()
{
	return myPlayer.GetPosition();
}

#include <SFML\Window\Event.hpp>
void CGame::HandleWindowEvents()
{
	sf::Event e;

	while (myWindow.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			myShouldRun = false;
		}
	}
}

void CGame::HandlePlayerCollision(float aDT)
{
	unsigned short index = (unsigned short)myPlayer.GetPosition().x / 64 + myWorldWidth * (unsigned short)(myPlayer.GetPosition().y / 64);

	CDebugDrawer::GetInstance().DrawTimedText(std::to_string(index), { 10, 200 }, 1.f, sf::Color::Black);

	myPlayer.UpdateX(aDT);

	if (CheckCollisionWithNeighbour(index) || 
		CheckCollisionWithNeighbour(index + 1) ||
		CheckCollisionWithNeighbour(index + 1 + myWorldWidth) || 
		CheckCollisionWithNeighbour(index + myWorldWidth) ||
		CheckCollisionWithNeighbour(index - 1 + myWorldWidth) ||
		CheckCollisionWithNeighbour(index - 1) ||
		CheckCollisionWithNeighbour(index - 1 - myWorldWidth) ||
		CheckCollisionWithNeighbour(index - myWorldWidth) ||
		CheckCollisionWithNeighbour(index + 1 - myWorldWidth))
	{
		myPlayer.RevertXMovement();
	}

	myPlayer.UpdateY(aDT);

	if (CheckCollisionWithNeighbour(index + 1 + myWorldWidth) ||
		CheckCollisionWithNeighbour(index + myWorldWidth) ||
		CheckCollisionWithNeighbour(index - 1 + myWorldWidth))
	{
		myPlayer.SetIsGrounded(true);
	}

	if (CheckCollisionWithNeighbour(index - 1 - myWorldWidth) ||
		CheckCollisionWithNeighbour(index - myWorldWidth) ||
		CheckCollisionWithNeighbour(index + 1 - myWorldWidth))
	{
		myPlayer.ResetVelocity();
	}

	if (CheckCollisionWithNeighbour(index) ||
		CheckCollisionWithNeighbour(index + 1) ||
		CheckCollisionWithNeighbour(index + 1 + myWorldWidth) ||
		CheckCollisionWithNeighbour(index + myWorldWidth) ||
		CheckCollisionWithNeighbour(index - 1 + myWorldWidth) ||
		CheckCollisionWithNeighbour(index - 1) ||
		CheckCollisionWithNeighbour(index - 1 - myWorldWidth) ||
		CheckCollisionWithNeighbour(index - myWorldWidth) ||
		CheckCollisionWithNeighbour(index + 1 - myWorldWidth))
	{
		myPlayer.RevertYMovement();
	}

	if (myPlayer.GetPosition().x < 0.f || myPlayer.GetPosition().x > 1600.f - 64.f)
		myPlayer.RevertXMovement();
	if (myPlayer.GetPosition().y > (myWorldHeight-1) * 64.f)
	{
		myPlayer.SetPosition({ myPlayer.GetPosition().x, (myWorldHeight - 1) * 64.f });
		myPlayer.SetIsGrounded(true);
	}
}

bool CGame::CheckCollisionWithNeighbour(unsigned short aIndex)
{
	if (aIndex >= 0 && aIndex < myWorldWidth * myWorldHeight)
	{
		if (myTiles[aIndex].myType != ETileType::Sky && myTiles[aIndex].myIsDestroyed == false)
		{
			sf::FloatRect tileRect;
			tileRect.height = 64;
			tileRect.width = 64;
			tileRect.left = (aIndex % myWorldWidth) * 64;
			tileRect.top = (aIndex / myWorldWidth) * 64;

			if (myPlayer.Intersects(tileRect))
			{
				return true;
			}
		}
	}

	return false;
}
