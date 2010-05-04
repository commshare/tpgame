#pragma  once

#include "stdafx.h"
#include "lock.h"

namespace netpack
{
	///
	/// �����ʼ��
	/// 
	class wsa_life
	{
	public:
		wsa_life()
		{
			WSADATA wsa_data;
			::WSAStartup( MAKEWORD(2,2),&wsa_data);
		}
		~wsa_life()
		{
			::WSACleanup();
		}
	};


	/// 
	/// ����ͨ�ź�����װ client
	///
	class socket_type
	{
		typedef socket_type self_type;
	public:
		typedef unsigned streamsize;
		
		/// ���������ӵ��׽���Ĭ��ֵ
		struct values
		{
			enum { null_socket = INVALID_SOCKET ,};
		};
		
		/// ���캯��
		socket_type()
			:handle_(values::null_socket)
		{}

		socket_type( SOCKET s, int address_family )
			:handle_(s), addr_family_(address_family)
		{}

		socket_type(int address_family, int protocol , bool is_stream)
			:handle_( values::null_socket)
		{
			open( address_family ,protocol , is_stream);
		}
		
		socket_type( const self_type & rhs)
			:handle_(rhs.handle_) , addr_family_( rhs.addr_family_)
		{
			rhs.handle_ = values::null_socket;
		}

		/// ��д������
		self_type & operator = ( const self_type & rhs)
		{
			if ( this != &rhs)
			{	
				handle_ = rhs.handle_;
				addr_family_ = rhs.addr_family_;
				rhs.handle_ = values::null_socket;
			}
			return *this;
		}

		~socket_type()
		{
			close();
		}

		///
		/// �߼�����
		///

		/// �Ƿ�����
		bool empty() const{ return (handle_ == values::null_socket);}

		/// �õ�socket
		SOCKET GetHandle() const { return handle_ ; }

		/// ��ʼ���׽���
		bool open( int address_ , int proctorl , bool is_stream )
		{
			close();

			handle_ = ::socket( address_ , (is_stream ? SOCK_STREAM : SOCK_DGRAM ) , proctorl );
			
			addr_family_ = address_;

			return ( handle_ != values::null_socket );
		}

		/// �ر��׽���
		void close()
		{
			if ( values::null_socket != handle_)
			{
				::closesocket( handle_ );
				handle_ = values::null_socket;
			}
		}

		/// ���Ӻ���
		bool connect ( const char * hostname , int port )
		{
			if( handle_ != values::null_socket)
			{
				struct sockaddr_in server;
				server.sin_family = addr_family_;
				server.sin_port   = htons(port);
				server.sin_addr.s_addr   = inet_addr(hostname);

				if ( server.sin_addr.s_addr == INADDR_NONE)
				{
					struct hostent * host = ::gethostbyname( hostname );
					if ( host)
					{
						memcpy(&server.sin_addr , host->h_addr_list[0] , host->h_length );
					}
					else
						return false;
				}

				if ( SOCKET_ERROR == ::connect( handle_ , (struct sockaddr*)& server , sizeof( server)))
				{
					close();
					return false;
				}

				return true;
			}
		}

		/// ��ȡ����
		streamsize read( char *  buff , streamsize  bufsize)
		{
			if ( handle_ == values::null_socket )
			{
				return 0;
			}
			/// ����
			streamsize x = ::recv( handle_ , buff , bufsize , 0 );
			buff[ x ] = '\0';
			if ( x == SOCKET_ERROR )
			{
				close();
				
				int error = ::WSAGetLastError();
				error = 5;

				return 0;
			}
			return x;
		}

		/// д����
		streamsize write( const char * buf , streamsize bufsize)
		{
			if ( handle_ == values::null_socket )
			{
				return 0;
			}
			/// ����
			streamsize x = ::send( handle_ , buf, bufsize , 0 );
			if ( x == SOCKET_ERROR )
			{
				int error = ::WSAGetLastError();
				return 0;
			}

			return x;
		}

	private:
		/// �����׽���			
		mutable SOCKET handle_;
		/// ��ַ��  IPV4 �� 6
		int     addr_family_;
	};  // end class clent_socket_type


	/// 
	/// ����ͨ�ź�����װ Server( listener)
	///
	class listener_type
	{
	public:
		struct values
		{
			enum
			{
				null_socket = INVALID_SOCKET,
			};
		};
		
		/// ���캯��
		listener_type()
			:handle_(values::null_socket)
		{}

		listener_type(int address_family, int protocol, bool is_stream)
			:handle_(values::null_socket)
		{
		}

		bool empty() const
		{
			return (handle_ == values::null_socket);
		}

		/// ��ʼ���׽���
		bool open(int address_family, int protocol, bool is_stream)
		{
			close();

			handle_ = ::socket(address_family, (is_stream?SOCK_STREAM:SOCK_DGRAM), protocol);

			addr_family_ = address_family;

			return (handle_ != values::null_socket);
		}

		/// �ر�����
		void close()
		{
			if (  values::null_socket != handle_ )
			 {
				 ::closesocket( handle_ );
				 handle_ = values::null_socket;
			 }
		}

		/// ��
		bool  listener_bind( int port , int connections )
		{
			if ( handle_ == values::null_socket && connections > 0 )
			{
				return 0;
			}
		
			struct sockaddr_in saddr;
			saddr.sin_family = AF_INET;
			saddr.sin_port   = htons(port);
			saddr.sin_addr.s_addr = ::htonl(INADDR_ANY);

			if ( bind( handle_ , reinterpret_cast<sockaddr*>(&saddr), sizeof( saddr)))
				return false;
				
			return ( ::listen( handle_ , connections ) != 0 );
		}

		/// ����
		socket_type accept()
		{
			if ( handle_ != values::null_socket )
			{
				sockaddr_in saddr;
				int size = sizeof saddr;

				SOCKET  s = ::accept( handle_ , reinterpret_cast<sockaddr*>(&saddr), &size);
				if ( s != values::null_socket )
					return socket_type( s , saddr.sin_family ) ;
			}
			return socket_type();
		}

	private:
		/// �����׽���			
		mutable SOCKET handle_;
		/// ��ַ��  IPV4 �� 6
		int     addr_family_;

	};

};