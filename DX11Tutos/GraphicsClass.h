#pragma once
//////////////
// INCLUDES //
//////////////
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ClipPlaneShaderClass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.00001f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float);
	bool Render();

private:
	bool Render(float);
	bool RenderToTexture();
	bool RenderScene();

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ClipPlaneShaderClass* m_ClipPlaneShader;
};
