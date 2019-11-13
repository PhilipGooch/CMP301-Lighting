#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class StencilShader : public BaseShader
{

public:
	StencilShader(ID3D11Device* device, HWND hwnd);
	~StencilShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* stencil);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateStencil;
	ID3D11Buffer* matrixBuffer;
};

