#pragma once
//////////////
// INCLUDES //
//////////////
#include "D3DClass.h"
#include "CameraClass.h"
#include "TextureShaderClass.h"
#include "BitmapClass.h"
#include "TextClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"
#include "ModelClass.h"
#include "MultiTextureShaderClass.h"
#include "AlphaMapShaderClass.h"


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

private:
	bool Render(float);

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TextureShaderClass *m_TextureShader;
	FontShaderClass *m_FontShader;
	LightShaderClass *m_LightShader;
	LightClass *m_Light;
	ModelClass *m_Model;

	BitmapClass* m_Bitmap;
	TextClass* m_Text;

	MultiTextureShaderClass* m_MultiTextureShader;
	AlphaMapShaderClass* m_AlphaMapShader;
};
