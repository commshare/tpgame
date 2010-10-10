/**
 * @brief  ����2ά������һ����װ��
 *
 *
 */
#pragma  once

#include <limits>
#include <cmath>
#include <windows.h>

using namespace std;
#define  pi        3.1415926
#define  tol       0.0000001

class  Vector2D
{

public:
	Vector2D(): x(0.0),y(0.0) { }

	Vector2D(double a ,double b ) : x ( a ) , y ( b ) { }

	Vector2D(const Vector2D& rhs )	{  x = rhs.x  , y = rhs.y ; }

	void  Zero();

	bool  IsZero() const ;

	/// ��ģ�Ĵ�С
	double Length() const ;

	/// ����
	double LengthSq() const;

	/// ��ʽ��
	void   Normalize();

	/// ���
	double Dot( const Vector2D& v2) const;

	/// ����v2�����ڵ�ǰ�������ϻ����·�
	int    Sign(const Vector2D& v2) const;

	/// ���ص�ǰ�����Ĵ�ֱ����
	Vector2D Perp()const;

	/// ��ȡ��������С�ڵ���max
	void    Truncate( double max );

	/// �������2����������
	double  Distance( const Vector2D& v2 )const;

	double  DistanceSq( const Vector2D& v2)const;

	/// ����v2����õ���������
	void    Reflect( const Vector2D& v2) ;

	/// �õ�������
	Vector2D GetReverse() const;

	Vector2D& operator+= ( const Vector2D& rhs );

	Vector2D& operator-= ( const Vector2D& rhs );

	Vector2D& operator/= ( double rhs );

	Vector2D& operator*= ( double rhs );

	bool operator!= ( const Vector2D& rhs );

	bool operator== ( const Vector2D& rhs );

	friend Vector2D operator * (const Vector2D& a , double vsys		  );

	friend Vector2D operator * ( double vsys      , const Vector2D& b );

	friend Vector2D operator / (const Vector2D& a , double vsys       );

	friend Vector2D operator + (const Vector2D& a , const Vector2D& b );

	friend Vector2D operator - (const Vector2D& a , const Vector2D& b );

public:

	double    x , y;   /// x,y����
};

inline Vector2D Vec2DNormalize(const Vector2D &v)
{
	Vector2D vec = v;

	double vector_length = vec.Length();

	if (vector_length > std::numeric_limits<double>::epsilon())
	{
		vec.x /= vector_length;
		vec.y /= vector_length;
	}

	return vec;
}


inline double Vec2DDistance(const Vector2D &v1, const Vector2D &v2)
{
	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

inline double Vec2DDistanceSq(const Vector2D &v1, const Vector2D &v2)
{

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return ySeparation*ySeparation + xSeparation*xSeparation;
}

inline double Vec2DLength(const Vector2D& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

inline double Vec2DLengthSq(const Vector2D& v)
{
	return (v.x*v.x + v.y*v.y);
}

inline Vector2D POINTStoVector(const POINTS& p)
{
	return Vector2D(p.x, p.y);
}

inline Vector2D POINTtoVector(const POINT& p)
{
	return Vector2D((double)p.x, (double)p.y);
}

inline POINTS VectorToPOINTS(const Vector2D& v)
{
	POINTS p;
	p.x = (short)v.x;
	p.y = (short)v.y;

	return p;
}

inline POINT VectorToPOINT(const Vector2D& v)
{
	POINT p;
	p.x = (long)v.x;
	p.y = (long)v.y;

	return p;
}

inline void WrapAround(Vector2D &pos, int MaxX, int MaxY)
{
	if (pos.x > MaxX) {pos.x = 0.0;}

	if (pos.x < 0)    {pos.x = (double)MaxX;}

	if (pos.y < 0)    {pos.y = (double)MaxY;}

	if (pos.y > MaxY) {pos.y = 0.0;}
}

/// p����top_left,bot_rgt��
inline bool NotInsideRegion(Vector2D p,
							Vector2D top_left,
							Vector2D bot_rgt)
{
	return (p.x < top_left.x) || (p.x > bot_rgt.x) || 
		(p.y < top_left.y) || (p.y > bot_rgt.y);
}

/// p��top_left,bot_rgt��
inline bool InsideRegion(Vector2D p,
						 Vector2D top_left,
						 Vector2D bot_rgt)
{
	return !NotInsideRegion(p,top_left,bot_rgt);
}

/// p��top��left,bot��rgt��
inline bool InsideRegion(Vector2D p, int left, int top, int right, int bottom)
{
	return !( (p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom) );
}

inline bool isSecondInFOVOfFirst(Vector2D posFirst,
								 Vector2D facingFirst,
								 Vector2D posSecond,
								 double    fov)
{
	Vector2D toTarget = Vec2DNormalize(posSecond - posFirst);

	return facingFirst.Dot(toTarget) >= cos(fov/2.0);
}