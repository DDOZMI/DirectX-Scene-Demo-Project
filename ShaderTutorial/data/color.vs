////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
// These globals can be modified externally from the framework(C++) code.
// Use cbuffer for most globals. 
// Logically organizing these buffers is important for efficient execution of 
// shaders as well as how the graphics card will store the buffers. 
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
    float time; // 매 순간 회전하게 하기 위해 설정한 시간 값
};

//회전 계산을 위한 행렬들 (각각 x, y, z축)
matrix RotateX(float angle)
{
    return matrix(
        1, 0, 0, 0,
        0, cos(angle), sin(angle), 0,
        0, -sin(angle), cos(angle), 0,
        0, 0, 0, 1
    );
}

matrix RotateY(float angle)
{
    return matrix(
        cos(angle), 0, -sin(angle), 0,
        0, 1, 0, 0,
        sin(angle), 0, cos(angle), 0,
        0, 0, 0, 1
    );
}

matrix RotateZ(float angle)
{
    return matrix(
        cos(angle), sin(angle), 0, 0,
        -sin(angle), cos(angle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

//////////////
// TYPEDEFS //
// Use different types such as float4 that are available to HLSL which make 
// programming shaders easier and readable. 
// The POSITION, COLOR, and SV_POSITION are semantics that convey to the GPU 
// the use of the variable. 
// e.g. POSITION: vertex shader, SV_POSITION: pixel shader, COLOR: both. 
// e.g. Same type: COLOR0, COLOR1, ...
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
// The vertex shader is called by the GPU when it is processing data from the 
// vertex buffers that have been sent to it. This vertex shader will be called 
// for every single vertex in the vertex buffer. 
// The input to the vertex shader must match the data format in the vertex buffer 
// as well as the type definition in the shader source file which in this case is 
// VertexInputType. 
// The output of the vertex shader will be sent to the pixel shader. 
////////////////////////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // 회전각 계산
    float angle = time * 1.0f; // 매 순간마다 얼만큼 돌아가는지가 각도니까 이러함.

    // x좌표 범위로 회전 적용 구분
    if (input.position.x >= -5.0f && input.position.x <= -3.0f)
    {
        // x축 회전
        matrix rotationMatrix = RotateX(angle);
        input.position = mul(input.position, rotationMatrix);
    }
    else if (input.position.x >= -1.0f && input.position.x <= 0.5f)
    {
        // y축 회전
        matrix rotationMatrix = RotateY(angle);
        input.position = mul(input.position, rotationMatrix);
    }
    else if (input.position.x >= 3.0f && input.position.x <= 4.75f)
    {
        // z축 회전
        matrix rotationMatrix = RotateZ(angle);
        input.position = mul(input.position, rotationMatrix);
    }

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the input color for the pixel shader to use.
    output.color = input.color;
    
    return output;
}