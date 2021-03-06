#pragma once

#include "inputclass.h"
#include "graphicsclass.h"
#include <Windows.h>

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool initialize();
	void shutdown();
	void run();

	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool frame();
	void initializeWindows(int&, int&);
	void shutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass *m_input;
	GraphicsClass *m_graphics;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass *applicationHandle = nullptr;
