#include "FontClass.h"



FontClass::FontClass()
{
	m_Font = nullptr;
	m_Texture = nullptr;
}

FontClass::FontClass(const FontClass &)
{
}


FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device * device, ID3D11DeviceContext * context, char * fontFilename, const char *textureFilename)
{
	bool result;


	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, context, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	// Release the font texture.
	ReleaseTexture();

	// Release the font data.
	ReleaseFontData();
}

ID3D11ShaderResourceView * FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void * vertices, char * sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	vertexPtr = (VertexType*)vertices;

	numLetters = (int)strlen(sentence);

	index = 0;

	for (i = 0; i < numLetters; i++) {
		letter = ((int)sentence[i]) - 32;

		//if the letter is a space move over three pixels
		if (letter == 0) {
			drawX = drawX + 3.0f;
		}
		else {
			//first triangle in quad
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f); //top left
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, (drawY - 16), 0.0f); //bottom right
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, drawY - 16, 0.0f); //bottom left
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;


			//second triangle in quad
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f); //top left
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;
			
			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f); //top right
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY - 16, 0.0f); //bottom right
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			//update x for drawing by the size of the letter and one pixel
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}

bool FontClass::LoadFontData(char * filename)
{
	ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer.
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	// Release the font data array.
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}

}

bool FontClass::LoadTexture(ID3D11Device * device, ID3D11DeviceContext* context, const char * filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, context, filename);
	if (!result)
	{
		return false;
	}

	return true;

}

void FontClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}
