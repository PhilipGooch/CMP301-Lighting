#include "World.h"

World::World(D3D* renderer, TextureShader* textureShader, TextureManager* textureManager, float planeSize) :
	renderer(renderer),
	textureShader(textureShader),
	textureManager(textureManager),
	planeSize(planeSize)
{
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	planeMesh = new MyPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), planeSize + 1);
}

World::~World()
{
}

void World::cubes(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	// front left
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixTranslation(planeSize * 0.5f - 2, 1, planeSize * 0.5f + 2);
	cubeMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"red"));
	textureShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// front right
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixTranslation(planeSize * 0.5f + 2, 1, planeSize * 0.5f + 2);
	cubeMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"green"));
	textureShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// back right
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixTranslation(planeSize * 0.5f + 2, 1, planeSize * 0.5f - 2);
	cubeMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"blue"));
	textureShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// back left
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixTranslation(planeSize * 0.5f - 2, 1, planeSize * 0.5f - 2);
	cubeMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"yellow"));
	textureShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
}

void World::room(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	// bottom
	worldMatrix = renderer->getWorldMatrix();
	planeMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"concrete"));
	textureShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// top
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixRotationRollPitchYaw(PI, 0.0f, 0.0f);
	worldMatrix *= XMMatrixTranslation(0.0f, planeSize, planeSize);
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"concrete"));
	textureShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// left
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixRotationRollPitchYaw(-PI / 2, -PI / 2, 0.0f);
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"concrete"));
	textureShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// right
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixRotationRollPitchYaw(-PI / 2, PI / 2, 0.0f);
	worldMatrix *= XMMatrixTranslation(planeSize, 0.0f, planeSize);
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"concrete"));
	textureShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// front
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixRotationRollPitchYaw(-PI / 2, 0.0f, 0.0f);
	worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, planeSize);
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"concrete"));
	textureShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// back
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixRotationRollPitchYaw(PI / 2, 0.0f, 0.0f);
	worldMatrix *= XMMatrixTranslation(0.0f, planeSize, 0.0f);
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureManager->getTexture(L"concrete"));
	textureShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
}