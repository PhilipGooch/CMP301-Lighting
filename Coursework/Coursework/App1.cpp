// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"
#include <string>

App1::App1()
{
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	textureMgr->loadTexture(L"red", L"res/red.png");
	textureMgr->loadTexture(L"green", L"res/green.png");
	textureMgr->loadTexture(L"blue", L"res/blue.png");
	textureMgr->loadTexture(L"yellow", L"res/yellow.png");
	textureMgr->loadTexture(L"magenta", L"res/magenta.png");
	textureMgr->loadTexture(L"cyan", L"res/cyan.png");
	textureMgr->loadTexture(L"white", L"res/white.png");

	textureMgr->loadTexture(L"concrete", L"res/concreteWall.png");
	textureMgr->loadTexture(L"camel", L"res/camel.jpg");
	textureMgr->loadTexture(L"arrow", L"res/arrow.png");
	textureMgr->loadTexture(L"mirror", L"res/mirror.png");
	textureMgr->loadTexture(L"hello", L"res/hello.png");
	textureMgr->loadTexture(L"transparant", L"res/transparant.png");
	textureMgr->loadTexture(L"alphaTestRed", L"res/alphaTestRedA.png");
	textureMgr->loadTexture(L"alphaTestGreen", L"res/alphaTestGreenA.png");

	planeSize = 32;

	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	stencilShader = new StencilShader(renderer->getDevice(), hwnd);

	camera->setPosition(1, 2.0f, 1);
	camera->setRotation(-10.0f, 35.0f, 0.0f);

	portal = new Portal(hwnd, renderer, input, screenWidth, screenHeight, XMFLOAT3(planeSize / 4, 3, planeSize), textureShader, stencilShader, textureMgr);
	portal2 = new Portal(hwnd, renderer, input, screenWidth, screenHeight, XMFLOAT3(planeSize / 2, 3, planeSize), textureShader, stencilShader, textureMgr);

	world = new World(renderer, textureShader, textureMgr, planeSize);

	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);

	quadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

}

bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}


	portal->updateCamera(XMFLOAT3(planeSize - camera->getPosition().x, camera->getPosition().y, planeSize + (planeSize - camera->getPosition().z)), XMFLOAT3(camera->getRotation().x, 180 + camera->getRotation().y, 0));
	portal2->updateCamera(XMFLOAT3(planeSize - camera->getPosition().x, camera->getPosition().y, planeSize + (planeSize - camera->getPosition().z)), XMFLOAT3(camera->getRotation().x, 180 + camera->getRotation().y, 0));


	// clamp camera
	/*if (camera->getPosition().x < 1)
	{
		camera->setPosition(1, camera->getPosition().y, camera->getPosition().z);
	}
	if (camera->getPosition().y < 1)
	{
		camera->setPosition(camera->getPosition().x, 1, camera->getPosition().z);
	}
	if (camera->getPosition().z < 1)
	{
		camera->setPosition(camera->getPosition().x, camera->getPosition().y, 1);
	}
	if (camera->getPosition().x > planeSize - 1)
	{
		camera->setPosition(planeSize - 1, camera->getPosition().y, camera->getPosition().z);
	}
	if (camera->getPosition().y > planeSize - 1)
	{
		camera->setPosition(camera->getPosition().x, planeSize - 1, camera->getPosition().z);
	}
	if (camera->getPosition().z > planeSize - 1)
	{
		camera->setPosition(camera->getPosition().x, camera->getPosition().y, planeSize - 1);
	}*/

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::finalPass()
{
	// Clear the scene. 
	renderer->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();
	
	world->cubes(worldMatrix, viewMatrix, projectionMatrix);

	world->room(worldMatrix, viewMatrix, projectionMatrix);


	renderer->setAlphaBlending(true);
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getOrthoViewMatrix();
	projectionMatrix = renderer->getOrthoMatrix();
	orthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, portal->combinedTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setAlphaBlending(false);

	renderer->setAlphaBlending(true);
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getOrthoViewMatrix();
	projectionMatrix = renderer->getOrthoMatrix();
	orthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, portal2->combinedTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setAlphaBlending(false);


	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();
	return true;
}

bool App1::render()
{
	portal->stencilPass(camera);
	portal->cameraPass(world);
	portal->combinedPass();
	portal2->stencilPass(camera);
	portal2->cameraPass(world);
	portal2->combinedPass();
	finalPass();
	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	//ImGui::Text("FPS: %.2f", timer->getFPS());

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

