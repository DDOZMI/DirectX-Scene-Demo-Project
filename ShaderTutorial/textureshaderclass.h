////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
// The TextureShaderClass is just an updated version of the ColorShaderClass from 
// the previous tutorial. This class will be used to draw the 3D models using vertex 
// and pixel shaders.
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>

#include <fstream>

using namespace std;
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		float time;
		float modelID;
	};

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, float, float);
	bool SetTextureFilter(ID3D11Device*, int);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, float, float);
	void RenderShader(ID3D11DeviceContext*, int);
	bool CreateSamplerState(ID3D11Device*, int);

	bool SetupShaderResources(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, float, float);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;

	int m_currentFilterMode;
};

#endif