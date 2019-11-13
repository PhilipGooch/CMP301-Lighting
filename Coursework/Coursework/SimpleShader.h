#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class LightShader : public BaseShader
{
private:
	struct ManipulationBufferType
	{
		float time;
		float constantFactor;
		float linearFactor;
		float quadraticFactor;
		
		float isLight;
		float isDirectionalLightOn;
		float isSpotLightOn;
		float isRedPointLightOn;
		float isGreenPointLightOn;
		float isBluePointLightOn;
		XMFLOAT2 padding;
	};

	struct DirectionalLightBufferType
	{
		XMFLOAT4 ambientColour;
		XMFLOAT4 diffuseColour;
		XMFLOAT3 lightDirection;
		float padding;
	};

	struct PointLightBufferType
	{
		XMFLOAT4 ambientColour[3];
		XMFLOAT4 diffuseColour[3];
		XMFLOAT4 position[3];
	};

	struct SpotLightBufferType
	{
		XMFLOAT4 ambientColour;
		XMFLOAT4 diffuseColour;
		XMFLOAT4 position;
		XMFLOAT4 direction;
	};

public:
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, Light* directionalLight, Light** pointLights, Light* spotLight, float constantFactor, float linearFactor, float quadraticFactor, float time, bool isLight, bool isDirectionalLightOn, bool isSpotLightOn, bool isRedPointLightOn, bool isGreenPointLightOn, bool isBluePointLightOn);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* manipulationBuffer;
	ID3D11Buffer* directionalLightBuffer;
	ID3D11Buffer* pointLightBuffer;
	ID3D11Buffer* spotLightBuffer;
};
