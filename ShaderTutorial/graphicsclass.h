////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "inputclass.h"
#include "bitmapclass.h"
#include "fontclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include <vector>

using namespace std;

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	void SetBackgroundColor(float, float, float);
	void SetBrightness(float);
	void SetRenderMode(bool);
	void SetBackFaceCulling(bool);
	void SetTextureFilter(int);
	void SetLightingMode(int);
	void SetLightColor(float, float, float);
	void SetLightBrightness(bool);

	CameraClass* GetCamera();

private:
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model[12];
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	InputClass* m_Input;
	BitmapClass* m_Bitmap;
	FontClass* m_Font;
	CpuClass* m_Cpu;
	FpsClass* m_Fps;

	float m_bgColor[4];
	float m_brightness;
	bool m_wireframeMode;
	bool m_backFaceCulling;
	int m_lightingMode;
	float m_lightBrightness;
	unsigned int m_totalPolygons;
};

#endif