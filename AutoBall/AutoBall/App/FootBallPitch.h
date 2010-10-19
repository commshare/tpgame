#pragma once

#include <vector>
#include <assert.h>

class  FootBallTeam;
class  Goal;
class  FootBall;
class  Region;
class  Wall2D;

/// ����
class FootBallPitch
{ 

public:

	FootBallPitch(int cxClient, int cyClient);

	~FootBallPitch();

	void  Update();

	bool  Render();

	void  TogglePause(){m_bPaused = !m_bPaused;}
	bool  Paused()const{return m_bPaused;}

	int   cxClient()const{return m_cxClient;}
	int   cyClient()const{return m_cyClient;}

	bool  GoalKeeperHasBall()const{return m_bGoalKeeperHasBall;}
	void  SetGoalKeeperHasBall(bool b){m_bGoalKeeperHasBall = b;}

	const Region*const         PlayingArea()const{return m_pPlayingArea;}
	const std::vector<Wall2D>& Walls(){return m_vecWalls;}                      
	FootBall*const			   Ball()const{return m_pBall;}

	const Region* const GetRegionFromIndex(int idx)                                
	{
		assert ( (idx >= 0) && (idx < (int)m_Regions.size()) );

		return m_Regions[idx];
	}

	bool  GameOn()const{return m_bGameOn;}
	void  SetGameOn(){m_bGameOn = true;}
	void  SetGameOff(){m_bGameOn = false;}

private:

	void CreateRegions(double width, double height);/// ����һ����������

public:

	FootBall*			 m_pBall;		///> ������ 
	FootBallTeam*        m_pRedTeam;	///> ���
	FootBallTeam*        m_pBlueTeam;	///> ����

	Goal*                m_pRedGoal;	///> �������
	Goal*                m_pBlueGoal;	///> ��������

	std::vector<Wall2D>  m_vecWalls;	///> �߽�ǽ������

	Region*              m_pPlayingArea;///> ���������ĳߴ�
	std::vector<Region*> m_Regions;		///> ��¼���еĳ�����Ϣ
	bool                 m_bGoalKeeperHasBall;///> ����Ա�Ƿ��������
	bool                 m_bGameOn;		///> ��¼��Ϸ�Ƿ�����У�����������жϱ���

	bool                 m_bPaused;	    ///> ��Ϸ��ͣ
	int                  m_cxClient;    ///> �ͻ��˴��ڵı��ظ����ߴ�
	int				   	 m_cyClient;  

};