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

char item[3][10] = { "����", "����", "��" };
char result_char[3][10] = { "���� �¸�", "Ŭ�� �¸�", "���º�" };
int result[3][3] = { {D,L,V},{V,D,L},{L,V,D}};

bool GameService::Update()
{
	if (m_p1_score >= 2)
	{
		Session* session = App::GetInstance()->GetSession();
		session->SetSendBuf("p");
		printf("���� ����!! �籸 �¸�\n");
		send(session->GetScoket(), session->GetSendBuf(), session->GetBufSize(), 0);
		recv(session->GetScoket(), session->GetRecvBuf(), session->GetBufSize(), 0);
		return false;
	}
	else if (m_p2_score >= 2)
	{
		Session* session = App::GetInstance()->GetSession();
		session->SetSendBuf("q");
		printf("���� ����!! �ο� �¸�\n");
		send(session->GetScoket(), session->GetSendBuf(), session->GetBufSize(), 0);
		recv(session->GetScoket(), session->GetRecvBuf(), session->GetBufSize(), 0);
		return false;
	}
	else
	{
		Session* session = App::GetInstance()->GetSession();
		session->SetSendBuf("f");
		send(session->GetScoket(), session->GetSendBuf(), session->GetBufSize(), 0);
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
			Timer = GetTickCount64();
			m_count++;

			if( m_count > 4 && m_count <= 9)
				printf("%d�� ���ҽ��ϴ�...\n",10 - m_count);
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


	recv(session->GetScoket(), session->GetRecvBuf(), session->GetBufSize(), 0);
	m_p2_key = *(session->GetRecvBuf());
	session->SetSendBuf("t");
	send(session->GetScoket(), session->GetSendBuf(), session->GetBufSize(), 0);
}

void GameService::CalResult()
{
	printf("����� ����մϴ�...\n");
	
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
	session->SetSendBuf(send_buffer);
	send(session->GetScoket(), session->GetSendBuf(), session->GetSendBufSize(), 0);

	const int p1_index = GetIndex(m_p1_key);
	const int p2_index = GetIndex(m_p2_key);
	printf("���� %s \t", item[p1_index]);
	printf("Ŭ�� %s \t", item[p2_index]);
	printf("%s \n", result_char[m_result]);
	switch (m_result)
	{
	case P1:
		m_p1_score += 1;
		break;

	case P2:
		m_p2_score += 1;
		break;
	}

	Sleep(1500);
	m_p1_key = 0;
	ConsoleHelper::Clear();
}

