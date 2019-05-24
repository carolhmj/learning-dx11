#include "RenderTextureClass.h"



RenderTextureClass::RenderTextureClass()
{
	m_renderTargetTexture = nullptr;
	m_renderTargetView = nullptr;
	m_shaderResourceView = nullptr;
}

RenderTextureClass::RenderTextureClass(const RenderTextureClass &)
{
}


RenderTextureClass::~RenderTextureClass()
{
}

bool RenderTextureClass::Initialize(ID3D11Device *device, int textureWidth, int textureHeight)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	return false;
}
