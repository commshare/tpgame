#pragma once

#include <vector>
#include <assert.h>

class  SoccerBall;
class  SoccerTeam;
class  Goal;
class  Region;
class  Wall2D;

/// ����
class SoccerPitch
{ 

public:

	SoccerPitch(int cxClient, int cyClient);

	~SoccerPitch();

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
	SoccerBall*const           Ball()const{return m_pBall;}

	const Region* const GetRegionFromIndex(int idx)                                
	{
		assert ( (idx >= 0) && (idx < (int)m_Regions.size()) );

		return m_Regions[idx];
	}

	bool  GameOn()const{return m_bGameOn;}
	void  SetGameOn(){m_bGameOn = true;}
	void  SetGameOff(){m_bGameOn = false;}

public:

	SoccerBall*          m_pBall;       ///> ������ 
	SoccerTeam*          m_pRedTeam;	///> ���
	SoccerTeam*          m_pBlueTeam;	///> ����

	Goal*                m_pRedGoal;	///> �������
	Goal*                m_pBlueGoal;	///> ��������

	std::vector<Wall2D>  m_vecWalls;	///> �߽�ǽ������

	Region*              m_pPlayingArea;///> ���������ĳߴ�
	std::vector<Region*> m_Regions;		///> ��¼���еĳ�����Ϣ
	bool                 m_bGoalKeeperHasBall;///> ����Ա�Ƿ��������
	bool                 m_bGameOn;		///> ��¼��Ϸ�Ƿ�����У�����������жϱ���

	//set true to pause the motion
	bool                 m_bPaused;	    ///> ��Ϸ��ͣ

	//local copy of client window dimensions
	int                  m_cxClient,    ///> �ͻ��˴��ڵı��ظ����ߴ�
		int				   	 m_cyClient;  

	//this instantiates the regions the players utilize to  position
	//themselves
	void CreateRegions(double width, double height);/// ����һ����������
};