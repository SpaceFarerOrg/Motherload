#pragma once
#include <SFML\System\Vector2.hpp>

namespace Math
{
	template<typename T>
	T Clamp(T aValue, T aMin, T aMax)
	{
		return aValue < aMin ? aMin: (aValue > aMax ? aMax : aValue);
	}

	float Lerp(float aStart, float aTarget, float aPercentage)
	{
		return (1 - aPercentage) * aStart + aPercentage * aTarget;
	}

	sf::Vector2f Lerp(const sf::Vector2f& aStart, const sf::Vector2f& aEnd, float aPercentage)
	{
		sf::Vector2f returnVector;

		returnVector.x = Lerp(aStart.x, aEnd.x, aPercentage);
		returnVector.y = Lerp(aStart.y, aEnd.y, aPercentage);

		return std::move(returnVector);
	}


}