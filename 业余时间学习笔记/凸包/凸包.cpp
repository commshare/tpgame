#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace  std;

typedef struct CPoint
{
	double x, y;
	CPoint()
	{
		x = y = 0.0f;
	}
	CPoint( double a , double b ) : x(a),y(b)
	{

	}
}Point;

const int MAXSIZE = 10000;
const double EPS = 1E-6;
int   stack[MAXSIZE]; //����ģ���ջ�����͹�������򶥵�
int   top;            //͹��������p[stack[0]...stack[top]]
Point p[MAXSIZE];

void   GrahamScan(const int &n); //Grahamɨ�跨��͹��
double Dist(const Point &arg1, const Point &arg2); //�������
double Length(const int &n);   //������ܳ�
double Area(const int &n);     //��������
void   Swap(const int &i, const int &j); //����������
double Multi(const Point &p0, const Point &p1, const Point &p2); //�������
int    cmp(const void *arg1, const void *arg2); //�Զ�������:���ռ���

/*
// ��� y=p.y �� p1p2 �Ľ��� 
if ( p1.y == p2.y ) // p1p2 �� y=p0.yƽ�� 
continue; 
if ( ps.y < min(p1.y, p2.y) ) // ������p1p2�ӳ����� 
continue; 
if ( ps.y >= max(p1.y, p2.y) ) // ������p1p2�ӳ����� 
continue; 

// �󽻵�� X ���� -------------------------------------------------------------- 
double x = (double)(ps.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 

if ( x > ps.x ) 
nCross++; // ֻͳ�Ƶ��߽��� 
*/


bool PtInPolygon (Point ps, int* ptPolygon, int nCount) 
{ 
	float  angle = 0.0f;
	/*
	for (int i = 0; i < nCount; i++) 
	{ 
		Point p1 = p[ ptPolygon[i] ]; 
		Point p2 = p[ ptPolygon[(i + 1) % nCount] ]; 

		Point v1 ( p1.x - ps.x , p1.y - ps.y );
		Point v2  ( p2.x - ps.x , p2.y - ps.y );

		float cve1 = v1.x*v1.x + v1.y*v1.y;
		float cve2 = v2.x*v2.x + v2.y*v2.y;

		//float ve = (p1.x-ps.x)*(p2.x-ps.x)  - (p2.y-ps.y)*(p1.y-ps.y);

		//float cv1 = sqrt( (p1.x-ps.x)*(p1.x-ps.x) + (p1.y-ps.y) * (p1.y-ps.y) );
		//float cv2 = sqrt( (p2.x-ps.x)*(p2.x-ps.x) + (p2.y-ps.y) * (p2.y-ps.y) );

		angle  += acos ( ( v1.x*v2.x + v1.y *v2.y ) / ( sqrt( cve1 ) * sqrt(cve2) ) );

	} 
	
	return  fabs(angle-3.1415926f*2) < 0.2f;
	*/
    long nCross = 0 ;
	for ( int i = 0 ; i < nCount ; i++ )
	{
		Point p1 = p[ ptPolygon[i] ]; 
		Point p2 = p[ ptPolygon[(i + 1) % nCount] ];

		// ��� y=p.y �� p1p2 �Ľ��� 
		if ( p1.y == p2.y ) // p1p2 �� y=p0.yƽ�� 
			continue; 
		if ( ps.y < min(p1.y, p2.y) ) // ������p1p2�ӳ����� 
			continue; 
		if ( ps.y >= max(p1.y, p2.y) ) // ������p1p2�ӳ����� 
			continue; 

		// �󽻵�� X ���� -------------------------------------------------------------- 
		double x = (double)(ps.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 

		if ( x > ps.x ) 
			nCross++; // ֻͳ�Ƶ��߽��� 
	}

	return ( nCross % 2 == 1 );
	
} 

int main(void)
{

	char str[100];

	strcat( str , "d");
	strcpy( str ,"1");
	int  n;
	// freopen("Input.txt", "r", stdin);
	while( scanf("%d", &n), n )
	{
		int  i;
		for( i=0; i < n; ++i )
		{
			scanf("%lf %lf", &p[i].x, &p[i].y);
		}
		if( n == 1 )
		{
			printf("�ܳ�: 0.00\t���: 0.00\n");
		}
		else if( n == 2 ) 
		{
			printf("�ܳ�: %.2lf\t���: 0.00\n", Dist(p[0], p[1]));
		}
		else 
		{
			GrahamScan(n); 

			for( int k=0; k <= top; ++k )
			{
				printf("%lf  %lf\n", p[stack[k]].x, p[stack[k]].y);
			}


			Point pi;					   //pi

			scanf("%d", &n);

			for ( int i = 0 ;  i < n ; i++ )
			{
				scanf("%lf %lf", &pi.x, &pi.y);

				bool flag = PtInPolygon( pi , stack , top+1 );

				printf(" Point x = %f , y = %f , flag = %d " , pi.x , pi.y , flag);
			}

			

			printf("�ܳ�: %.2lf\t���: %.2lf\n", Length(top+1), Area(top+1));
		}
	}

	return 0;
}

double Length(const int &n)
{
	double  sum = Dist( p[stack[n-1]], p[stack[0]] );
	for( int i = 0; i < n-1; ++i )
	{
		sum += Dist( p[stack[i]], p[stack[i+1]] );
	}
	return sum;
}

double Area(const int &n)
{
	double area = 0;
	for( int i=0; i < n-1; ++i )
	{
		area += (p[stack[i]].x*p[stack[i+1]].y) - (p[stack[i+1]].x*p[stack[i]].y);
	}
	area = fabs(area) / 2;
	return area;
}

double Dist(const Point &arg1, const Point &arg2)
{
	return sqrt( (arg1.x - arg2.x)*(arg1.x - arg2.x) + (arg1.y - arg2.y)*(arg1.y - arg2.y) );
}

void Swap(const int &i, const int &j)
{
	Point temp = p[i];
	p[i] = p[j];
	p[j] = temp;
}

//ͨ��ʸ������󼫽ǹ�ϵ(p0p1)(p0p2)
//P*Q > 0,P��Q��˳ʱ�뷽��... ...
double Multi(const Point &p0, const Point &p1, const Point &p2)
{
	return (p1.x-p0.x)*(p2.y-p0.y) - (p2.x-p0.x)*(p1.y-p0.y);
}

int cmp(const void *arg1, const void *arg2)
//int cmp(const Point &arg1, const Point &arg2)
{
	Point a = *(Point*)arg1;
	Point b = *(Point*)arg2;

	double k = Multi(p[0], a, b);
	if( k < -EPS ) return 1;
	else if( fabs(k) < EPS && (Dist(a, p[0]) - Dist(b, p[0])) > EPS ) return 1;
	else return -1;
}


void GrahamScan(const int &n)
{
	int  i, u = 0;
	for( i = 1; i < n; ++i )
	{
		//�ҵ������µĶ���, ����Ϊp[0]
		if( ( p[i].y < p[u].y ) || ( p[i].y == p[u].y && p[i].x < p[u].x ) ) 
			u = i;
	}

	i = 0;
	Swap(i, u); //ֻ����һ��

	//���㰴�����p[0]�ļ��Ǵ�С��������, ��qsort
	qsort( p+1, n-1, sizeof(p[0]), cmp );

	//sort( p+1 , p+n-1, cmp );
	//��ÿһ������һ����, ��͹�������ջ��, �����򵯳�ջ��
	//ע��: �����, p[0]��p[1]һ����͹���Ķ���
	stack[0] = 0; stack[1] = 1; stack[2] = 2;
	top = 2;
	for( int j = 3; j < n; ++j )
	{
		while( Multi( p[j], p[stack[top]], p[stack[top-1]] ) > 0 )
		{
			--top;
			if( top == 0 ) break;
		}
		stack[++top] = j;
	}

	i = 0;
}
