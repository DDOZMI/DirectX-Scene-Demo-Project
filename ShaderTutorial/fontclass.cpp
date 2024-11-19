#include "fontclass.h"

FontClass::FontClass()
{
    m_d2dFactory = nullptr;
    m_renderTarget = nullptr;
    m_writeFactory = nullptr;
    m_textFormat = nullptr;
    m_brush = nullptr;
    m_hwnd = nullptr;
}

FontClass::~FontClass()
{

}

bool FontClass::Initialize(ID3D11Device* device, HWND hwnd, int screenWidth, int screenHeight)
{
    HRESULT result;
    m_hwnd = hwnd;

    // D2D Factory
    result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory);
    if (FAILED(result))
    {
        return false;
    }

    // DirectWrite Factory
    result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_writeFactory));
    if (FAILED(result))
    {
        return false;
    }

    // Text Format
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

    // Get window size
    D2D1_SIZE_U size = SizeU(screenWidth, screenHeight);

    // Create render target
    result = m_d2dFactory->CreateHwndRenderTarget(
        RenderTargetProperties(),
        HwndRenderTargetProperties(hwnd, size),
        &m_renderTarget);
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

    if (m_renderTarget)
    {
        m_renderTarget->Release();
        m_renderTarget = nullptr;
    }

    if (m_writeFactory)
    {
        m_writeFactory->Release();
        m_writeFactory = nullptr;
    }

    if (m_d2dFactory)
    {
        m_d2dFactory->Release();
        m_d2dFactory = nullptr;
    }
}

bool FontClass::Render()
{
    m_renderTarget->BeginDraw();
    m_renderTarget->SetTransform(Matrix3x2F::Identity());

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

    return SUCCEEDED(m_renderTarget->EndDraw());
}