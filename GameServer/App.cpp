#include "App.h"
#include "SocketHelper.h"
#include "Session.h"
#include "ConsoleHelper.h"
#include "GameService.h"
App* App::m_instance = nullptr;

App::App()
{

}

void App::Init()
{
	ConsoleHelper::ShowConsoleCursor(false);

	if (WSAStartup(MAKEWORD(2, 2), &m_wsa_data) != 0)	
		printf("Failed WSAStartup() \n");

	m_socket =  SocketHelper::CreateIP4Socket();
	
	if (m_socket == INVALID_SOCKET) 
		printf("Failed socket() \n");

	SocketHelper::InitIP4Socket(m_sock_addr_in, "58.236.86.23", 3000);

	if (bind(m_socket, (SOCKADDR*)&m_sock_addr_in, sizeof(m_sock_addr_in)) == SOCKET_ERROR)
		printf("Binding Error \n");

	if (listen(m_socket, 5) == SOCKET_ERROR)
		printf("listen Error \n");

	m_session = new Session();
	int	sizeClientAddr = sizeof(m_session->GetSockAddrIN());

	printf("Listening... \n");

	m_session->SetScoket(accept(m_socket, (SOCKADDR*)( & (m_session->GetSockAddrIN())), & sizeClientAddr));

	printf("Game Start ! \n");

	ConsoleHelper::Clear();
}

void App::Update()
{
	while (GameService::GetInstance()->Update()) {}
}

void App::Clean()
{
	WSACleanup();
	closesocket(m_socket);
	closesocket(m_session->GetScoket());
}

App* App::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new App();

	return m_instance;
}
