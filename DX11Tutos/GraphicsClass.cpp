#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_ClipPlaneShader = nullptr;
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

	m_Camera->SetPosition(0.0f, 2.0f, -10.0f);

	//Create the new model object
	m_Model = new ModelClass;
	if (!m_Model) {
		return false;
	}

	//Initialize the model object
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "cube.txt", { "stone01.tga" });
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	// Create the clip plane shader object.
	m_ClipPlaneShader = new ClipPlaneShaderClass;
	if (!m_ClipPlaneShader)
	{
		return false;
	}

	// Initialize the clip plane shader object.
	result = m_ClipPlaneShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the clip plane shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the clip plane shader object.
	if (m_ClipPlaneShader)
	{
		m_ClipPlaneShader->Shutdown();
		delete m_ClipPlaneShader;
		m_ClipPlaneShader = 0;
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

bool GraphicsClass::Render() {
	XMFLOAT4 clipPlane;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	static float angle = 0.0f;

	angle += XM_PI * 0.005;

	if (angle > 360.0f) angle -= 360.0f;

	//Setup a clipping plane 
	clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);

	//Clear the buffers to begin the scene
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	
	m_Camera->Render();

	//Get the world, view and projection matrix based on camera's position
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixRotationY(angle);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_ClipPlaneShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTextures()[0], clipPlane);
	if (!result) {
		return false;
	}

	//Present the rendered scene to the screen
	m_Direct3D->EndScene();

	return true;
}