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

	myConnectedStatus.setString("Not Connected");

	myShouldRun = true;
}

void CGame::Update()
{
	HandleWindowEvents();

	myWindow.clear(sf::Color(50, 150, 250));

	myWindow.draw(myConnectedStatus);

	myWindow.display();
}

void CGame::SetIsConnected(bool aIsConnected)
{
	if (aIsConnected)
		myConnectedStatus.setString("Connected");
}

bool CGame::GetShouldRun() const
{
	return myShouldRun;
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
