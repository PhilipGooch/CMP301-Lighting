// Sphere Mesh
// Generated sphere mesh with texture coordinates and normals.
// Uses the cube sphere normalisation method. First a cube is generated,
// then the vertices are normalised creating a sphere.
// Resolution specifies the number of segments in the sphere (top and bottom, matches equator).

#pragma once

#include "BaseMesh.h"

using namespace DirectX;

class MySphereMesh : public BaseMesh
{

public:
	MySphereMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float size = 1.0f, int resolution = 20);
	~MySphereMesh();

protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
	float size;
};