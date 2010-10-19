// AutoBall.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AutoBall.h"
#include <windows.h>
#include <time.h>

#include "Config.h"
#include "App/SoccerPitch.h"
#include "Public/Timer.h"
#include "Public/GameSetup.h"
#include "Public/Singleton.h"
#include "Public/Stream_Fun.h"
#include "Render/Vector2D.h"
#include "Render/VGdi.h"
#include "Render/Utils.h"
#include <luabind/luabind.hpp>
#include <iostream>

extern "C"
{
#include <lua.h>
#include <Lualib.h>
#include <lauxlib.h>
};

using namespace luabind;
using namespace std;

#define MAX_LOADSTRING 100

const	char*		g_szApplicationName = "FootBall()";
const	char*		g_szWindowClassName = "MyWindowClass";
SoccerPitch*		g_SoccerPitch;//ȫ�ֵ�
HBITMAP				g_Bgmp;		  //λͼ���
HDC					g_Mdc;		  //
luabind::object		g_states;	  //ȫ�ֵ�luaObject
lua_State*			g_pLua;		  //g_pLuaȫ�ֶ���
HDC					g_hWndDC;	  //����DC 

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HWND hWnd,int nCmdShow);
void				MyPaint(HDC hdcc);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
extern  void		ReisterAllFun(lua_State* g_pLua);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	try{
		UNREFERENCED_PARAMETER(hPrevInstance);
		UNREFERENCED_PARAMETER(lpCmdLine);

		//handle to our window
		HWND						hWnd;

		//our window class structure
		WNDCLASSEX     winclass;

		// first fill in the window class stucture
		winclass.cbSize        = sizeof(WNDCLASSEX);
		winclass.style         = CS_HREDRAW | CS_VREDRAW;
		winclass.lpfnWndProc   = WndProc;
		winclass.cbClsExtra    = 0;
		winclass.cbWndExtra    = 0;
		winclass.hInstance     = hInstance;
		winclass.hIcon		   = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AUTOBALL));
		winclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		winclass.hbrBackground = NULL;
		winclass.lpszMenuName  = MAKEINTRESOURCE(IDC_AUTOBALL);
		winclass.lpszClassName = g_szWindowClassName;
		winclass.hIconSm       = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
		CTimer timer(GetInstObj(CGameSetup).FrameRate);

		//create a lua state
		g_pLua = lua_open();

		//open the lua
		luaL_openlibs(g_pLua);

		//open luabind
		open(g_pLua);

		ReisterAllFun(g_pLua);

		if (int error = luaL_dofile(g_pLua, "BallAI.lua") != 0)
		{
			throw std::runtime_error("ERROR(" + ttos(error) + "): Problem with lua script file BallAI.lua");
		}

		g_states = globals(g_pLua);


		if (type(g_states) != LUA_TTABLE)
		{
			throw std::runtime_error("ERROR: runing lua script file BallAI.lua error!");
			return 0;
		}

		if (!RegisterClassEx(&winclass))
		{
			MessageBox(NULL, "Registration Failed!", "Error", 0);

			return 0;
		}

		//create the window and assign its ID to hwnd    
		hWnd = CreateWindowEx (NULL,
			g_szWindowClassName, 
			g_szApplicationName, 
			WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			GetSystemMetrics(SM_CXSCREEN)/2 - WindowWidth/2,
			GetSystemMetrics(SM_CYSCREEN)/2 - WindowHeight/2,                    
			WindowWidth,          
			WindowHeight,        
			NULL,                 
			NULL,                
			hInstance,           
			NULL);

		//make sure the window creation has gone OK
		if(!hWnd)
		{
			MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);
		}

		InitInstance (hWnd,nCmdShow);

		g_hWndDC=GetDC(hWnd);

		timer.Start();

		MSG msg;
		bool bDone = false;

		while(!bDone)
		{
			while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{
				if( msg.message == WM_QUIT ) 
				{
					bDone = true;
				} 

				else 
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}

			if (timer.ReadyForNextFrame() && msg.message != WM_QUIT)
			{

				g_SoccerPitch->Update(); 

				//render 
				RedrawWindow(hWnd, NULL, NULL,true);

				Sleep(2);
			}

		}//end while

		SAFE_DELETE( g_SoccerPitch );

		UnregisterClass( g_szWindowClassName, winclass.hInstance );

		return msg.wParam;
	}
	catch(luabind::error e)
	{

#ifdef  ERROR_LOG 
		//debug_con << e.what() <<std::endl;
#endif
		__asm int 3
	}
	catch(luabind::cast_failed e)
	{
#ifdef  ERROR_LOG 
		//debug_con << e.what() <<std::endl;
#endif
		__asm int 3 
	}
	catch( ... )
	{
		__asm int 3 
	}
	return 0;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AUTOBALL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_AUTOBALL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//

void MyPaint(HDC hdc)
{
	//�鿴�����Ĳ���ϵͳ�汾
	SelectObject(g_Mdc,g_Bgmp);
	BitBlt(hdc,0,0,1050,680,g_Mdc,0,0,SRCCOPY);
}

BOOL InitInstance(HWND hWnd,int nCmdShow)
{
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	HDC  hdc_ = GetDC(hWnd);
	g_Mdc  = CreateCompatibleDC(hdc_);

	SelectObject(g_Mdc,g_Bgmp);

	g_Bgmp = (HBITMAP)LoadImage(NULL,"background.bmp",IMAGE_BITMAP,1050,680,LR_LOADFROMFILE);

	SelectObject(g_Mdc,g_Bgmp);
	ReleaseDC(hWnd,hdc_);
	DeleteDC(hdc_);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int cxClient, cyClient; 

	static HDC		hdcBackBuffer;
	static HBITMAP	hBitmap;
	static HBITMAP	hOldBitmap;

	switch (msg)
	{
	case WM_CREATE:
		{
			RECT rect;

			GetClientRect(hwnd, &rect);

			cxClient = rect.right;
			cyClient = rect.bottom;

			srand((unsigned) time(NULL));

			HDC hdc = GetDC(hwnd);

			hdcBackBuffer = CreateCompatibleDC(hdc);

			hBitmap = CreateCompatibleBitmap(hdc,
				cxClient,
				cyClient);

			hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);

			ReleaseDC(hwnd, hdc); 

			g_SoccerPitch = new SoccerPitch(cxClient, cyClient); 

		}

		break;

	case WM_KEYUP:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				{             
					SendMessage(hwnd, WM_DESTROY, NULL, NULL);
				}

				break;

			case 'R':
				{
					SAFE_DELETE( g_SoccerPitch );

					g_SoccerPitch = new SoccerPitch(cxClient, cyClient);
				}
				break;


			}//end switch

		}//end WM_KEYUP

		break;


	case WM_PAINT:
		{	

			PAINTSTRUCT ps;

			BeginPaint (hwnd, &ps);

			GetInstObj(CGDI).StartDrawing(hdcBackBuffer);

			MyPaint(hdcBackBuffer);

			g_SoccerPitch->Render();

			GetInstObj(CGDI).StopDrawing(hdcBackBuffer);

			BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY); 

			EndPaint (hwnd, &ps);

		}

		break;
	case WM_DESTROY:
		{
			SelectObject(hdcBackBuffer, hOldBitmap);

			DeleteDC(hdcBackBuffer);
			DeleteObject(hBitmap); 

			PostQuitMessage (0);
		}

		break;

	}
	return DefWindowProc (hwnd, msg, wParam, lParam);
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

