// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ManipulationBuffer : register(b0)
{
	float time;
	float constantFactor;
	float linearFactor;
	float quadraticFactor;
	
	float isLight;
	float isDirectionalLightOn;
	float isSpotLightOn;
	float isRedPointLightOn;
	float isGreenPointLightOn;
	float isBluePointLightOn;
	float2 padding0;
};

cbuffer DirectionalLightBuffer : register(b1)
{
	float4 directionalLightAmbientColour;
	float4 directionalLightDiffuseColour;
	float3 directionalLightDirection;
	float padding1;
};

cbuffer PointLightBuffer : register(b2)
{
	float4 pointLightAmbientColours[3];
	float4 pointLightDiffuseColours[3];
	float4 pointLightPositions[3];
};

cbuffer SpotLightBuffer : register(b3)
{
	float4 spotLightAmbientColour;
	float4 spotLightDiffuseColour;
	float4 spotLightPosition;
	float4 spotLightDirection;
};

struct InputType
{
	float4 screenPosition : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPosition : POSITION;
};

float map(float value, float istart, float istop, float ostart, float ostop) {
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

float4 calculateLighting(float3 inverseLightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, inverseLightDirection));
	//intensity = intensity / 2;;
	//intensity += 0.5f;
	intensity = map(intensity, 0, 1, 1, 1);
	float4 colour = saturate(diffuse * intensity);				
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	// Texture colour
	float4 textureColour;

	textureColour = texture0.Sample(sampler0, input.tex);

	return textureColour;

	// If light indicator geometry, just return texture colour.
	if (isLight == float(1.0f))
	{
		return textureColour;
	}
	
	float4 lightColour = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Point lights
	float4 pointLightColour = { 0.0f, 0.0f, 0.0f, 0.0f };
	float3 pointLightVectors[3];
	float pointLightDistances[3];

	bool pointLightsOn[3];
	
	if (isRedPointLightOn) 
		pointLightsOn[0] = true; 
	else
		pointLightsOn[0] = false;

	if (isGreenPointLightOn)
		pointLightsOn[1] = true;
	else
		pointLightsOn[1] = false;

	if (isBluePointLightOn)
		pointLightsOn[2] = true;
	else
		pointLightsOn[2] = false;

	for (int i = 0; i < 3; i++)
	{
		if (pointLightsOn[i])
		{
			pointLightVectors[i] = normalize(pointLightPositions[i] - input.worldPosition);

			pointLightDistances[i] = length(pointLightPositions[i] - input.worldPosition);

			float attenuation = 1 / (constantFactor + (linearFactor * pointLightDistances[i]) + (quadraticFactor * pow(pointLightDistances[i], 2)));

			pointLightColour += calculateLighting(pointLightVectors[i], input.normal, pointLightDiffuseColours[i]) * attenuation + pointLightAmbientColours[i];

			lightColour += pointLightColour;
		}
	}

	

	float4 directionalLightColour = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Directional light
	if (isDirectionalLightOn == float(1.0f))
	{
		float3 manipulatedDirectionalLightDirection = directionalLightDirection;

		manipulatedDirectionalLightDirection.x = sin(time);
		manipulatedDirectionalLightDirection.z = cos(time);

		directionalLightColour = calculateLighting(-manipulatedDirectionalLightDirection, input.normal, directionalLightDiffuseColour) + directionalLightAmbientColour;

		lightColour += directionalLightColour;
	}

	// Spot light
	if (isSpotLightOn == float(1.0f))
	{
		float4 spotLightColour = { 0.0f, 0.0f, 0.0f, 1.0f };
		float3 spotLightVector;
		float3 inverseSpotLightVector;
		float spotLightDistance;
		float cosA;
		float cosB;
		float halfConeAngle = 3.1415926 / 12;

		spotLightVector = normalize(spotLightPosition - input.worldPosition);
		inverseSpotLightVector = -spotLightVector;
		spotLightDistance = length(spotLightPosition - input.worldPosition);
		cosA = dot(spotLightDirection, inverseSpotLightVector);
		cosB = cos(halfConeAngle);
		//if (cosA > cosB)
		{
			float attenuation = 1 / (constantFactor + (linearFactor * spotLightDistance) + (quadraticFactor * pow(spotLightDistance, 2)));
			spotLightColour = calculateLighting(spotLightVector, input.normal, spotLightDiffuseColour);
			spotLightColour *= attenuation; 
			spotLightColour *= pow(max(cosA, 0), 16);
		}
		lightColour += spotLightColour;
	}
	 

	return textureColour * saturate(lightColour);
}



