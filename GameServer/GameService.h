#pragma once
class GameService
{
	
	enum RockPaperScissors : char
	{
		Scissors = '1',
		Rock = '2',
		Paper = '3'
	};

	enum Result : int
	{
		P1 = 0,
		P2 = 1,
		DD = 2
	};

private:
	static	GameService* m_instance;
	int		m_p1_score = 0;
	int		m_p2_score = 0;
	Result	m_result;
	int		m_sumTick = 0;
	char	m_p1_key = 0;
	char	m_p2_key = 0;
	int     m_count = 0;
public:
	GameService() {};
	~GameService() {};
	void Pick();
	void CalResult();
	void CalResultRender();
	bool Update();
	int  GetIndex(char key)
	{
		return key - 48 - 1;
	}
	static GameService* GetInstance() 
	{
		if (m_instance == nullptr)
			m_instance = new GameService();

		return m_instance;
	};
};

