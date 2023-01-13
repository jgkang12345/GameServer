#include "SocketHelper.h"

SOCKET SocketHelper::CreateIP4Socket()
{
	return socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void SocketHelper::InitIP4Socket(SOCKADDR_IN& serverAddr, const char* host, int port)
{
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, host, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(port);
}
