#include "SimpleShader.h"

LightShader::LightShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"simple_vs.cso", L"simple_ps.cso");
}


LightShader::~LightShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the light constant buffer.
	if (directionalLightBuffer)
	{
		directionalLightBuffer->Release();
		directionalLightBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void LightShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Setup the description of the dynamic constant buffers that are in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.

	// Manipulation buffer
	D3D11_BUFFER_DESC manipulationBufferDesc;
	manipulationBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	manipulationBufferDesc.ByteWidth = sizeof(ManipulationBufferType);
	manipulationBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	manipulationBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	manipulationBufferDesc.MiscFlags = 0;
	manipulationBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&manipulationBufferDesc, NULL, &manipulationBuffer);

	// Directional light buffer
	D3D11_BUFFER_DESC directionalLightBufferDesc;
	directionalLightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	directionalLightBufferDesc.ByteWidth = sizeof(DirectionalLightBufferType);
	directionalLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	directionalLightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	directionalLightBufferDesc.MiscFlags = 0;
	directionalLightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&directionalLightBufferDesc, NULL, &directionalLightBuffer);

	// Point light buffer
	D3D11_BUFFER_DESC pointLightBufferDesc;
	pointLightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pointLightBufferDesc.ByteWidth = sizeof(PointLightBufferType);
	pointLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pointLightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pointLightBufferDesc.MiscFlags = 0;
	pointLightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&pointLightBufferDesc, NULL, &pointLightBuffer);

	// Spot light buffer
	D3D11_BUFFER_DESC spotLightBufferDesc;
	spotLightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	spotLightBufferDesc.ByteWidth = sizeof(SpotLightBufferType);
	spotLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	spotLightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	spotLightBufferDesc.MiscFlags = 0;
	spotLightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&spotLightBufferDesc, NULL, &spotLightBuffer);
}


void LightShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture, Light* directionalLight, Light** pointLights, Light* spotLight, float pointLightConstantFactor, float pointLightLinearFactor, float pointLightQuadraticFactor, float spotLightConstantFactor, float spotLightLinearFactor, float spotLightQuadraticFactor, float time, bool isLight, bool isDirectionalLightOn, bool isSpotLightOn, bool isRedPointLightOn, bool isGreenPointLightOn, bool isBluePointLightOn)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	MatrixBufferType* dataPtr;
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	ManipulationBufferType* manipulationPtr;
	deviceContext->Map(manipulationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	manipulationPtr = (ManipulationBufferType*)mappedResource.pData;
	manipulationPtr->time = time;
	manipulationPtr->pointLightConstantFactor = pointLightConstantFactor;
	manipulationPtr->pointLightLinearFactor = pointLightLinearFactor;
	manipulationPtr->pointLightQuadraticFactor = pointLightQuadraticFactor;
	manipulationPtr->spotLightConstantFactor = spotLightConstantFactor;
	manipulationPtr->spotLightLinearFactor = spotLightLinearFactor;
	manipulationPtr->spotLightQuadraticFactor = spotLightQuadraticFactor;
	manipulationPtr->isLight = isLight ? float(1.0f) : float(0.0f);
	manipulationPtr->isDirectionalLightOn = isDirectionalLightOn ? float(1.0f) : float(0.0f);
	manipulationPtr->isSpotLightOn = isSpotLightOn ? float(1.0f) : float(0.0f);
	manipulationPtr->isRedPointLightOn = isRedPointLightOn ? float(1.0f) : float(0.0f);
	manipulationPtr->isGreenPointLightOn = isGreenPointLightOn ? float(1.0f) : float(0.0f);
	manipulationPtr->isBluePointLightOn = isBluePointLightOn ? float(1.0f) : float(0.0f);
	deviceContext->Unmap(manipulationBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &manipulationBuffer);

	DirectionalLightBufferType* directionalLightPtr;
	deviceContext->Map(directionalLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	directionalLightPtr = (DirectionalLightBufferType*)mappedResource.pData;
	directionalLightPtr->ambientColour = directionalLight->getAmbientColour();
	directionalLightPtr->diffuseColour = directionalLight->getDiffuseColour();
	directionalLightPtr->lightDirection = directionalLight->getDirection();
	deviceContext->Unmap(directionalLightBuffer, 0);
	deviceContext->PSSetConstantBuffers(1, 1, &directionalLightBuffer);

	PointLightBufferType* pointLightPtr;
	deviceContext->Map(pointLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	pointLightPtr = (PointLightBufferType*)mappedResource.pData;
	for (int i = 0; i < 3; i++)
	{
		pointLightPtr->ambientColour[i] = pointLights[i]->getAmbientColour();
		pointLightPtr->diffuseColour[i] = pointLights[i]->getDiffuseColour();
		pointLightPtr->position[i] = XMFLOAT4(pointLights[i]->getPosition().x, pointLights[i]->getPosition().y, pointLights[i]->getPosition().z, 1.0f);
	}
	deviceContext->Unmap(pointLightBuffer, 0);
	deviceContext->PSSetConstantBuffers(2, 1, &pointLightBuffer);

	SpotLightBufferType* spotLightPtr;
	deviceContext->Map(spotLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	spotLightPtr = (SpotLightBufferType*)mappedResource.pData;
	spotLightPtr->ambientColour = spotLight->getAmbientColour();
	spotLightPtr->diffuseColour = spotLight->getDiffuseColour();
	spotLightPtr->direction = XMFLOAT4(spotLight->getDirection().x, spotLight->getDirection().y, spotLight->getDirection().z, 1.0f);
	spotLightPtr->position = XMFLOAT4(spotLight->getPosition().x, spotLight->getPosition().y, spotLight->getPosition().z, 1.0f);
	deviceContext->Unmap(spotLightBuffer, 0);
	deviceContext->PSSetConstantBuffers(3, 1, &spotLightBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}
