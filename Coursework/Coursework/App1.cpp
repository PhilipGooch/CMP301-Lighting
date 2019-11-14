// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"
#include <string>

App1::App1()
{
	sphereMesh = nullptr;
	simpleShader = nullptr;
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

	portalTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	planeSize = 32;

	directionalLight = new Light;
	directionalLight->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	directionalLight->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight->setDirection(1.0f, -1.0f, 1.0f);

	isDirectionalLightOn = false;
	isSpotLightOn = false;

	spotLightConstantFactor = 0.2f;
	spotLightLinearFactor = 0.025f;
	spotLightQuadraticFactor = 0.0f;
	
	pointLights[0] = new PointLight(textureMgr, XMFLOAT3(planeSize * 0.25f, 2.0f, planeSize * 0.75f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.02f, 0.0f, 0.0f, 1.0f));
	pointLights[1] = new PointLight(textureMgr, XMFLOAT3(planeSize * 0.5f, 2.0f, planeSize * 0.75f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.02f, 1.0f));
	pointLights[2] = new PointLight(textureMgr, XMFLOAT3(planeSize * 0.75f, 2.0f, planeSize * 0.75f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.02f, 1.0f));
	

	spotLightDiffuseColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	spotLightDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	spotLight = new Light;
	spotLight->setDiffuseColour(spotLightDiffuseColour.x, spotLightDiffuseColour.y, spotLightDiffuseColour.z, spotLightDiffuseColour.w);
	spotLight->setDirection(spotLightDirection.x, spotLightDirection.y, spotLightDirection.z);

	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	planeMesh = new MyPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), planeSize + 1);

	simpleShader = new LightShader(renderer->getDevice(), hwnd);

	time = 0.0f;

	camera->setPosition(1, 2.0f, 1);
	camera->setRotation(-10.0f, 35.0f, 0.0f);

	portalCamera = new Camera();
	portalCamera->setPosition(-planeSize / 2, 0, 0);
	portalCamera->setRotation(90, 0, 0);

	portalMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());


}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (sphereMesh)
	{
		delete sphereMesh;
		sphereMesh = 0;
	}

	if (planeMesh)
	{
		delete planeMesh;
		planeMesh = 0;
	}

}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	time += timer->getTime();
	
	for (int i = 0; i < 3; i++)
	{
		pointLights[i]->setPosition();
	}
	
	rotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(camera->getRotation().x), XMConvertToRadians(camera->getRotation().y), XMConvertToRadians(camera->getRotation().z));

	XMVECTOR lookAt = XMVectorSet(0.0, 0.0, 1.0f, 1.0f);
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);

	spotLightDirection.x = XMVectorGetX(lookAt);
	spotLightDirection.y = XMVectorGetY(lookAt);
	spotLightDirection.z = XMVectorGetZ(lookAt);

	spotLight->setPosition(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
	spotLight->setDirection(spotLightDirection.x, spotLightDirection.y, spotLightDirection.z);

	if (camera->getPosition().x < 1)
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
	}

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

void App1::finalPass()
{

	// Clear the scene. (default blue colour)
	//renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
	renderer->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	// Cube
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixTranslation(planeSize * 0.66f, 1, planeSize * 0.33f);
	cubeMesh->sendData(renderer->getDeviceContext());
	simpleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"camel"), directionalLight, pointLights, spotLight, spotLightConstantFactor, spotLightLinearFactor, spotLightQuadraticFactor, time, false, isDirectionalLightOn, isSpotLightOn);
	simpleShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());


	const wchar_t* textureIDs[3] = { L"red", L"green", L"blue" };
	for (int i = 0; i < 3; i++)
	{
		if (pointLights[i]->on)
		{
			worldMatrix = renderer->getWorldMatrix();
			worldMatrix *= XMMatrixScaling(0.5f, 0.5f, 0.5f);
			worldMatrix *= XMMatrixTranslation(pointLights[i]->position.x, pointLights[i]->position.y, pointLights[i]->position.z);
			sphereMesh->sendData(renderer->getDeviceContext());
			simpleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(textureIDs[i]), directionalLight, pointLights, spotLight, spotLightConstantFactor, spotLightLinearFactor, spotLightQuadraticFactor, time, true, isDirectionalLightOn, isSpotLightOn);
			simpleShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
		}
	}

	// Room
	// bottom
	worldMatrix = renderer->getWorldMatrix();
	planeMesh->sendData(renderer->getDeviceContext());
	simpleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"concrete"), directionalLight, pointLights, spotLight, spotLightConstantFactor, spotLightLinearFactor, spotLightQuadraticFactor, time, false, isDirectionalLightOn, isSpotLightOn);
	simpleShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// top
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixRotationRollPitchYaw(PI, 0.0f, 0.0f);
	worldMatrix *= XMMatrixTranslation(0.0f, planeSize, planeSize);
	simpleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"concrete"), directionalLight, pointLights, spotLight, spotLightConstantFactor, spotLightLinearFactor, spotLightQuadraticFactor, time, false, isDirectionalLightOn, isSpotLightOn);
	simpleShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// left
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixRotationRollPitchYaw(-PI / 2, -PI / 2, 0.0f);
	simpleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"concrete"), directionalLight, pointLights, spotLight, spotLightConstantFactor, spotLightLinearFactor, spotLightQuadraticFactor, time, false, isDirectionalLightOn, isSpotLightOn);
	simpleShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// right
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixRotationRollPitchYaw(-PI / 2, PI / 2, 0.0f);
	worldMatrix *= XMMatrixTranslation(planeSize, 0.0f, planeSize);
	simpleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"concrete"), directionalLight, pointLights, spotLight, spotLightConstantFactor, spotLightLinearFactor, spotLightQuadraticFactor, time, false, isDirectionalLightOn, isSpotLightOn);
	simpleShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// front
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixRotationRollPitchYaw(-PI / 2, 0.0f, 0.0f);
	worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, planeSize);
	simpleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"concrete"), directionalLight, pointLights, spotLight, spotLightConstantFactor, spotLightLinearFactor, spotLightQuadraticFactor, time, false, isDirectionalLightOn, isSpotLightOn);
	simpleShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// back
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixRotationRollPitchYaw(PI / 2, 0.0f, 0.0f);
	worldMatrix *= XMMatrixTranslation(0.0f, planeSize, 0.0f);
	simpleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"concrete"), directionalLight, pointLights, spotLight, spotLightConstantFactor, spotLightLinearFactor, spotLightQuadraticFactor, time, false, isDirectionalLightOn, isSpotLightOn);
	simpleShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	portalMesh->sendData(renderer->getDeviceContext());
	simpleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, portalTexture->getShaderResourceView(), directionalLight, pointLights, spotLight, spotLightConstantFactor, spotLightLinearFactor, spotLightQuadraticFactor, time, false, isDirectionalLightOn, isSpotLightOn);
	simpleShader->render(renderer->getDeviceContext(), portalMesh->getIndexCount());

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();
}

bool App1::render()
{
	//portalPass();

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
	/*ImGui::SliderFloat("Point Light Constant", &pointLightConstantFactor, 0.0f, 0.5f);
	ImGui::SliderFloat("Point Light Linear", &pointLightLinearFactor, 0.0f, 0.5f);
	ImGui::SliderFloat("Point Light Quadratic", &pointLightQuadraticFactor, 0.0f, 0.5f);*/
	ImGui::SliderFloat("Red X", &pointLights[0]->position.x, 0, (float)planeSize);
	ImGui::SliderFloat("Red Y", &pointLights[0]->position.y, 0, (float)planeSize);
	ImGui::SliderFloat("Red Z", &pointLights[0]->position.z, 0, (float)planeSize);
	ImGui::SliderFloat("Green X", &pointLights[1]->position.x, 0, (float)planeSize);
	ImGui::SliderFloat("Green Y", &pointLights[1]->position.y, 0, (float)planeSize);
	ImGui::SliderFloat("Green Z", &pointLights[1]->position.z, 0, (float)planeSize);
	ImGui::SliderFloat("Blue X", &pointLights[2]->position.x, 0, (float)planeSize);
	ImGui::SliderFloat("Blue Y", &pointLights[2]->position.y, 0, (float)planeSize);
	ImGui::SliderFloat("Blue Z", &pointLights[2]->position.z, 0, (float)planeSize);
	ImGui::SliderFloat("Spot Light Constant", &spotLightConstantFactor, 0.0f, 0.5f);
	ImGui::SliderFloat("Spot Light Linear", &spotLightLinearFactor, 0.0f, 0.5f);
	ImGui::SliderFloat("Spot Light Quadratic", &spotLightQuadraticFactor, 0.0f, 0.5f);
	ImGui::Checkbox("Directional Light", &isDirectionalLightOn);
	ImGui::Checkbox("Spot Light", &isSpotLightOn);
	ImGui::Checkbox("Red Point Light", &pointLights[0]->on);
	ImGui::Checkbox("Green Point Light", &pointLights[1]->on);
	ImGui::Checkbox("Blue Point Light", &pointLights[2]->on);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

