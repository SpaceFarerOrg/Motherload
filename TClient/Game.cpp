#include "stdafx.h"
#include "Game.h"

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
}

void CGame::Update()
{
	HandleWindowEvents();

	float dt = myClock.getElapsedTime().asSeconds();
	myClock.restart();

	myPlayer.Update(dt);

	Render();
}

void CGame::Render()
{
	myWindow.clear(sf::Color(50, 150, 250));

	for (auto& pair : myOtherPlayers)
	{
		myWindow.draw(pair.second);
	}

	myPlayer.Render(&myWindow);

	myWindow.draw(myConnectedStatus);

	myWindow.draw(myKbPerSecond);

	myWindow.display();
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
	myOtherPlayers.erase(aID);
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
