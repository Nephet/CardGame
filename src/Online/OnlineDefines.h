#pragma once
#define STORMANCER_ENDPOINT "http://api.stormancer.com"
#define STORMANCER_ACCOUNT "dotemu-windjammers"

#if defined(STORMANCER_ENV_QA)
#define STORMANCER_APPLICATION "qa"
#elif defined(STORMANCER_ENV_DEVCLIENT)
#define STORMANCER_APPLICATION "dev-client"
#else //ENV DEVSERVER
#define STORMANCER_APPLICATION "dev-server"
#endif