////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_ColorShader = 0;
	m_TextureShader = 0;
	for (int i = 0; i < 4; i++)
	{
		m_Model[i] = 0;
	}

	// 기본값 검은색
	m_bgColor[0] = 0.0f;
	m_bgColor[1] = 0.0f;
	m_bgColor[2] = 0.0f;
	m_bgColor[3] = 1.0f;

	m_brightness = 1.0f;

	m_wireframeMode = false;
	m_backFaceCulling = true;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}



bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 1.0f, -6.0f);

	for (int i = 0; i < 4; i++)
	{
		// Create the model object.
		m_Model[i] = new ModelClass;
		if (!m_Model)
		{
			return false;
		}

		// Initialize the model object.
		if (i == 0)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/jeep.obj", L"./data/jeep.dds");
		}
		else if (i == 1)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/nissan.obj", L"./data/nissan.dds");
		}
		else if (i == 2)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/ford.obj", L"./data/ford.dds");
		}
		else
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/land.obj", L"./data/land.dds");
		}

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if(!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	m_Input->Initialize();

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the color shader object.
	if(m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	for (int i = 0; i < 4; i++)
	{
		if (m_Model[i])
		{
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


void GraphicsClass::SetBackgroundColor(float r, float g, float b)
{
	m_bgColor[0] = r;
	m_bgColor[1] = g;
	m_bgColor[2] = b;
}


void GraphicsClass::SetBrightness(float brightness)
{
	m_brightness = brightness;
}

void GraphicsClass::SetRenderMode(bool wireframe)
{
	m_wireframeMode = wireframe;

	if (m_D3D)
	{
		m_D3D->SetRenderMode(wireframe);
	}
}

void GraphicsClass::SetBackFaceCulling(bool enable)
{
	m_backFaceCulling = enable;
	if (m_D3D)
	{
		m_D3D->SetBackFaceCulling(enable);
	}
}

void GraphicsClass::SetTextureFilter(int filterMode)
{
	m_TextureShader->SetTextureFilter(m_D3D->GetDevice(), filterMode);
}


bool GraphicsClass::Render()
{
	XMMATRIX viewMatrix, projectionMatrix;
	XMMATRIX worldMatrix[4];
	bool result;

	static float time = 0.0f;
	time += 0.01f;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	for (int i = 0; i < 4; i++)
	{
		m_D3D->GetWorldMatrix(worldMatrix[i]);
	}
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// 
	m_D3D->SetRenderMode(m_wireframeMode);

	//
	m_D3D->SetBackFaceCulling(m_backFaceCulling);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix[0] = XMMatrixTranslation(-2.2f, 0.0f, 0.0f);
	worldMatrix[1] = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	worldMatrix[2] = XMMatrixTranslation(2.2f, 0.0f, 0.0f);
	worldMatrix[3] = XMMatrixScaling(5.0f, -1.0f, 10.0f) * XMMatrixTranslation(0.0f, 0.0f, -6.0f);

	for (int i = 0; i < 4; i++)
	{
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Model[i]->Render(m_D3D->GetDeviceContext());

		// Render the model using the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(),
			worldMatrix[i], viewMatrix, projectionMatrix, m_Model[i]->GetTexture(), time, static_cast<float>(i));
		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

