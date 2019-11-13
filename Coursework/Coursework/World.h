#pragma once
#include "DXF.h"
#include "TextureShader.h"
#include "MyPlaneMesh.h"

#define PI 3.1415962f

class World
{
public:
	World(D3D* renderer, TextureShader* textureShader, TextureManager* textureManager, float planeSize);
	~World();

	void cubes(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	void room(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:
	D3D* renderer;
	TextureShader* textureShader;
	TextureManager* textureManager;
	float planeSize;
	CubeMesh* cubeMesh;
	MyPlaneMesh* planeMesh;
};

