#include "PointLight.h"

PointLight::PointLight(TextureManager* textureMgr, XMFLOAT3 position, XMFLOAT4 diffuse, XMFLOAT4 ambient) :
	position(position),
	diffuse(diffuse),
	ambient(ambient),
	constant(0.412f),
	linear(0.108f),
	quadratic(0.004f),
	on(false)
{
	light = new Light;
	light->setDiffuseColour(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
	light->setAmbientColour(ambient.x, ambient.y, ambient.z, ambient.w);
	light->setPosition(position.x, position.y, position.z);
}

PointLight::~PointLight()
{
}

void PointLight::setPosition()
{
	//light->setPosition(position.x, position.y, position.z);
}