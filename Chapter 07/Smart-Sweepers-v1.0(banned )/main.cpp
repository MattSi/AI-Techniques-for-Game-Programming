#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>


#include "BufferLayer.h"
#include "StandardWindow.h"
#include "LogSpawner.h"
#include "CTimer.h"

using RedContritio::BUFFERLAYER ;
using RedContritio::InitializeWindowClass ;
using RedContritio::CreateVisibleWindow ;
using RedContritio::LogSpawner ;
using RedContritio::Timer ;

#include "utils.hpp"
#include "CParams.h"
#include "CController.h"

const char* MainClassName = "RedContritio Test ANN-SS Project" ;
const char* MainTitleName = "Smart Sweepers" ;
BUFFERLAYER BackBuffer ;
int cxClient ,cyClient ;
int pxMouse ,pyMouse ;
static void GetClientArguments(HWND );

LogSpawner loger ;

PARAMETERASSIST __HiddenParameterAssist ;

CPARAMETERS GlobalParameters ;

CCONTROLLER *pController ;

LRESULT CALLBACK WindowProc(HWND ,UINT ,WPARAM ,LPARAM );

int WINAPI WinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance ,
				   LPSTR lpCmdLine ,int nShowCmd )
{
	loger.printf("Application Started\n" );

	WNDCLASSEX winClass ;
	InitializeWindowClass(&winClass ,WindowProc ,hInstance ,MainClassName );

	if(!RegisterClassEx(&winClass ))
	{
		loger.printf("Window Registration Failed\n" );
		return 0 ;
	}
	HWND hwnd = CreateVisibleWindow(MainClassName ,MainTitleName ,CPARAMETERS::WindowWidth ,CPARAMETERS::WindowHeight ,hInstance );
	if(!hwnd )
	{
		loger.printf("Window Creation Failed\n" );
		return 0 ;
	}
	ShowWindow(hwnd ,nShowCmd );
	UpdateWindow(hwnd );
	
	loger.printf("Window Creation Succeeded\n" );

	bool EndFlag = false ;
	Timer timer(CPARAMETERS::FramesPerSecond );
	MSG msg ;
	while(!EndFlag )
	{
		while(PeekMessage(&msg ,NULL ,0 ,0 ,PM_REMOVE )!= 0)
		{
			if(msg.message == WM_QUIT )
			{
				EndFlag = true ;
			}
			TranslateMessage(&msg );
			DispatchMessage(&msg );
		}

		
		if(timer.ReadyForNextFrame() || pController->FaseRender() )
		{
			if(!(pController->Update()))
			{
				EndFlag = true ;
			}
			InvalidateRect(hwnd, NULL, true);
			UpdateWindow(hwnd);
		}

	}

	UnregisterClass(MainClassName ,winClass.hInstance );
	
	loger.printf("Application Quited\n" );
	return 0 ;
}

LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,WPARAM wParam ,LPARAM lParam )
{
	switch(msg )
	{
		case WM_CREATE :
		{
			srand((unsigned)time(NULL ));
			GetClientArguments(hwnd );
			BackBuffer.ResetBufferLayer(hwnd );
			pController = new CCONTROLLER();
			break ;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			BeginPaint(hwnd ,&ps );

			BitBlt(BackBuffer.GetHdc( ),0 ,0 ,cxClient ,cyClient ,NULL ,0 ,0 ,WHITENESS );

			pController->Render(BackBuffer.GetHdc() ) ;

			BitBlt(ps.hdc ,0 ,0 ,cxClient ,cyClient ,BackBuffer.GetHdc( ),0 ,0 ,SRCCOPY );

			EndPaint(hwnd ,&ps );
			break ;
		}
		case WM_KEYUP :
		{
			switch(wParam )
			{
				case 'R' :
				{
					if(pController )
					{
						delete pController ;
					}
					pController = new CCONTROLLER();
					break ;
				}
				case 'F' :
				{
					pController->FaseRenderToggle();
					break ;
				}
				case VK_RETURN :
				{
					break ;
				}
				case VK_SPACE :
				{
					break ;
				}
				case VK_LEFT : case VK_UP :
				case VK_RIGHT : case VK_DOWN :
				{
					break ;
				}
				case VK_ESCAPE :
				{
					SendMessage(hwnd ,WM_DESTROY ,NULL ,NULL );
					break ;
				}
			}
			break ;
		}
		case WM_SIZE :
		{
			GetClientArguments(hwnd );
			BackBuffer.AdjustSize(hwnd );
			break ;
		}
		case WM_LBUTTONDOWN :
		{
			int x = LOWORD(lParam );
			int y = HIWORD(lParam );

			break ;
		}
		case WM_LBUTTONUP :
		{
			int x = LOWORD(lParam );
			int y = HIWORD(lParam );

			break ;
		}
		case WM_RBUTTONDOWN :
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			break ;
		}
		case WM_RBUTTONUP :
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			;
			break ;
		}
		case WM_MOUSEMOVE :
		{
			pxMouse = LOWORD(lParam );
			pyMouse = HIWORD(lParam );
			;
			break ;
		}
		case WM_COMMAND :
		{
			break ;
		}
		case WM_DESTROY :
		{
			BackBuffer.DeleteBufferLayer( );
			PostQuitMessage(0 );
			if(pController )
			{
				delete pController ;
			}
			break ;
		}
	}
	return DefWindowProc(hwnd ,msg ,wParam ,lParam );
}


static inline void GetClientArguments(HWND hwnd )
{
	RECT rect ;
	GetClientRect(hwnd ,&rect );
	CPARAMETERS::WindowWidth = cxClient = rect.right ;
	CPARAMETERS::WindowHeight = cyClient = rect.bottom ;
}