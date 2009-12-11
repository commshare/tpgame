/*
*	File  :  locks.h
*   Brief :  һ��֧���¼����ź��������������ü����ķ�ʽ����Ҫ�ο�����ָ��(Auto_ptr)��ʵ��
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once
#include <windows.h>

namespace sync
{
	/// 
	/// һ�����Ľӿں���
	///
	class Super_lock
	{
	public:
		Super_lock(void);
		virtual ~Super_lock(void);

	public:
		virtual bool open() = 0;
		virtual void close()= 0;
		virtual bool enter()= 0;
		virtual void leave()= 0;

	};


	class scope_guard
	{
	public:
		scope_guard( Super_lock & _lock);
		~scope_guard();

	private:
		Super_lock & lock;

	};


	/// �¼���
	class eventlock 
		: public Super_lock
	{
		struct state_type
		{
			enum 
			{
				none,
				timeout,
			};
		};

		eventlock();
		~eventlock();

		bool open();
		void close();
		bool enter();
		void leave();

	private:
		HANDLE handle_;
		DWORD  timeout_;
		int    state_;
	};

	/// �ٽ���
	class csectionlock
		:public Super_lock
	{
	public:
		csectionlock();
		~csectionlock();

		bool open();
		void close();
		bool enter();
		void leave();

	private:
		bool  is_ok;
		::CRITICAL_SECTION cs_;
	};

}
