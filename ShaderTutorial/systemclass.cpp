////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"


SystemClass::SystemClass()
{
	m_Input = nullptr;
	m_Graphics = nullptr;
	m_Fps = nullptr;
	m_Cpu = nullptr;
	m_Timer = nullptr;
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if(!result)
	{
		return false;
	}

	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}
	m_Fps->Initialize();

	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}
	m_Cpu->Initialize();

	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}
	
	return true;
}


void SystemClass::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	// Release the input object.
	if(m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}

	if (m_Cpu)
	{
		delete m_Cpu;
		m_Cpu = nullptr;
	}

	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
	
	return;
}


void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool SystemClass::Frame()
{
	bool result;
	static bool cKeyPressed = false;
	static bool backFaceCulling = true;
	static bool key6Pressed = false;
	static bool key7Pressed = false;
	static bool key8Pressed = false;

	float cameraSpeed = 0.1f;

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//------------------------------------------------------------------
	if (m_Input->IsKeyDown('W'))
	{
		m_Graphics->GetCamera()->MoveForward(cameraSpeed);
	}
	if (m_Input->IsKeyDown('S'))
	{
		m_Graphics->GetCamera()->MoveForward(-cameraSpeed);
	}
	if (m_Input->IsKeyDown('A'))
	{
		m_Graphics->GetCamera()->MoveRight(-cameraSpeed);
	}
	if (m_Input->IsKeyDown('D'))
	{
		m_Graphics->GetCamera()->MoveRight(cameraSpeed);
	}

	POINT currentMousePos;
	GetCursorPos(&currentMousePos);

	if (m_firstMouse)
	{
		m_lastMousePos = currentMousePos;
		m_firstMouse = false;
	}

	// 마우스 델타값 계산
	float deltaX = (float)(currentMousePos.x - m_lastMousePos.x);
	float deltaY = (float)(currentMousePos.y - m_lastMousePos.y);

	// 카메라 회전
	m_Graphics->GetCamera()->RotateByMouse(deltaX, deltaY);

	// 마우스 위치 업데이트
	m_lastMousePos = currentMousePos;

	// 마우스를 화면 중앙으로 고정
	RECT windowRect;
	GetWindowRect(m_hwnd, &windowRect);
	int centerX = (windowRect.right + windowRect.left) / 2;
	int centerY = (windowRect.bottom + windowRect.top) / 2;
	SetCursorPos(centerX, centerY);
	m_lastMousePos.x = centerX;
	m_lastMousePos.y = centerY;
	//---------------------------------------------------------------------

	if (m_Input->IsKeyDown('R'))
	{
		//m_Graphics->SetBackgroundColor(1.0f, 0.0f, 0.0f);
		m_Graphics->SetLightColor(1.0f, 0.0f, 0.0f);
	}
	else if (m_Input->IsKeyDown('G'))
	{
		//m_Graphics->SetBackgroundColor(0.0f, 1.0f, 0.0f);
		m_Graphics->SetLightColor(0.0f, 1.0f, 0.0f);
	}
	else if (m_Input->IsKeyDown('B'))
	{
		//m_Graphics->SetBackgroundColor(0.0f, 0.0f, 1.0f);
		m_Graphics->SetLightColor(0.0f, 0.0f, 1.0f);
	}
	else if (m_Input->IsKeyDown('F')) // 색상 초기값(black or white)으로 되돌림
	{
		//m_Graphics->SetBackgroundColor(0.0f, 0.0f, 0.0f);
		m_Graphics->SetLightColor(1.0f, 1.0f, 1.0f);
	}

	if (m_Input->IsKeyDown('O'))
	{
		m_Graphics->SetRenderMode(true);
	}
	else if (m_Input->IsKeyDown('P'))
	{
		m_Graphics->SetRenderMode(false);
	}

	if (m_Input->IsKeyDown('C'))
	{
		if (!cKeyPressed)
		{
			backFaceCulling = !backFaceCulling;
			m_Graphics->SetBackFaceCulling(backFaceCulling);
			cKeyPressed = true;
		}
	}
	else
	{
		cKeyPressed = false;
	}

	if (m_Input->IsKeyDown('1'))
	{
		m_Graphics->SetBrightness(1.0f);
	}
	else if (m_Input->IsKeyDown('2'))
	{
		m_Graphics->SetBrightness(0.5f);
	}
	else if (m_Input->IsKeyDown('3'))
	{
		m_Graphics->SetTextureFilter(0);
	}
	else if (m_Input->IsKeyDown('4'))
	{
		m_Graphics->SetTextureFilter(1);
	}
	else if (m_Input->IsKeyDown('5'))
	{
		m_Graphics->SetTextureFilter(2);
	}

	if (m_Input->IsKeyDown('6'))
	{
		if (!key6Pressed)
		{
			m_Graphics->SetLightingMode(1);
			key6Pressed = true;
		}
	}
	else
	{
		key6Pressed = false;
	}

	if (m_Input->IsKeyDown('7'))
	{
		if (!key7Pressed)
		{
			m_Graphics->SetLightingMode(2);
			key7Pressed = true;
		}
	}
	else
	{
		key7Pressed = false;
	}

	if (m_Input->IsKeyDown('8'))
	{
		if (!key8Pressed)
		{
			m_Graphics->SetLightingMode(3);
			key8Pressed = true;
		}
	}
	else
	{
		key8Pressed = false;
	}

	if (m_Input->IsKeyDown(VK_OEM_MINUS) || m_Input->IsKeyDown(VK_SUBTRACT))
	{
		m_Graphics->SetLightBrightness(false);
	}
	else if (m_Input->IsKeyDown(VK_OEM_PLUS) || m_Input->IsKeyDown(VK_ADD))
	{
		m_Graphics->SetLightBrightness(true);
	}

	m_Cpu->Frame();
	m_Timer->Frame();
	m_Fps->Frame();

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if(!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 1600;
		screenHeight = 900;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	m_firstMouse = true;
	GetCursorPos(&m_lastMousePos);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}