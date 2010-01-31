/*
*	File  :	 Region.h
*   Brief :	 һ�������࣬��Ҫ����AI2ά������Ѱ�в���
			����ʵ��һ����ά��map
*   Author:  Expter 
*	Creat Date:  [2009/12/9]
*/

#pragma once

#include "../Ai/Ai_tools.h"

namespace  AI_SYSTEM
{

	struct  Point 
	{
		/// ��ͼ�����һ��������
		int		xPos;
		int		yPos;
		int		val;
		bool    tostring( char * str)
		{
			sprintf(str,"x: %d y: %d val: %d",xPos,yPos,val);
			return true;
		}

		bool operator == ( const Point & node2 ) const
		{
			return ( ( this->xPos == node2.xPos ) && ( node2.yPos == this->yPos) );
		}
	};

	/*
	 *	  ��Ҫ������Ѱ·��һ�������Ϣ
	 */
	class   Node2D 
	{
	public:
		void  setStart( bool _start = true ) {  start = _start ;}
		void  setDestination( bool _Des =true ) {  destinantion = _Des;}
		void  setPath( bool _Path =true ) { path = _Path;}
		void  setWalkable( bool _able) { walkable = _able;}
		bool  getWalkable()  { return walkable; }
		void  setPoint(Point point)			 
		{
			memcpy(&this->point,&point,sizeof(Point));
		}

		bool  operator < ( const Node2D & node ) const
		{
			return this->f < node.f;
		}

		bool  operator < ( const Node2D * node ) const
		{
			return this->f < node->f ; 
		}

		bool  operator() ( const Node2D & node1 , const Node2D & node2 ) const
		{
			return node1 < node2 ; 
		}

		bool  operator == ( const Node2D & node )const
		{
			return	this->point == node.point;
		}

		bool  operator != ( const Node2D & node )const
		{
			return  !(*this == node);
		}


	public:
		static const int SIZE = 80;
		Point			 point;
		Node2D			 *parent;	// ���׽ڵ�
		int				 f;
		int				 g;
		int				 h;
		int				 ambient;
		int			     color;
		int				 alpha;
		bool			 walkable;
		bool			 start;
		bool			 destinantion;
		bool			 path;
	};


	class CRegion
	{
	public:
		CRegion(void);
	
		CRegion( long Width , long Height) :m_Width( Width) , m_Height ( Height )
		{
			m_Point = new long[ Width * Height ];
		}

		~CRegion(void);
		
	public:

		void	Init()	;

		void	SetPoint( long x , long y  , long  val );

		int		GetPoint( long x , long y );
		
// 		int		operator [] ( int  x , int y)	const ;
// 
// 		int		operator [] ( int  x , int y, int val);

		
	private:
		/*
		 *	 ��һά�����ʾ��ά����
		 */
		long		* m_Point;		// ��������
		long		  m_Width;		// ������
		long		  m_Height;		// ������
		long		  m_uid;		// ����id
	
	};
}