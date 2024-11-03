////////////////////////////////////////////////////////////////////////////////
// Filename: texture.vs
// The texture vertex shader is similar to the previous color shader except that 
// there have been some changes to accommodate texturing.
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
    float time;
    float modelID;
};


//////////////
// TYPEDEFS //
// We are no longer using color in our vertex type and have instead moved to using 
// texture coordinates. Since texture coordinates take a U and V float coordinate, 
// we use float2 as its type. The semantic for texture coordinates is TEXCOORD0 for 
// vertex shaders and pixel shaders. 
// You can change the zero to any number to indicate which set of coordinates you 
// are working with as multiple texture coordinates are allowed.
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
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4x4 rotationMatrix;
    float rotationAngle;
    
    if(modelID < 3.0f)  // 자동차 모델
    {
       rotationAngle = time * 1.0f;
        
        // Y축 회전 행렬
        float cosAngle = cos(rotationAngle);
        float sinAngle = sin(rotationAngle);
        
        rotationMatrix = float4x4(
            cosAngle, 0.0f, -sinAngle, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            sinAngle, 0.0f, cosAngle, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        
        // 회전 적용
        input.position.w = 1.0f;
        float4 rotatedPosition = mul(input.position, rotationMatrix);
        output.position = mul(rotatedPosition, worldMatrix);
    }
    else  // 바닥 모델
    {
        // 회전 없이 적용
        input.position.w = 1.0f;
        output.position = mul(input.position, worldMatrix);
    }

    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.tex = input.tex;
    
    return output;
}