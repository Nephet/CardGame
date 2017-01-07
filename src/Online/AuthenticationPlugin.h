#pragma once
#include "stormancer.h"
#include "IAuthenticationService.h"

namespace Stormancer
{
	class AuthenticationPlugin : public IPlugin
	{
	public:
		AuthenticationPlugin();
		virtual ~AuthenticationPlugin();

		void clientCreated(Client* client);

		void destroy();
	};
};
