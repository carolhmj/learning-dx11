#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_TextureShader = nullptr;
	m_LightShader = nullptr;
	m_FontShader = nullptr;
	m_Bitmap = nullptr;
	m_Text = nullptr;
	m_Light = nullptr;
	m_Model = nullptr;
	m_MultiTextureShader = nullptr;
	m_AlphaMapShader = nullptr;
	m_BumpMapShader = nullptr;
	m_TextureShader = nullptr;
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
	XMMATRIX baseViewMatrix;

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

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

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

	//Create the light shader object
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	//Initialize the light shader object
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the light shader object", L"Error", MB_OK);
		return false;
	}

	//Create the light object
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	//Initialize the light object
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	//Create the render to texture object
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture) {
		return false;
	}

	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result) {
		return false;
	}

	//Create the debug window object
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow) {
		return false;
	}

	//Initialize the debug window object
	result = m_DebugWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, 100, 75);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the debug window object", L"Error", MB_OK);
		return false;
	}

	//Create the texture shader object
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader) {
		return false;
	}

	//Initialize the texture shader object
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initalize the texture shader object", L"Error", MB_OK);
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the debug window object.
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
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
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	//Render the entire scene to the texture first
	result = RenderToTexture();
	if (!result) {
		return false;
	}

	//Clear the buffers to begin the scene
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//Render the scene as normal to the back buffer
	result = RenderScene();
	if (!result) {
		return false;
	}

	//Turn off the Z buffer to begin 2D rendering
	m_Direct3D->TurnZBufferOff();

	//Get the world, view and ortho matrices from the camera and d3d objects
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//Put the debug window vertex and index buffers on the graphics pipeline to prepare them for drawing
	result = m_DebugWindow->Render(m_Direct3D->GetDeviceContext(), 50, 50);
	if (!result) {
		return false;
	}

	//Render the debug window using the texture shader
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView());
	if (!result) {
		return false;
	}

	//Turn the Z buffer back on now
	m_Direct3D->TurnZBufferOn();

	//Present the rendered scene to the screen
	m_Direct3D->EndScene();

	return true;
}

bool GraphicsClass::RenderToTexture() {
	bool result;

	//Set the render target to be the render to texture
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	//Clear the render to texture
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	//Render the scene now and it will draw to the texture instead of the black buffer
	result = RenderScene();
	if (!result) {
		return false;
	}

	//Reset the render target back to the back buffer
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene() {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	static float rotation = 0.0f;

	//Generate the view matrix based on the camera's position
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

	//Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	m_Model->Render(m_Direct3D->GetDeviceContext());

	//Render the model using the light shader
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextures()[0],
		m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result) {
		return false;
	}

	return true;
}


bool GraphicsClass::Frame(int fps, int cpu, float frameTime)
{
	bool result;
	static float rotation = 0.0f;

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	//update rotation each frame
	rotation += XM_PI * 0.005f;
	if (rotation > 2*XM_PI) {
		rotation = 0;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//Turn z-Buffer off for 2D rendering
	m_Direct3D->TurnZBufferOff();

	//result = m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 0, 0);
	//if (!result) {
	//	return false;
	//}

	////render the bitmap with texture shader
	//result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	//if (!result) {
	//	return false;
	//}
	
	m_Direct3D->TurnOnAlphaBlending();

	result = m_Text->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result) {
		return false;
	}

	m_Direct3D->TurnOffAlphaBlending();

	//turn the z-buffer back on
	m_Direct3D->TurnZBufferOn();

	worldMatrix = XMMatrixRotationY(rotation);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	m_BumpMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextures(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	m_Direct3D->EndScene();

	return true;
}