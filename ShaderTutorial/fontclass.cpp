#include "fontclass.h"

FontClass::FontClass()
{
    m_renderTarget = nullptr;
    m_writeFactory = nullptr;
    m_textFormat = nullptr;
    m_brush = nullptr;
    m_hwnd = nullptr;
}

FontClass::FontClass(const FontClass&)
{

}

FontClass::~FontClass() 
{

}

bool FontClass::Initialize(ID3D11Device* device, D3DClass* d3dClass, HWND hwnd, int screenWidth, int screenHeight)
{
    HRESULT result;
    m_hwnd = hwnd;

    // Get D2D render target from D3DClass
    m_renderTarget = d3dClass->GetD2DRenderTarget();
    if (!m_renderTarget)
    {
        return false;
    }

    // Create DirectWrite factory
    result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_writeFactory));
    if (FAILED(result))
    {
        return false;
    }

    // Create text format
    result = m_writeFactory->CreateTextFormat(
        L"Arial",
        nullptr,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        20.0f, // 폰트 크기 담당
        L"en-us",
        &m_textFormat);
    if (FAILED(result))
    {
        return false;
    }

    // Create brush
    result = m_renderTarget->CreateSolidColorBrush(
        ColorF(ColorF::White),
        &m_brush);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void FontClass::Shutdown()
{
    if (m_brush)
    {
        m_brush->Release();
        m_brush = nullptr;
    }

    if (m_textFormat)
    {
        m_textFormat->Release();
        m_textFormat = nullptr;
    }

    if (m_writeFactory)
    {
        m_writeFactory->Release();
        m_writeFactory = nullptr;
    }

    m_renderTarget = nullptr;
}

bool FontClass::Render(int polygonCount, int fps, int cpu)
{
    D2D1_SIZE_F size = m_renderTarget->GetSize();
    D2D1_RECT_F rect = RectF(
        20.0f,
        20.0f,
        size.width - 10.0f,
        40.0f);

    wstring text = L"Screen Resolution: 1600 x 800\n"
                L"Objects: 14\n"
                L"Polygons : " + to_wstring(polygonCount) + L"\n"
                L"CPU: " + to_wstring(cpu) + L"%\n"
                L"FPS: " + to_wstring(fps);

    m_renderTarget->DrawText(
        text.c_str(),
        text.length(),
        m_textFormat,
        rect,
        m_brush);

    return true;
}

bool FontClass::RenderTitle()
{
    if (!m_renderTarget)
    {
        return false;
    }

    D2D1_SIZE_F size = m_renderTarget->GetSize();

    D2D1_RECT_F rect = D2D1::RectF(
        size.width - 300.0f,
        20.0f,
        size.width - 20.0f,
        60.0f
    );

    wstring titleInfo = L"Title: Racing Track\n\n"
        L"C077021 LEE DONGHOON\n\n"
        L"W,A,S,D to move\n\n"
        L"Mouse control to rotate camera";

    m_renderTarget->DrawText(
        titleInfo.c_str(),
        titleInfo.length(),
        m_textFormat,
        rect,
        m_brush
    );

    return true;
}