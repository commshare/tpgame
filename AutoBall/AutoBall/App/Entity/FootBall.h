
#pragma  once 

#include "MoveEntity.h"
#include "../Wall2D.h"
#include "../../Public/GameSetup.h"
#include "../../Public/Singleton.h"
#include "../../Render/VGdi.h"

///	 ������
class  FootBall : public CMoveEntity
{
public:
	FootBall(Vector2D  pos,double  BallSize,double mass,
			   std::vector<Wall2D>& PitchBoundary):
			   m_PitchBoundary(PitchBoundary),
			  CMoveEntity(pos,BallSize,Vector2D(0,0),
				-1.0,Vector2D(0,1),
				mass,Vector2D(1.0,1.0),0,0)		
	{

	}

	~FootBall(){}

public:

	/// ��ײ���,Ȼ����������⣬�����������״̬
	void	  TestCollisionWithWalls(const std::vector<Wall2D>& walls);

	void      Update();

	void      Render();

	bool      OnMessage(const tagMessage& msg){return false;}

	/// �յ�һ��������
	void      Kick(Vector2D direction, double force,CMoveEntity* entity);

	/// 
	///	 ����һ����������ͨ�������յ�Ķ�����ƶ�����
	///	 �ķ��������򾭹���ξ���Ҫ���೤ʱ��
	/// 
	double    TimeToCoverDistance(Vector2D from,
								  Vector2D to,
								  double     force)const;

	/// �÷��������ڸ���ʱ������λ��
	Vector2D FuturePosition(double time)const;

	/// �ⱻ���ڶ�Ա������Ա����ͣ��ͣ����ʾ��������
	void      Trap()		{m_vVelocity.Zero();}  

	/// �����һ�θ����е�λ��
	Vector2D  OldPos()const	{return m_vOldPos;}

	/// ���������λ�ã������ٶ�Ϊ0
	void      PlaceAtPosition(Vector2D NewPos);

private:

	/// ���һ����������
	CMoveEntity				 *m_LastPlayer;

	/// ��¼������һ�θ����е�λ��
	Vector2D                  m_vOldPos;

	/// ������򳡱߽�ı�������(������ײ���)
	const std::vector<Wall2D>& m_PitchBoundary;   
};

/// ��һ��������������Ķ�Ա
inline Vector2D AddNoiseToKick(Vector2D BallPos, Vector2D BallTarget)
{

	double displacement = (Pi - Pi*0.99) * RandomClamped();

	Vector2D toTarget = BallTarget - BallPos;

	Vec2DRotateAroundOrigin(toTarget, displacement);

	return toTarget + BallPos;
}