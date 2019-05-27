#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_FogShader = nullptr;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	//Create the model object
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	//m_Camera->SetPosition(0.0f, 0.0f, -8.0f);

	//Create the new model object
	m_Model = new ModelClass;
	if (!m_Model) {
		return false;
	}

	//Initialize the model object
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "cube.txt", { "owl.tga" });
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	//Create the fog shader object
	m_FogShader = new FogShaderClass;
	if (!m_FogShader) {
		return false;
	}

	//Initialize
	result = m_FogShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the fog shader object", L"Error", MB_OK);
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	//Release the fog shader object
	if (m_FogShader) {
		m_FogShader->Shutdown();
		delete m_FogShader;
		m_FogShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

bool GraphicsClass::Frame()
{
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	return true;
}

bool GraphicsClass::Render() {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	static float rotation = 0.0f;
	float fogColor, fogStart, fogEnd;

	//Set the color of the fog to gray
	fogColor = 0.5f;

	//Set start and end of fog
	fogStart = 0.0f;
	fogEnd = 10.0f;

	//Clear the buffers to begin the scene
	m_Direct3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);
	
	m_Camera->Render();

	//Get the world, view and projection matrix based on camera's position
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	//Update the rotation variable each frame
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	worldMatrix = XMMatrixRotationY(rotation);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_FogShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextures()[0], fogStart, fogEnd);

	//Present the rendered scene to the screen
	m_Direct3D->EndScene();

	return true;
}