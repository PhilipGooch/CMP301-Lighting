// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "SimpleShader.h"
#include "MyPlaneMesh.h"
#include "TextureShader.h"
#include "StencilShader.h"
#include "Portal.h"
#include "World.h"

#define PI 3.1415962f

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool finalPass();
	bool render();
	void gui();

private:
	TextureShader* textureShader;
	StencilShader* stencilShader;

	World* world;

	Portal* portal;
	Portal* portal2;

	OrthoMesh* orthoMesh;

	QuadMesh* quadMesh;

	int planeSize;

};

#endif