#include "SystemClass.h"
#include "InputClass.h"
#include "GraphicsClass.h"

SystemClass::SystemClass()
{
	m_input = nullptr;
	m_graphics = nullptr;
}


SystemClass::~SystemClass()
{
}

bool SystemClass::initialize() {
	int screenWidth = 0, screenHeight = 0;
	bool result;

	initializeWindows(screenWidth, screenHeight);

	m_input = new InputClass;
	if (!m_input) {
		return false;
	}
	m_input->Initialize();

	m_graphics = new GraphicsClass;
	if (!m_graphics) {
		return false;
	}
	result = m_graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result) {
		return false;
	}

	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void SystemClass::shutdown() {
	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	if (m_graphics) {
		m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = nullptr;
	}

	if (m_input) {
		delete m_input;
		m_input = nullptr;
	}

	shutdownWindows();
}

void SystemClass::run() {
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			result = frame();
			if (!result) {
				done = true;
			}
		}
	}
}

bool SystemClass::frame() {
	bool result;

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	//Check if user pressed esc
	if (m_input->IsKeyDown(VK_ESCAPE)) {
		return false;
	}

	result = m_graphics->Frame();
	if (!result) {
		return false;
	}

	result = m_graphics->Render();
	if (!result) {
		return false;
	}
	return true;
}

LRESULT CALLBACK SystemClass::messageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg)
	{
		case WM_KEYDOWN: {
			m_input->KeyDown((unsigned int)wparam);
			return 0;
		}
		case WM_KEYUP: {
			m_input->KeyUp((unsigned int)wparam);
			return 0;
		}
		default: {
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::initializeWindows(int& screenWidth, int& screenHeight) {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//External pointer to this object
	applicationHandle = this;

	//Instance of this application
	m_hinstance = GetModuleHandle(NULL);

	//Application name
	m_applicationName = L"DX11Tutos";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Additional elements of the window class
	/*
		CS_HREDRAW - Redraws the entire window if a movement or size adjustment changes the width of the client area.
		CS_VREDRAW - Redraws the entire window if a movement or size adjustment changes the height of the client area.
		CS_OWNDC - Allocates a unique device context for each window in the class. 
	*/
	wc.lpfnWndProc = WndProc; //Address of the window procedure class that defines the entry point used to process all messages for windows in the class
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance; //Identifies the application or .dll that registered the class
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); //Large icon 
	wc.hIconSm = wc.hIcon; //Small icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Shape of the cursor when it is in the client area of a window in the class
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //Prepares the client area of a window for subsequent drawing by the application. Fills the client area with a solid color or pattern
	wc.lpszMenuName = NULL; //Defines the default menu. Refers to the resource name of the menu
	wc.lpszClassName = m_applicationName; //Class name that distinguishes a window class from another in the same process
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, m_hinstance, nullptr);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);
}

void SystemClass::shutdownWindows() {
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
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
	applicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	if (umessage == WM_DESTROY || umessage == WM_CLOSE) {
		PostQuitMessage(0);
		return 0;
	}
	else {
		return applicationHandle->messageHandler(hwnd, umessage, wparam, lparam);
	}
}