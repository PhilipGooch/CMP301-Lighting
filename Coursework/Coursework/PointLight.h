#pragma once

#include "DXF.h"

class PointLight 
{
public:
	PointLight(TextureManager* textureMgr, XMFLOAT3 position, XMFLOAT4 diffuse, XMFLOAT4 ambient);
	~PointLight();

public:
	void setPosition();

public:
	XMFLOAT3 position;
	Light* light;

	float constant;
	float linear;
	float quadratic;

	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;

	bool on;

};

