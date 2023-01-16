#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
class Session
{
private:
	SOCKET			m_socket;
	SOCKADDR_IN		m_sock_addr_in;
	char			m_recv_buf[256];
	char			m_send_buf[256];
public:
	Session() {};
	SOCKADDR_IN& GetSockAddrIN() { return m_sock_addr_in; }
	SOCKET&		 GetScoket() { return m_socket; }
	char*		 GetRecvBuf() { return m_recv_buf; }
	int			 GetBufSize() { return sizeof(m_recv_buf); }
	int			 GetSendBufSize() { return sizeof(m_send_buf); }
	char*		 GetSendBuf() { return m_send_buf; }
	void		 SetScoket(const SOCKET& socket) { m_socket = socket; }
	void         SetSendBuf(const char* buf) { strcpy_s(m_send_buf, buf);}


};

