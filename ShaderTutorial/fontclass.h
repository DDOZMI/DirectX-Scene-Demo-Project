#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <d2d1_1.h>
#include <dwrite_1.h>
#include <d3d11.h>
#include "d3dclass.h"

using namespace D2D1;

class FontClass {
public:
    FontClass();
    FontClass(const FontClass&);
    ~FontClass();

    bool Initialize(ID3D11Device* device, D3DClass* d3dClass, HWND hwnd, int screenWidth, int screenHeight);
    void Shutdown();
    bool Render();

private:
    ID2D1RenderTarget* m_renderTarget;
    IDWriteFactory* m_writeFactory;
    IDWriteTextFormat* m_textFormat;
    ID2D1SolidColorBrush* m_brush;
    HWND m_hwnd;
};

#endif // !_FONTCLASS_H_

