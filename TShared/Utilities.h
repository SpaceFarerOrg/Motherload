#pragma once
#include <string>
#include <iostream>
#include "CommonNetworkIncludes.h"

namespace Utilities
{
	static std::string GetInput(const std::string& aPrompt, const std::string& aDefaultValue)
	{
		std::cout << aPrompt;

		std::string input;

		std::getline(std::cin, input);

		if (input.size() == 0)
		{
			return aDefaultValue;
		}

		return input;
	}
}

