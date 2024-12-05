////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
	m_LightShader = nullptr;
	m_Light = nullptr;
	for (int i = 0; i < 12; i++) { m_Model[i] = nullptr; }
	m_Input = nullptr;
	m_Bitmap = nullptr;
	m_Font = nullptr;
	m_Cpu = nullptr;
	m_Fps = nullptr;

	// 기본값 검은색 - 배경 사용으로 가려져서 의미없음
	m_bgColor[0] = 0.0f;
	m_bgColor[1] = 0.0f;
	m_bgColor[2] = 0.0f;
	m_bgColor[3] = 1.0f;

	// 폴리곤 색상 밝기도 더이상 안씀
	m_brightness = 1.0f;
	m_lightBrightness = 1.0f;

	m_lightingMode = 0;
	m_totalPolygons = 0;

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

	/*vector<wstring> groundTextures;
	groundTextures.push_back(L"")*/

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// 카메라 오브젝트 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	// 카메라 포지션 설정
	m_Camera->SetPosition(0.0f, 4.5f, -15.0f);

	// 모델 오브젝트를 담은 배열 생성
	for (int i = 0; i < 12; i++)
	{
		// 각각의 모델 오브젝트 생성
		m_Model[i] = new ModelClass;
		if (!m_Model)
		{
			return false;
		}

		// 모델 오브젝트 초기화
		if (i == 0)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/por.obj", L"./data/por.dds");
		}
		else if (i == 1)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/SLR.obj", L"./data/SLR.dds");
		}
		else if (i == 2)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/aventador.obj", L"./data/aventador.dds");
		}
		else if (i == 3)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/mustang.obj", L"./data/mustang.dds");
		}
		else if (i == 4)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/tesla.obj", L"./data/tesla.dds");
		}
		else if (i == 5)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/E46.obj", L"./data/E46.dds");
		}
		else if (i == 6)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/honda.obj", L"./data/honda.dds");
		}
		else if (i == 7)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/finish.obj", L"./data/finish.dds");
		}
		else if (i == 8)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/light.obj", L"./data/light.dds");
		}
		else if (i == 9 || i == 10)
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/flag.obj", L"./data/flag.dds");
		}
		else
		{
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/track.obj", L"./data/track.dds");
		}

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		if (i == 3)
		{
			m_totalPolygons += static_cast<unsigned int>((m_Model[i]->GetIndexCount() / 3) * 10); // 3번 모델만 10개 인스턴싱
		}
		else
		{
			m_totalPolygons += static_cast<unsigned int>(m_Model[i]->GetIndexCount()) / 3;
		}
	}


	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/sky.dds", screenWidth, screenHeight - 220);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// 라이트 쉐이더 오브젝트 생성
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// 라이트 쉐이더 오브젝트 초기화
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// 라이트 오브젝트 생성
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// 라이트 오브젝트 초기화 (라이팅 설정값)
	m_Light->SetAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 1.0f, -1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(16.0f);

	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}
	result = m_Font->Initialize(m_D3D->GetDevice(), m_D3D, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}
	m_Fps->Initialize();

	// Create and initialize the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}
	m_Cpu->Initialize();

	return true;
}


void GraphicsClass::Shutdown()
{
	// 모델 오브젝트 해제
	for (int i = 0; i < 12; i++)
	{
		if (m_Model[i])
		{
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = nullptr;
		}
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}

	// 비트맵 오브젝트 해제
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = nullptr;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = nullptr;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
	}

	// Release the input object.
	if (m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}

	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = nullptr;
	}

	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = nullptr;
	}

	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = nullptr;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	m_Fps->Frame();
	m_Cpu->Frame();

	// Render the graphics scene.
	result = Render();
	if (!result)
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
	m_LightShader->SetTextureFilter(m_D3D->GetDevice(), filterMode);
}

void GraphicsClass::SetLightingMode(int mode)
{
	static XMFLOAT4 currentAmbient;
	static XMFLOAT4 currentDiffuse;
	static XMFLOAT4 currentSpecular;
	static float originalSpecularPower;
	static bool isFirstTime = true;

	if (isFirstTime)
	{
		currentAmbient = m_Light->GetAmbientColor();
		currentDiffuse = m_Light->GetDiffuseColor();
		currentSpecular = m_Light->GetSpecularColor();
		originalSpecularPower = m_Light->GetSpecularPower();
		isFirstTime = false;
	}

	if (mode == m_lightingMode)
	{
		m_Light->SetAmbientColor(currentAmbient.x, currentAmbient.y, currentAmbient.z, currentAmbient.w);
		m_Light->SetDiffuseColor(currentDiffuse.x, currentDiffuse.y, currentDiffuse.z, currentDiffuse.w);
		m_Light->SetSpecularColor(currentSpecular.x, currentSpecular.y, currentSpecular.z, currentSpecular.w);
		m_Light->SetSpecularPower(originalSpecularPower);
		m_lightingMode = 0;
		return;
	}

	switch (mode)
	{
	case 1:
		m_Light->SetAmbientColor(currentAmbient.x, currentAmbient.y, currentAmbient.z, currentAmbient.w);
		m_Light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light->SetSpecularColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light->SetSpecularPower(1000000.0f);
		m_lightingMode = 1;
		break;

	case 2:
		m_Light->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light->SetDiffuseColor(currentDiffuse.x, currentDiffuse.y, currentDiffuse.z, currentDiffuse.w);
		m_Light->SetSpecularColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light->SetSpecularPower(1000000.0f);
		m_lightingMode = 2;
		break;

	case 3:
		m_Light->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light->SetSpecularColor(currentSpecular.x, currentSpecular.y, currentSpecular.z, currentSpecular.w);
		m_Light->SetSpecularPower(originalSpecularPower);
		m_lightingMode = 3;
		break;
	}
}

void GraphicsClass::SetLightColor(float red, float green, float blue)
{
	m_Light->SetDiffuseColor(red, green, blue, 1.0f);
}

void GraphicsClass::SetLightBrightness(bool isPlus)
{
	XMFLOAT4 currentAmbient = m_Light->GetAmbientColor();
	XMFLOAT4 currentDiffuse = m_Light->GetDiffuseColor();
	XMFLOAT4 currentSpecular = m_Light->GetSpecularColor();
	float currentSpecularPow = m_Light->GetSpecularPower();

	if (isPlus)
	{
		m_Light->SetAmbientColor(
			currentAmbient.x + 0.01f,
			currentAmbient.y + 0.01f,
			currentAmbient.z + 0.01f,
			1.0f
		);
		m_Light->SetDiffuseColor(
			currentDiffuse.x + 0.01f,
			currentDiffuse.y + 0.01f,
			currentDiffuse.z + 0.01f,
			1.0f
		);
		m_Light->SetSpecularColor(
			currentSpecular.x + 0.01f,
			currentSpecular.y + 0.01f,
			currentSpecular.z + 0.01f,
			1.0f
		);
		if (currentSpecularPow >= 8.0f)
			m_Light->SetSpecularPower(currentSpecularPow - 1.0f);

		if (currentAmbient.x > 1.0f)
			currentAmbient.x = 1.0f;
		else if (currentAmbient.y > 1.0f)
			currentAmbient.y = 1.0f;
		else if (currentAmbient.z > 1.0f)
			currentAmbient.z = 1.0f;
		if (currentDiffuse.x > 1.0f)
			currentDiffuse.x = 1.0f;
		else if (currentDiffuse.y > 1.0f)
			currentDiffuse.y = 1.0f;
		else if (currentDiffuse.z > 1.0f)
			currentDiffuse.z = 1.0f;
		if (currentSpecular.x > 1.0f)
			currentSpecular.x = 1.0f;
		else if (currentSpecular.y > 1.0f)
			currentSpecular.y = 1.0f;
		else if (currentSpecular.z > 1.0f)
			currentSpecular.z = 1.0f;
	}
	else
	{
		m_Light->SetAmbientColor(
			currentAmbient.x - 0.01f,
			currentAmbient.y - 0.01f,
			currentAmbient.z - 0.01f,
			1.0f
		);
		m_Light->SetDiffuseColor(
			currentDiffuse.x - 0.01f,
			currentDiffuse.y - 0.01f,
			currentDiffuse.z - 0.01f,
			1.0f
		);
		m_Light->SetSpecularColor(
			currentSpecular.x - 0.01f,
			currentSpecular.y - 0.01f,
			currentSpecular.z - 0.01f,
			1.0f
		);
		if (currentSpecularPow <= 32.0f)
			m_Light->SetSpecularPower(currentSpecularPow + 1.0f);

		if (currentAmbient.x < 0.0f)
			currentAmbient.x = 0.0f;
		else if (currentAmbient.y < 0.0f)
			currentAmbient.y = 0.0f;
		else if (currentAmbient.z < 0.0f)
			currentAmbient.z = 0.0f;
		if (currentDiffuse.x < 0.0f)
			currentDiffuse.x = 0.0f;
		else if (currentDiffuse.y < 0.0f)
			currentDiffuse.y = 0.0f;
		else if (currentDiffuse.z < 0.0f)
			currentDiffuse.z = 0.0f;
		if (currentSpecular.x < 0.0f)
			currentSpecular.x = 0.0f;
		else if (currentSpecular.y < 0.0f)
			currentSpecular.y = 0.0f;
		else if (currentSpecular.z < 0.0f)
			currentSpecular.z = 0.0f;
	}
}

CameraClass* GraphicsClass::GetCamera() 
{ 
	return m_Camera; 
}

bool GraphicsClass::Render()
{
	XMMATRIX viewMatrix, projectionMatrix, orthoMatrix;
	XMMATRIX worldMatrix[13];
	bool result;
	static float time = 0.0f;
	time += 0.01f;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	for (int i = 0; i < 13; i++) { m_D3D->GetWorldMatrix(worldMatrix[i]); }
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// solid - wireframe 변경기능 추가부
	m_D3D->SetRenderMode(m_wireframeMode);

	// 가려진 뒷면까지 렌더링 반영 변경기능 추가부
	m_D3D->SetBackFaceCulling(m_backFaceCulling);

	// 각각의 월드 매트릭스 변형, 위치조정
	worldMatrix[0] = XMMatrixTranslation(-1.1f, 0.0f, -2.4f); // 벤츠-맥라렌 SLR
	worldMatrix[1] = XMMatrixTranslation(1.13f, 0.0f, -0.7f); // 람보르기니 아벤타도르
	worldMatrix[2] = XMMatrixTranslation(1.13f, 0.0f, -4.4f); // 포르쉐 911
	worldMatrix[3] = XMMatrixTranslation(-1.1f, 0.0f, -6.0f); // 포드 머스탱
	worldMatrix[4] = XMMatrixTranslation(1.13f, 0.0f, -8.0f); // 테슬라 모델S
	worldMatrix[5] = XMMatrixTranslation(-1.1f, 0.0f, -9.7f); // BMW E46
	worldMatrix[6] = XMMatrixTranslation(1.13f, 0.0f, -11.7f); // 혼다 NSX
	worldMatrix[7] = XMMatrixTranslation(0.0f, 0.0f, 1.15f); // 피니시 구조물
	worldMatrix[8] = XMMatrixTranslation(2.5f, 0.0f, 15.0f); // 신호등
	worldMatrix[9] = XMMatrixTranslation(-2.5f, 0.3f, -18.5f); // 깃발 인스턴스 1
	worldMatrix[10] = XMMatrixTranslation(2.5f, 0.3f, -18.5f); // 깃발 인스턴스 2
	worldMatrix[11] = XMMatrixScaling(1.0f, 1.0f, 1.0f) * XMMatrixTranslation(18.0f, -0.3f, 10.0f); // 트랙 모델
	worldMatrix[12] = XMMatrixTranslation(0.0f, 0.0f, 0.0f); // 배경


	m_D3D->TurnZBufferOff();

	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result)
	{
		return false;
	}
	result = m_LightShader->Render(
		m_D3D->GetDeviceContext(),
		m_Bitmap->GetIndexCount(),
		worldMatrix[12],
		viewMatrix,
		orthoMatrix,
		m_Bitmap->GetTexture(),
		m_Light->GetDirection(),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), ///이거 하면 배경에는 빛 적용 안됨.
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		/*m_Light->GetAmbientColor(),
		m_Light->GetDiffuseColor(),*/
		m_Camera->GetPosition(),
		//XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower(),
		NULL, NULL
	);
	if (!result)
	{
		return false;
	}

	m_D3D->TurnZBufferOn();
	m_D3D->ClearDepthBuffer();

	// Render the model using the light shader.
	for (int i = 0; i < 12; i++)
	{
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Model[i]->Render(m_D3D->GetDeviceContext());

		// Render the model using the texture shader.
		result = m_LightShader->Render(
			m_D3D->GetDeviceContext(),
			m_Model[i]->GetIndexCount(),
			worldMatrix[i],
			viewMatrix,
			projectionMatrix,
			m_Model[i]->GetTexture(),
			m_Light->GetDirection(),
			m_Light->GetAmbientColor(),
			m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(),
			m_Light->GetSpecularPower(),
			time, static_cast<float>(i)
		);
		if (!result)
		{
			return false;
		}
	}

	if (!result)
	{
		return false;
	}

    m_Font->Render(static_cast<int>(m_totalPolygons), 
		m_Fps->GetFps(), 
		m_Cpu->GetCpuPercentage());

	m_Font->RenderTitle();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}