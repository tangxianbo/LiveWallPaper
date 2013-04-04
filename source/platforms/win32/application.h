#pragma once

#include <Windows.h>
#include "base/singleton.h"


class Application:public Singleton<Application>
{
	friend class Singleton<Application>;
protected: 
	Application():hDC(NULL)
				,hRC(NULL)
				,m_hWnd(NULL)
				,hInstance(NULL)
				//,m_game(NULL)
				,active(TRUE)
				,m_bFullscreen(false)
				,m_screenWidth(960)
				,m_screenHeight(640)
	{
		ZeroMemory(keys,sizeof(keys));
	}
	~Application()
	{
	}

public:
	bool Init();
	int	 Run();
	void ResizeScene(unsigned int width, unsigned int height);

protected:
	bool CreateRenderWindow(LPCWSTR title, int width, int height, int bits, bool isFullScreen,HWND& m_hWnd );
	void CloseRenderWindow(void);


private:
	HDC			hDC;
	HGLRC		hRC;
	HWND		m_hWnd;
	HINSTANCE	hInstance;
	//Game*		m_game;

public:
	bool	keys[256];		
	bool	active;		
	bool	m_bFullscreen;

	int		m_screenWidth;
	int		m_screenHeight;

};