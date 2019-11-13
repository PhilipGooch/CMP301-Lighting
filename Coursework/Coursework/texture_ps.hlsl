
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

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

	clip(textureColour.a - 1.0f);			// what is this doing?? discarding the pixel, but under what condition?


	return textureColour;
	
}



