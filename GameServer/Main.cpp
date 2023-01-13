#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "App.h"
#pragma comment( lib, "ws2_32.lib")
int main()
{

	App::GetInstance()->Init();

	App::GetInstance()->Update();
	Sleep(10);

	App::GetInstance()->Clean();
	return 0;
}
