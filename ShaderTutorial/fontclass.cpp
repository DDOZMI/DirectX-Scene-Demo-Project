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
        20.0f,
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

bool FontClass::Render()
{
    D2D1_SIZE_F size = m_renderTarget->GetSize();
    D2D1_RECT_F rect = RectF(
        size.width - 550.0f,
        10.0f,
        size.width - 10.0f,
        40.0f);

    m_renderTarget->DrawText(
        L"Number of Objects: 14 (3 + 10 + 1)",
        34,
        m_textFormat,
        rect,
        m_brush);

    return true;
}