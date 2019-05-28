/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

//We have a clip plane 4 float vector so that outside objects can set where the clip plane is located and which side of the plane will be clipped. 
//The first three floats specify the location of the clip plane, the fourth float indicates the position. 
//For example if it holds (0.0f, 1.0f, 0.0f, -5.0f) then it means that it will clip everything beneath the Y axis and this clipping will start at the height of 5.0. 
//So any pixel in the scene that is below 5.0 on the Y-axis will never be drawn. 
cbuffer ClipPlaneBuffer
{
    float4 clipPlane;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float clip : SV_ClipDistance0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ClipPlaneVertexShader(VertexInputType input)
{
    PixelInputType output;
    

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
	
    // Set the clipping plane.
    output.clip = dot(mul(input.position, worldMatrix), clipPlane);

    return output;
}