#pragma once

#include <d3d11.h>
#include <fstream>

class TextureArrayClass
{
public:

	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*, const char*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	bool InitializeShaderResourceView(ID3D11Device*, ID3D11DeviceContext*, const char*, ID3D11ShaderResourceView** shaderResourceView);
	bool LoadTarga(const char*, int&, int&);
	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textures[2];
};

