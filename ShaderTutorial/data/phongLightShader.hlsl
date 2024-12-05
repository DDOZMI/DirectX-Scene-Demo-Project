// GLOBALS //
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    float time;
    float modelID;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

cbuffer PointPosBuffer
{
    float4 pointPos;
};

cbuffer PointColorBuffer
{
    float4 pointCol;
};

Texture2D shaderTexture;
SamplerState SampleType;

// TYPEDEFS //
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    uint instanceID : SV_InstanceID;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

// Vertex Shader
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;
    float4x4 rotationMatrix;
    float rotationAngle;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    /*if (modelID < 3.0f)  // 자동차 모델
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
        
        // 노말에도 회전 적용
        float3 rotatedNormal = mul(input.normal, (float3x3) rotationMatrix);
        output.normal = mul(rotatedNormal, (float3x3) worldMatrix);
    }*/
    if (modelID == 9.0f || modelID == 10.0f) // 깃발 오브젝트 인스턴스
    {
        // 각 인스턴스의 Z 오프셋 계산
        float zOffset = 3.0f * input.instanceID;
        
        // 회전 각도 설정
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

        // 먼저 오브젝트의 로컬 위치에 Z 오프셋 적용
        float4 offsetPosition = input.position;
        offsetPosition.z += zOffset;
        
        // 오프셋 위치를 원점으로 되돌림
        float4 centeredPosition = offsetPosition;
        centeredPosition.z -= zOffset;
        
        // 회전 적용
        float4 rotatedPosition = mul(centeredPosition, rotationMatrix);
        
        // 다시 오프셋 위치로 이동
        rotatedPosition.z += zOffset;
        
        // 월드 변환 적용
        output.position = mul(rotatedPosition, worldMatrix);
        
        // 노말에도 회전 적용
        float3 rotatedNormal = mul(input.normal, (float3x3) rotationMatrix);
        output.normal = mul(rotatedNormal, (float3x3) worldMatrix);
    }
    else // 바닥 모델
    {
        // 회전 없이 적용
        input.position.w = 1.0f;
        output.position = mul(input.position, worldMatrix);
        
        output.normal = mul(input.normal, (float3x3) worldMatrix);
    }
    
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);

    return output;
}

// Pixel Shader
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Set the default output color to the ambient light value for all pixels.
    color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    color = ambientColor;

	// Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);

	    // Saturate the ambient and diffuse color.
        color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * input.normal - lightDir);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }

    // Multiply the texture pixel and the input color to get the textured result.
    color = color * textureColor;

	// Add the specular component last to the output color.
    color = saturate(color + specular);

    return color;
}