#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
class SocketHelper
{
public:
	static SOCKET	CreateIP4Socket();
	static void		InitIP4Socket(SOCKADDR_IN& serverAddr, const char* host, int port);
};

