#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
class Session;
class App
{
private:
	static App*			m_instance;	
	WSADATA				m_wsa_data;
	SOCKET				m_socket;
	SOCKADDR_IN			m_sock_addr_in;
	Session*			m_session;
public:
	App();
	~App() {};
	Session* GetSession() { return m_session; }
	void Init();
	void Update();
	void Clean();
	static App* GetInstance();
};

