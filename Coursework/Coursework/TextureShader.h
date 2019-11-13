#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class TextureShader : public BaseShader
{

public:
	TextureShader(ID3D11Device* device, HWND hwnd);
	~TextureShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* matrixBuffer;
};

