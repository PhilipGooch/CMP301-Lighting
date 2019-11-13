#pragma once
#include "DXF.h"
#include "TextureShader.h"
#include "StencilShader.h"
#include "World.h"

class Portal
{
public:
	Portal(HWND hwnd, D3D* renderer, Input* input, int screenWidth, int screenHeight, XMFLOAT3 position, 
		   TextureShader* textureShader, StencilShader* stencilShader, TextureManager* textureManager);
	~Portal();

	void updateCamera(XMFLOAT3 playerPosition, XMFLOAT3 playerLookAt);

	void stencilPass(FPCamera* playerCamera);

	void cameraPass(World* world);

	void combinedPass();

//private:	make private...!
public:
	XMFLOAT3 position;
	//char axis;
	//Portal* otherPortal;
	//char ID;

	FPCamera* camera;

	RenderTexture* stencilTexture;
	RenderTexture* cameraTexture;
	RenderTexture* combinedTexture;

	QuadMesh* stencilQuadMesh;
	OrthoMesh* orthoMesh;

	D3D* renderer;
	TextureShader* textureShader;
	StencilShader* stencilShader;
	TextureManager* textureManager;
};

