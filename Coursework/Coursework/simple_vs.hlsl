// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct InputType
{
	float4 localPosition : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 screenPosition : SV_POSITION;		// why SV_POSITION instead of POSITION?
	float2 tex : TEXCOORD0;						// TEXCOORD is a float4 and we are declaring it as a float2. is this ok?
	float3 normal : NORMAL;						// NORMAL is also a float4...
	float4 worldPosition : POSITION;			// why is it TEXCOORD in the slides?
};

OutputType main(InputType input)
{
	OutputType output;

	output.worldPosition = mul(input.localPosition, worldMatrix);

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.screenPosition = mul(input.localPosition, worldMatrix);
	output.screenPosition = mul(output.screenPosition, viewMatrix);
	output.screenPosition = mul(output.screenPosition, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = 1 - input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}