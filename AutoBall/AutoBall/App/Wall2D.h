/**
 *   @brief ǽ��һ���߶κ��߶εķ������
 *
 */
#pragma  once 
#include <fstream>

#include "../Render/Vector2D.h"
#include "../Render/VGdi.h"
#include "../Public/Singleton.h"

using namespace std;

class Wall2D 
{

public:

	Wall2D(){}

	Wall2D(Vector2D A, Vector2D B):m_vA(A), m_vB(B)
	{
		CalculateNormal();
	}

	Wall2D(Vector2D A, Vector2D B, Vector2D N):m_vA(A), m_vB(B), m_vN(N)
	{ }

	virtual void Render(bool RenderNormals = false)const
	{
		GetInstObj(CGDI).Line(m_vA, m_vB);
		GetInstObj(CGDI).Line(m_vA-Vector2D(1,1), m_vB-Vector2D(1,1));

		if (RenderNormals)
		{
			int MidX = (int)((m_vA.x+m_vB.x)/2);
			int MidY = (int)((m_vA.y+m_vB.y)/2);

			GetInstObj(CGDI).Line(MidX, MidY, (int)(MidX+(m_vN.x * 5)), (int)(MidY+(m_vN.y * 5)));
		}
	}

	Vector2D From()const  {return m_vA;}
	void     SetFrom(Vector2D v){m_vA = v; CalculateNormal();}

	Vector2D To()const    {return m_vB;}
	void     SetTo(Vector2D v){m_vB = v; CalculateNormal();}

	Vector2D Normal()const{return m_vN;}
	void     SetNormal(Vector2D n){m_vN = n;}

	Vector2D Center()const{return (m_vA+m_vB)/2.0;}

private:

	Vector2D    m_vA,
				m_vB,
				m_vN;

	void CalculateNormal()
	{
		Vector2D temp = Vec2DNormalize(m_vB - m_vA);

		m_vN.x = -temp.y;
		m_vN.y = temp.x;
	}
};