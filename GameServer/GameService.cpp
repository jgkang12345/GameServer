#include "GameService.h"
#include "App.h"
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include "Session.h"
#include"ConsoleHelper.h"
GameService* GameService::m_instance = nullptr;

enum 
{
	V = 0,
	L = 1,
	D = 2,
};

char item[3][10] = { "가위", "바위", "보" };
int result[3][3] = { {D,L,V},{V,D,L},{L,V,D}};

bool GameService::Update()
{
	if (m_p1_score >= 2)
	{
		Session* session = App::GetInstance()->GetSession();
		char send_buffer[256] = "p";
		char recv_buffer[256];
		printf("게임 종료!! 재구 승리\n");
		send(session->GetScoket(), send_buffer, sizeof(send_buffer), 0);
		recv(session->GetScoket(), recv_buffer, sizeof(recv_buffer), 0);
		return false;
	}
	else if (m_p2_score >= 2)
	{
		Session* session = App::GetInstance()->GetSession();
		char send_buffer[256] = "q";
		char recv_buffer[256];
		printf("게임 종료!! 로운 승리\n");
		send(session->GetScoket(), send_buffer, sizeof(send_buffer), 0);
		int len = recv(session->GetScoket(), recv_buffer, sizeof(recv_buffer), 0);
		if (len < 0) printf("recv error   \n");

		return false;
	}
	else
	{
		Session* session = App::GetInstance()->GetSession();
		char send_buffer[256] = "f";
		char recv_buffer[256];
		send(session->GetScoket(), send_buffer, sizeof(send_buffer), 0);
	}


	{
		printf("1.가위 2.바위 3.보 선택해주세요! (숫자 1: 가위, 숫자 2: 바위, 숫자 3: 보)\n");
		Pick();
	}

	ConsoleHelper::Clear();
	
	{
		CalResult();
	}

	return true;
}


void GameService::Pick()
{
	unsigned __int64 Timer = GetTickCount64();
	while (true)
	{
		if(GetTickCount64() >= Timer + 1000) //1초가 지났는가?
		{
			Timer = GetTickCount64();
			m_count++;

			if( m_count > 4 && m_count <= 9)
				printf("%d초 남았습니다...\n",10 - m_count);
			else if (m_count == 10)
			{
				printf("시간초과... 자동으로 선택됩니다.");
				m_count = 0;
				m_p1_key = ((rand() % 3) + 1) + 48;
				break;
			}
		}

		if (m_p1_key == 0 && _kbhit())
		{
			m_p1_key = _getch();
			if (m_p1_key != '1' && m_p1_key != '2' && m_p1_key != '3')
				m_p1_key = ((rand() % 3) + 1) + 48;
			m_count = 0;
			printf("다른플레이어를 기다립니다...");
			break;
		}
	}
	Session* session = App::GetInstance()->GetSession();

	char recv_buffer [256];
	char send_buffer [256];

	int retValue = recv(session->GetScoket(), recv_buffer, sizeof(recv_buffer), 0);
	m_p2_key = *(recv_buffer);
	strcpy_s(send_buffer, "t");
	send(session->GetScoket(), send_buffer, sizeof(send_buffer), 0);
}

void GameService::CalResult()
{
	printf("결과를 계산합니다...\n");
	
	Sleep(1500);

	const int p1_index = GetIndex(m_p1_key);
	const int p2_index = GetIndex(m_p2_key);

	m_result = (Result) result[p1_index][p2_index];

	ConsoleHelper::Clear();
	CalResultRender();
}

void GameService::CalResultRender()
{

	Session* session = App::GetInstance()->GetSession();
	char send_buffer[256];
	sprintf_s(send_buffer, "%c#%c#%d", m_p1_key, m_p2_key, m_result);
	send(session->GetScoket(), send_buffer, sizeof(send_buffer), 0);

	const int p1_index = GetIndex(m_p1_key);
	const int p2_index = GetIndex(m_p2_key);
	printf("서버 %s \t", item[p1_index]);
	printf("클라 %s \t", item[p2_index]);


	switch (m_result)
	{
	case P1:
		m_p1_score += 1;
		printf("서버 승리!\n");
		break;

	case P2:
		m_p2_score += 1;
		printf("클라 승리!\n");
		break;

	case DD:
		printf("무승부!\n");
		break;
	}

	Sleep(1500);
	m_p1_key = 0;
	ConsoleHelper::Clear();
}

