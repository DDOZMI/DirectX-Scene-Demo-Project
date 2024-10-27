////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 23;

	// Set the number of indices in the index array.
	m_indexCount =42;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	vertices[0].position = XMFLOAT3(-4.0f, 1.0f, 0.0f);
	vertices[0].color = XMFLOAT4(0.0f, 0.6f, 1.0f, 1.0f);

	vertices[1].position = XMFLOAT3(-5.0f, 0.5f, 0.0f);
	vertices[1].color = XMFLOAT4(0.0f, 0.6f, 1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(-5.0f, -0.5f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 0.6f, 1.0f, 1.0f);

	vertices[3].position = XMFLOAT3(-4.0f, -1.0f, 0.0f);
	vertices[3].color = XMFLOAT4(0.0f, 0.6f, 1.0f, 1.0f);

	vertices[4].position = XMFLOAT3(-3.0f, -0.5f, 0.0f);
	vertices[4].color = XMFLOAT4(0.0f, 0.6f, 1.0f, 1.0f);

	vertices[5].position = XMFLOAT3(-3.0f, 0.5f, 0.0f);
	vertices[5].color = XMFLOAT4(0.0f, 0.6f, 1.0f, 1.0f);

	//////////////////////////////////////////////////////

	vertices[6].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertices[6].color = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);

	vertices[7].position = XMFLOAT3(0.5f, 1.0f, 0.0f);
	vertices[7].color = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);

	vertices[8].position = XMFLOAT3(0.0f, 0.5f, 0.0f);
	vertices[8].color = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);

	vertices[9].position = XMFLOAT3(-0.5f, 0.0f, 0.0f);
	vertices[9].color = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);

	vertices[10].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertices[10].color = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);

	vertices[11].position = XMFLOAT3(0.0f, -0.5f, 0.0f);
	vertices[11].color = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);

	vertices[12].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[12].color = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);

	vertices[13].position = XMFLOAT3(0.5f, -1.0f, 0.0f);
	vertices[13].color = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);

	///////////////////////////////////////////////////////

	vertices[14].position = XMFLOAT3(3.0f, 1.0f, 0.0f);
	vertices[14].color = XMFLOAT4(1.0f, 0.5f, 0.3f, 1.0f);

	vertices[15].position = XMFLOAT3(4.25f, 1.0f, 0.0f);
	vertices[15].color = XMFLOAT4(1.0f, 0.5f, 0.3f, 1.0f);

	vertices[16].position = XMFLOAT3(3.5f, 0.5f, 0.0f);
	vertices[16].color = XMFLOAT4(1.0f, 0.5f, 0.3f, 1.0f);

	vertices[17].position = XMFLOAT3(4.5f, 0.5f, 0.0f);
	vertices[17].color = XMFLOAT4(1.0f, 0.5f, 0.3f, 1.0f);

	vertices[18].position = XMFLOAT3(4.0f, 0.5f, 0.0f);
	vertices[18].color = XMFLOAT4(1.0f, 0.5f, 0.3f, 1.0f);

	vertices[19].position = XMFLOAT3(4.0f, -0.5f, 0.0f);
	vertices[19].color = XMFLOAT4(1.0f, 0.5f, 0.3f, 1.0f);

	vertices[20].position = XMFLOAT3(3.0f, -1.0f, 0.0f);
	vertices[20].color = XMFLOAT4(1.0f, 0.5f, 0.3f, 1.0f);

	vertices[21].position = XMFLOAT3(4.75f, -0.5f, 0.0f);
	vertices[21].color = XMFLOAT4(1.0f, 0.5f, 0.3f, 1.0f);

	vertices[22].position = XMFLOAT3(4.25f, -1.0f, 0.0f);
	vertices[22].color = XMFLOAT4(1.0f, 0.5f, 0.3f, 1.0f);


	indices[0] = 1;
	indices[1] = 0;
	indices[2] = 5;

	indices[3] = 1;
	indices[4] = 5;
	indices[5] = 3;

	indices[6] = 1;
	indices[7] = 3;
	indices[8] = 2;

	indices[9] = 3;
	indices[10] = 5;
	indices[11] = 4;
	/////////////////////////
	indices[12] = 6;
	indices[13] = 7;
	indices[14] = 8;

	indices[15] = 9;
	indices[16] = 7;
	indices[17] = 10;

	indices[18] = 9;
	indices[19] = 10;
	indices[20] = 13;

	indices[21] = 12;
	indices[22] = 11;
	indices[23] = 13;
	//////////////////////////
	indices[24] = 14;
	indices[25] = 15;
	indices[26] = 16;

	indices[27] = 16;
	indices[28] = 15;
	indices[29] = 17;

	indices[30] = 19;
	indices[31] = 18;
	indices[32] = 17;

	indices[33] = 20;
	indices[34] = 18;
	indices[35] = 19;

	indices[36] = 20;
	indices[37] = 19;
	indices[38] = 22;

	indices[39] = 19;
	indices[40] = 21;
	indices[41] = 22;


	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

// This sets the vertex buffer and index buffer as active on the input assembler in the GPU. 
// Once the GPU has an active vertex buffer, it can then use the shader to render that buffer. 
// This function also defines how those buffers should be drawn such as triangles, lines, fans, 
// and etc using the IASetPrimitiveTopology DirectX function.
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}