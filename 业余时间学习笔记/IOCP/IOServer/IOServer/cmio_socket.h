#pragma once
#include "locks.h"
#include <vector>
#include <set>
#include <process.h>
using namespace std;

namespace _tp_io_complete_portname
{

	struct callback_info
	{
		void ( * handler )(int type, SOCKET, void* socket_extra, const char  * buf, unsigned long bufsize, void * userdata);
		void   * userdata;
	};

	class cmio_socket
	{
		/// 
		/// һЩ�ṹ��
		/// 
		typedef cmio_socket self_type;

		struct  handler_data 
		{
			handler_data()
				:sock( INVALID_SOCKET ), extra(0)
			{ }

			SOCKET sock;
			void   *extra;
		};

		enum{  BUFFERSIZE = 512, };
		struct operation_data
		{
			OVERLAPPED    overlapped;
			WSABUF		  wsabuf;
			char          data[BUFFERSIZE];
			//unsigned long bytes_recv;
			void  print()
			{
				std::cout << " wasbuf " << wsabuf.buf <<" " << wsabuf.len 
						  << " data "<< data <<" "
						  << /*bytes_recv<<*/ std::endl;			
			}
		};

		
		///
		/// Ԥ����
		/// 
	public:
		typedef unsigned    dstreamsize;
		typedef void ( *handler_funtion )( int type , SOCKET s , void * socket_extra , const char * buf , unsigned long bufsize , void * userdata );
		struct values
		{
			enum
			{
				null_socket = INVALID_SOCKET,
			};
		};

		/// 
		/// �����б�
		/// 
	public:
		cmio_socket(void);
		~cmio_socket(void);
		bool listener( int port , int connections );
		bool accept( void *(*extra_maker)(SOCKET s , void * arg) , void * arg);
		void handler( handler_funtion handle , void * userdata );
		bool recv(){  return true;		}
		static set<SOCKET>  GetFirstSocket( ){ return (socket_set_) ;}
	
		/// 
		/// ��Ϣ�������б�
		/// 
	private:
		static void  _m_recv( SOCKET s , operation_data * data );
		void   _m_call_handler( int type , SOCKET s , void * socket_extra , const char * buf , unsigned long bufsize );
		/// 
		/// ��ɶ˿ں�������
		/// 
		void _m_close_complete_io_port();
		void _m_make_complete_io_port();
		void _m_close_socket();
		void _m_open_socket();
		void _m_make_threads();

	private:
		static unsigned __stdcall _m_work_thread( void * arg);

		/// 
		/// �������/// 
	private:
		sync::csectionlock				callback_lock_;
		SOCKET							listener_socket_;
		HANDLE							iocp_;
		bool							threads_prepared;

		/// ........
		sync::csectionlock				socket_set_lock_;

		///
		/// client ��socket�б�		(volatile)
		/// 
		static std::set<SOCKET>				socket_set_;        

		/// ��̬��funtion  list	
		static std::vector<callback_info>	callbacks;			  

	};

}