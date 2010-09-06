// AutoBall.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AutoBall.h"
#include <windows.h>
#include <time.h>

#include "Config.h"
#include "App/SoccerPitch.h"
#include "Public/PrecisionTimer.h"
#include "Public/GameSetup.h"
#include "Public/Singleton.h"
#include "Render/Vector2D.h"
#include "Render/VGdi.h"


#define MAX_LOADSTRING 100

char*   g_szApplicationName = "Simple Soccer";
char*	g_szWindowClassName = "MyWindowClass";

SoccerPitch* g_SoccerPitch;

//create a timer
PrecisionTimer timer(GetInstObj(CGameSetup).FrameRate);

//used when a user clicks on a menu item to ensure the option is 'checked'
//correctly
void CheckAllMenuItemsAppropriately(HWND hwnd)
{
// 	CheckMenuItemAppropriately(hwnd, IDM_SHOW_REGIONS, GetInstObj(CGameSetup).bRegions);
// 	CheckMenuItemAppropriately(hwnd, IDM_SHOW_STATES, GetInstObj(CGameSetup).bStates);
// 	CheckMenuItemAppropriately(hwnd, IDM_SHOW_IDS, GetInstObj(CGameSetup).bIDs);
// 	CheckMenuItemAppropriately(hwnd, IDM_AIDS_SUPPORTSPOTS, GetInstObj(CGameSetup).bSupportSpots);
// 	CheckMenuItemAppropriately(hwnd, ID_AIDS_SHOWTARGETS, GetInstObj(CGameSetup).bViewTargets);
// 	CheckMenuItemAppropriately(hwnd, IDM_AIDS_HIGHLITE, GetInstObj(CGameSetup).bHighlightIfThreatened);
}

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HDC			g_hWndDC;	 //����DC 
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
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

	//register the window class
	if (!RegisterClassEx(&winclass))
	{
		MessageBox(NULL, "Registration Failed!", "Error", 0);

		//exit the application
		return 0;
	}

	//create the window and assign its ID to hwnd    
	hWnd = CreateWindowEx (NULL,                 // extended style
		g_szWindowClassName,  // window class name
		g_szApplicationName,  // window caption
		WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
		GetSystemMetrics(SM_CXSCREEN)/2 - WindowWidth/2,
		GetSystemMetrics(SM_CYSCREEN)/2 - WindowHeight/2,                    
		WindowWidth,     // initial x size
		WindowHeight,    // initial y size
		NULL,                 // parent window handle
		NULL,                 // window menu handle
		hInstance,            // program instance handle
		NULL);                // creation parameters

	//make sure the window creation has gone OK
	if(!hWnd)
	{
		MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);
	}

	g_hWndDC=GetDC(hWnd);

	//start the timer
	timer.Start();

	MSG msg;

	//enter the message loop
	bool bDone = false;

	while(!bDone)
	{

		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
		{
			if( msg.message == WM_QUIT ) 
			{
				// Stop loop if it's a quit message
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
			//update game states
			g_SoccerPitch->Update(); 

			//render 
			RedrawWindow(hWnd, NULL, NULL,true);

			Sleep(2);
		}

	}//end while

	delete g_SoccerPitch;

	UnregisterClass( g_szWindowClassName, winclass.hInstance );

	return msg.wParam;
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
	//these hold the dimensions of the client window area
	static int cxClient, cyClient; 

	//used to create the back buffer
	static HDC		hdcBackBuffer;
	static HBITMAP	hBitmap;
	static HBITMAP	hOldBitmap;

	switch (msg)
	{

		//A WM_CREATE msg is sent when your application window is first
		//created
	case WM_CREATE:
		{
			//to get get the size of the client window first we need  to create
			//a RECT and then ask Windows to fill in our RECT structure with
			//the client window size. Then we assign to cxClient and cyClient 
			//accordingly
			RECT rect;

			GetClientRect(hwnd, &rect);

			cxClient = rect.right;
			cyClient = rect.bottom;

			//seed random number generator
			srand((unsigned) time(NULL));


			//---------------create a surface to render to(backbuffer)

			//get the DC for the front buffer
			HDC hdc = GetDC(hwnd);

			//create a memory device context
			hdcBackBuffer = CreateCompatibleDC(hdc);

			hBitmap = CreateCompatibleBitmap(hdc,
				cxClient,
				cyClient);


			//select the bitmap into the memory device context
			hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);

			//don't forget to release the DC
			ReleaseDC(hwnd, hdc); 

			g_SoccerPitch = new SoccerPitch(cxClient, cyClient); 

			CheckAllMenuItemsAppropriately(hwnd);

		}

		break;

	case WM_COMMAND:
		{
			/*
			switch(wParam)
			{
			case ID_AIDS_NOAIDS:

				GetInstObj(CGameSetup).bStates        = 0;
				GetInstObj(CGameSetup).bRegions       = 0;
				GetInstObj(CGameSetup).bIDs           = 0;
				GetInstObj(CGameSetup).bSupportSpots  = 0;
				GetInstObj(CGameSetup).bViewTargets   = 0;

				CheckAllMenuItemsAppropriately(hwnd);

				break;

			case IDM_SHOW_REGIONS:

				GetInstObj(CGameSetup).bRegions = !GetInstObj(CGameSetup).bRegions;

				CheckAllMenuItemsAppropriately(hwnd);

				break;

			case IDM_SHOW_STATES:

				GetInstObj(CGameSetup).bStates = !GetInstObj(CGameSetup).bStates;

				CheckAllMenuItemsAppropriately(hwnd);

				break;

			case IDM_SHOW_IDS:

				GetInstObj(CGameSetup).bIDs = !GetInstObj(CGameSetup).bIDs;

				CheckAllMenuItemsAppropriately(hwnd);

				break;


			case IDM_AIDS_SUPPORTSPOTS:

				GetInstObj(CGameSetup).bSupportSpots = !GetInstObj(CGameSetup).bSupportSpots;

				CheckAllMenuItemsAppropriately(hwnd);

				break;

			case ID_AIDS_SHOWTARGETS:

				GetInstObj(CGameSetup).bViewTargets = !GetInstObj(CGameSetup).bViewTargets;

				CheckAllMenuItemsAppropriately(hwnd);

				break;

			case IDM_AIDS_HIGHLITE:

				GetInstObj(CGameSetup).bHighlightIfThreatened = !GetInstObj(CGameSetup).bHighlightIfThreatened; 

				CheckAllMenuItemsAppropriately(hwnd);

				break;
				*/

			//}//end switch
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
					delete g_SoccerPitch;

					g_SoccerPitch = new SoccerPitch(cxClient, cyClient);
				}

				break;

			case 'P':
				{
					g_SoccerPitch->TogglePause();
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

			g_SoccerPitch->Render();

			GetInstObj(CGDI).StopDrawing(hdcBackBuffer);



			//now blit backbuffer to front
			BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY); 

			EndPaint (hwnd, &ps);

		}

		break;

		//has the user resized the client area?
	case WM_SIZE:
		{
			//if so we need to update our variables so that any drawing
			//we do using cxClient and cyClient is scaled accordingly
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			//now to resize the backbuffer accordingly. First select
			//the old bitmap back into the DC
			SelectObject(hdcBackBuffer, hOldBitmap);

			//don't forget to do this or you will get resource leaks
			DeleteObject(hBitmap); 

			//get the DC for the application
			HDC hdc = GetDC(hwnd);

			//create another bitmap of the same size and mode
			//as the application
			hBitmap = CreateCompatibleBitmap(hdc,
				cxClient,
				cyClient);

			ReleaseDC(hwnd, hdc);

			//select the new bitmap into the DC
			SelectObject(hdcBackBuffer, hBitmap);

		}

		break;

	case WM_DESTROY:
		{

			//clean up our backbuffer objects
			SelectObject(hdcBackBuffer, hOldBitmap);

			DeleteDC(hdcBackBuffer);
			DeleteObject(hBitmap); 

			// kill the application, this sends a WM_QUIT message  
			PostQuitMessage (0);
		}

		break;

	}//end switch

	//this is where all the messages not specifically handled by our 
	//winproc are sent to be processed
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
