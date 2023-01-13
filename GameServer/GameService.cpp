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
		printf("���� ����!! ���� �¸�\n");
		send(session->GetScoket(), send_buffer, sizeof(send_buffer), 0);
		recv(session->GetScoket(), recv_buffer, sizeof(recv_buffer), 0);
		return false;
	}
	else if (m_p2_score >= 2)
	{
		Session* session = App::GetInstance()->GetSession();
		char send_buffer[256] = "q";
		char recv_buffer[256];
		printf("���� ����!! Ŭ�� �¸�\n");
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
		printf("1.���� 2.���� 3.�� �������ּ���! (���� 1: ����, ���� 2: ����, ���� 3: ��)\n");
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
		if(GetTickCount64() >= Timer + 1000) //1�ʰ� �����°�?
		{
			/*printf("1�� ����");*/
			Timer = GetTickCount64();
			m_count++;

			if (m_count == 5)
			{
				printf("5�� ���ҽ��ϴ�...\n");
			}
			else if (m_count == 6)
			{
				printf("4�� ���ҽ��ϴ�...\n");
			}
			else if (m_count == 7)
			{
				printf("3�� ���ҽ��ϴ�...\n");
			}
			else if (m_count == 8)
			{
				printf("2�� ���ҽ��ϴ�...\n");
			}
			else if (m_count == 9)
			{
				printf("1�� ���ҽ��ϴ�...\n");
			}
			else if (m_count == 10)
			{
				printf("�ð��ʰ�... �ڵ����� ���õ˴ϴ�.");
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
			printf("�ٸ��÷��̾ ��ٸ��ϴ�...");
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
	printf("����� ����մϴ�...\n");
	
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
		printf("���� ���� \t");
		break;

	case RockPaperScissors::Scissors:
		printf("���� ���� \t");
		break;

	case RockPaperScissors::Paper:
		printf("���� ���ڱ� \t");
		break;
	}

	switch (m_p2_key)
	{
	case RockPaperScissors::Rock:
		printf("Ŭ�� ���� \n");
		break;

	case RockPaperScissors::Scissors:
		printf("Ŭ�� ���� \n");
		break;

	case RockPaperScissors::Paper:
		printf("Ŭ�� ���ڱ� \n");
		break;
	}


	switch (m_result)
	{
	case P1:
		m_p1_score += 1;
		printf("���� �¸�~!\n");
		break;

	case P2:
		m_p2_score += 1;
		printf("Ŭ�� �¸�~!\n");
		break;

	case DD:
		printf("���º�!\n");
		break;
	}

	Sleep(1500);
	m_p1_key = 0;
	ConsoleHelper::Clear();
}

