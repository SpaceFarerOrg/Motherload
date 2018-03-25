#pragma once

namespace Math
{
	template<typename T>
	T Clamp(T aValue, T aMin, T aMax)
	{
		return aValue < aMin ? aMin: (aValue > aMax ? aMax : aValue);
	}
}