#include "GameService.h"
#include "App.h"
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include "Session.h"
#include"ConsoleHelper.h"
GameService* GameService::m_instance = nullptr;

bool GameService::Update()
{
	if (m_p1_score >= 2)
	{
		Session* session = App::GetInstance()->GetSession();
		char send_buffer[256] = "p";
		char recv_buffer[256];
		printf("게임 종료!! 서버 승리\n");
		send(session->GetScoket(), send_buffer, sizeof(send_buffer), 0);
		recv(session->GetScoket(), recv_buffer, sizeof(recv_buffer), 0);
		return false;
	}
	else if (m_p2_score >= 2)
	{
		Session* session = App::GetInstance()->GetSession();
		char send_buffer[256] = "q";
		char recv_buffer[256];
		printf("게임 종료!! 클라 승리\n");
		send(session->GetScoket(), send_buffer, sizeof(send_buffer), 0);
		recv(session->GetScoket(), recv_buffer, sizeof(recv_buffer), 0);
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
			/*printf("1초 지남");*/
			Timer = GetTickCount64();
			m_count++;

			if (m_count == 5)
			{
				printf("5초 남았습니다...\n");
			}
			else if (m_count == 6)
			{
				printf("4초 남았습니다...\n");
			}
			else if (m_count == 7)
			{
				printf("3초 남았습니다...\n");
			}
			else if (m_count == 8)
			{
				printf("2초 남았습니다...\n");
			}
			else if (m_count == 9)
			{
				printf("1초 남았습니다...\n");
			}
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

	recv(session->GetScoket(), recv_buffer, sizeof(recv_buffer), 0);
	m_p2_key = *(recv_buffer);
	strcpy_s(send_buffer, "t");
	send(session->GetScoket(), send_buffer, sizeof(send_buffer), 0);
}

void GameService::CalResult()
{
	printf("결과를 계산합니다...\n");
	
	Sleep(1500);

	switch (m_p1_key)
	{
	case RockPaperScissors::Rock:
		switch (m_p2_key)
		{
			case RockPaperScissors::Rock:
				m_result = DD;
				break;

			case RockPaperScissors::Scissors:
				m_result = P1;
				break;

			case RockPaperScissors::Paper:
				m_result = P2;
				break;
		}
		break;

	case RockPaperScissors::Scissors:
		switch (m_p2_key)
		{
		case RockPaperScissors::Rock:
			m_result = P2;
			break;

		case RockPaperScissors::Scissors:
			m_result = DD;
			break;

		case RockPaperScissors::Paper:
			m_result = P1;
			break;
		}
		break;

	case RockPaperScissors::Paper:
		switch (m_p2_key)
		{
		case RockPaperScissors::Rock:
			m_result = P1;
			break;

		case RockPaperScissors::Scissors:
			m_result = P2;
			break;

		case RockPaperScissors::Paper:
			m_result = DD;
			break;
		}
		break;
	}

	ConsoleHelper::Clear();
	CalResultRender();
}

void GameService::CalResultRender()
{

	Session* session = App::GetInstance()->GetSession();
	char send_buffer[256];
	sprintf_s(send_buffer, "%c#%c#%d", m_p1_key, m_p2_key, m_result);
	send(session->GetScoket(), send_buffer, sizeof(send_buffer), 0);


	switch (m_p1_key)
	{
	case RockPaperScissors::Rock:
		printf("서버 바위 \t");
		break;

	case RockPaperScissors::Scissors:
		printf("서버 가위 \t");
		break;

	case RockPaperScissors::Paper:
		printf("서버 보자기 \t");
		break;
	}

	switch (m_p2_key)
	{
	case RockPaperScissors::Rock:
		printf("클라 바위 \n");
		break;

	case RockPaperScissors::Scissors:
		printf("클라 가위 \n");
		break;

	case RockPaperScissors::Paper:
		printf("클라 보자기 \n");
		break;
	}


	switch (m_result)
	{
	case P1:
		m_p1_score += 1;
		printf("서버 승리~!\n");
		break;

	case P2:
		m_p2_score += 1;
		printf("클라 승리~!\n");
		break;

	case DD:
		printf("무승부!\n");
		break;
	}

	Sleep(1500);
	m_p1_key = 0;
	ConsoleHelper::Clear();
}

