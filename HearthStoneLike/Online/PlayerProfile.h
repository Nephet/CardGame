#pragma once
#include "stormancer.h"

namespace Stormancer
{

	class PlayerProfile
	{
	public:
		std::string Id;
		std::string PlayerId;
		int RMMR;

		int GMMR;

		MSGPACK_DEFINE(Id, PlayerId, GMMR, RMMR)
	};
}