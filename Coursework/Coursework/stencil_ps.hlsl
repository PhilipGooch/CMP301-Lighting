
Texture2D texture0 : register(t0);
Texture2D stencil0 : register(t1);
SamplerState sampler0 : register(s0);
SamplerState sampler1 : register(s1);

struct InputType
{
	float4 screenPosition : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	// Texture colour
	float4 textureColour;

	textureColour = texture0.Sample(sampler0, input.tex);

	float4 stencilColour;

	stencilColour = stencil0.Sample(sampler1, input.tex);

	if (stencilColour.r == 0.0f && stencilColour.g == 1.0f && stencilColour.b == 0.0f)
	{
		return textureColour;
	}
	else
	{
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	

}



