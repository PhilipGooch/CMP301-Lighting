// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "SimpleShader.h"
#include "MyPlaneMesh.h"
#include "PointLight.h"

#define PI 3.1415962f

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

	void scene();

protected:
	void finalPass();
	bool render();
	void gui();

private:
	LightShader* simpleShader;

	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;
	MyPlaneMesh* planeMesh;

	int planeSize;

	Light* directionalLight;
	bool isDirectionalLightOn;
	XMMATRIX rotationMatrix;

	Light* spotLight;
	XMFLOAT4 spotLightAmbientColour;
	XMFLOAT4 spotLightDiffuseColour;
	XMFLOAT3 spotLightDirection;
	bool isSpotLightOn;
	float spotLightConstantFactor;
	float spotLightLinearFactor;
	float spotLightQuadraticFactor;


	float time;

	PointLight* pointLights[3];



	RenderTexture* portalTexture;

	QuadMesh* portalMesh;

	Camera* portalCamera;


};

#endif