#include "Portal.h"

Portal::Portal(HWND hwnd, D3D* renderer, Input* input, int screenWidth, int screenHeight, XMFLOAT3 position, 
			   TextureShader* textureShader, StencilShader* stencilShader, TextureManager* textureManager) :
	renderer(renderer),
	position(position),
	textureShader(textureShader),
	stencilShader(stencilShader),
	textureManager(textureManager)
{
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);
	stencilQuadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());

	cameraTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, 0.1f, 100.0f);
	stencilTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, 0.1f, 100.0f);

	combinedTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, 0.1f, 100.0f);
	camera = new FPCamera(input, screenWidth, screenHeight, hwnd);
}

Portal::~Portal()
{
}

void Portal::updateCamera(XMFLOAT3 playerPosition, XMFLOAT3 playerLookAt)		// needs updating... (not passing player position currently)
{
	camera->setPosition(playerPosition.x, playerPosition.y, playerPosition.z);
	camera->setRotation(playerLookAt.x, playerLookAt.y, playerLookAt.z);
}

void Portal::stencilPass(FPCamera* playerCamera)
{
	stencilTexture->setRenderTarget(renderer->getDeviceContext());
	stencilTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = playerCamera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	playerCamera->update();

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixTranslation(position.x, position.y, position.z);
	stencilQuadMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"green"));
	textureShader->render(renderer->getDeviceContext(), stencilQuadMesh->getIndexCount());

	renderer->setBackBufferRenderTarget();
}

void Portal::cameraPass(World* world)
{
	cameraTexture->setRenderTarget(renderer->getDeviceContext());
	cameraTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	camera->update();

	world->cubes(worldMatrix, viewMatrix, projectionMatrix);

	world->room(worldMatrix, viewMatrix, projectionMatrix);


	renderer->setBackBufferRenderTarget();
}

void Portal::combinedPass()
{
	combinedTexture->setRenderTarget(renderer->getDeviceContext());
	combinedTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getOrthoViewMatrix();
	XMMATRIX projectionMatrix = renderer->getOrthoMatrix();
	renderer->setAlphaBlending(true);
	orthoMesh->sendData(renderer->getDeviceContext());
	stencilShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, cameraTexture->getShaderResourceView(), stencilTexture->getShaderResourceView());
	stencilShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setAlphaBlending(false);

	renderer->setBackBufferRenderTarget();
}
