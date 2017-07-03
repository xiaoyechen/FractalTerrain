#include "D3D.h"



D3D::D3D():
  m_swapChain(0),
  m_device(0),
  m_deviceContext(0),
  m_rtView(0),
  m_depthStencilBuffer(0),
  m_depthStencilState(0),
  m_depthStencilView(0),
  m_rasterState(0)
{
}


D3D::~D3D()
{
}

bool D3D::Initialize(int width, int height, bool vsync, HWND hwnd, bool fullscreen, float screen_depth, float screen_near)
{
  HRESULT hr;
  IDXGIFactory* factory;
  IDXGIAdapter* adapter;
  IDXGIOutput* adapter_output;
  DXGI_MODE_DESC* display_mode_list;
  DXGI_ADAPTER_DESC adaptor_desc;
  DXGI_SWAP_CHAIN_DESC swapchain_desc;
  D3D_FEATURE_LEVEL feature_lv;
  ID3D11Texture2D* back_buffer_ptr;
  D3D11_TEXTURE2D_DESC depth_buffer_desc;
  D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
  D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
  D3D11_RASTERIZER_DESC raster_desc;
  D3D11_VIEWPORT viewport;
  unsigned int num_modes, numerator, denominator, str_length;

  m_vsync_enabled = vsync;

  hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
  if (FAILED(hr)) return false;

  hr = factory->EnumAdapters(0, &adapter);
  if (FAILED(hr)) return false;

  hr = adapter->EnumOutputs(0, &adapter_output);
  if (FAILED(hr)) return false;

  hr = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);
  if (FAILED(hr)) return false;

  display_mode_list = new DXGI_MODE_DESC[num_modes];
  if (!display_mode_list) return false;

  hr = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
  if (FAILED(hr)) return false;

  for (int i = 0; i < num_modes; ++i)
  {
    if (display_mode_list[i].Width == (unsigned int)width && display_mode_list[i].Height == (unsigned int)height)
    {
      numerator = display_mode_list[i].RefreshRate.Numerator;
      denominator = display_mode_list[i].RefreshRate.Denominator;
    }
  }

  // get adaptor(video card) info
  hr = adapter->GetDesc(&adaptor_desc);
  if (FAILED(hr)) return false;

  m_videocardMemory = (int)(adaptor_desc.DedicatedVideoMemory / 1024 / 1024);
  if (wcstombs_s(&str_length, m_videocardInfo, 128, adaptor_desc.Description, 128) != 0)
    return false;

  // release memory
  delete[] display_mode_list;
  display_mode_list = 0;

  adapter_output->Release();
  adapter_output = 0;

  adapter->Release();
  adapter = 0;

  factory->Release();
  factory = 0;

  // fill out swap chain description
  ZeroMemory(&swapchain_desc, sizeof(swapchain_desc));

  swapchain_desc.BufferCount = 1;
  swapchain_desc.BufferDesc.Width = width;
  swapchain_desc.BufferDesc.Height = height;
  swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  if (m_vsync_enabled)
  {
    swapchain_desc.BufferDesc.RefreshRate.Numerator = numerator;
    swapchain_desc.BufferDesc.RefreshRate.Denominator = denominator;
  }
  else
  {
    swapchain_desc.BufferDesc.RefreshRate.Numerator = 0;
    swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
  }

  swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  swapchain_desc.OutputWindow = hwnd;

  // turn off multisampling
  swapchain_desc.SampleDesc.Count = 1;
  swapchain_desc.SampleDesc.Quality = 0;

  if (fullscreen)
    swapchain_desc.Windowed = false;
  else
    swapchain_desc.Windowed = true;

  swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  swapchain_desc.Flags = 0;

  feature_lv = D3D_FEATURE_LEVEL_11_0;

  hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &feature_lv, 1, D3D11_SDK_VERSION, &swapchain_desc, &m_swapChain, &m_device, NULL, &m_deviceContext);
  if (FAILED(hr)) return false;

  hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_ptr);
  if (FAILED(hr)) return false;

  hr = m_device->CreateRenderTargetView(back_buffer_ptr, NULL, &m_rtView);
  if (FAILED(hr)) return false;

  back_buffer_ptr->Release(); back_buffer_ptr = 0;

  // set up depth buffer
  ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));

  depth_buffer_desc.Width = width;
  depth_buffer_desc.Height = height;
  depth_buffer_desc.MipLevels = 1;
  depth_buffer_desc.ArraySize = 1;
  depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depth_buffer_desc.SampleDesc.Count = 1;
  depth_buffer_desc.SampleDesc.Quality = 0;
  depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depth_buffer_desc.CPUAccessFlags = 0;
  depth_buffer_desc.MiscFlags = 0;

  hr = m_device->CreateTexture2D(&depth_buffer_desc, NULL, &m_depthStencilBuffer);
  if (FAILED(hr)) return false;

  // set up depth stencil description
  ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

  depth_stencil_desc.DepthEnable = true;
  depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
  
  depth_stencil_desc.StencilEnable = true;
  depth_stencil_desc.StencilReadMask = 0xff;
  depth_stencil_desc.StencilWriteMask = 0xff;

  depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  hr = m_device->CreateDepthStencilState(&depth_stencil_desc, &m_depthStencilState);
  if (FAILED(hr)) return false;

  m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

  // set up depth stencil view
  ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));

  depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  depth_stencil_view_desc.Texture2D.MipSlice = 0;

  hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depth_stencil_view_desc, &m_depthStencilView);
  if (FAILED(hr)) return false;

  // bind render target view and depth stencil buffer to output render pipe
  m_deviceContext->OMSetRenderTargets(1, &m_rtView, m_depthStencilView);

  // set up raster description
  raster_desc.AntialiasedLineEnable = false;
  raster_desc.CullMode = D3D11_CULL_BACK;
  raster_desc.DepthBias = 0;
  raster_desc.DepthBiasClamp = 0.f;
  raster_desc.DepthClipEnable = true;
  raster_desc.FillMode = D3D11_FILL_SOLID;
  raster_desc.FrontCounterClockwise = false;
  raster_desc.MultisampleEnable = false;
  raster_desc.ScissorEnable = false;
  raster_desc.SlopeScaledDepthBias = 0.f;

  hr = m_device->CreateRasterizerState(&raster_desc, &m_rasterState);
  if (FAILED(hr)) return false;

  m_deviceContext->RSSetState(m_rasterState);

  // set up viewport for rendering
  viewport.Width = (float)width;
  viewport.Height = (float)height;
  viewport.MinDepth = 0.f;
  viewport.MaxDepth = 1.f;
  viewport.TopLeftX = 0.f;
  viewport.TopLeftY = 0.f;

  m_deviceContext->RSSetViewports(1, &viewport);

  // set up matrices
  float field_of_view = (float)XM_PI / 4.f;
  float screen_aspect = (float)width / (float)height;
  m_projection_matrix = XMMatrixPerspectiveFovLH(field_of_view, screen_aspect, screen_near, screen_depth);

  m_world_matrix = XMMatrixIdentity();
  m_ortho_matrix = XMMatrixOrthographicLH((float)width, (float)height, screen_near, screen_depth);

  return true;
}

ID3D11Device * D3D::GetDevice()
{
  return m_device;
}

ID3D11DeviceContext * D3D::GetDeviceContext()
{
  return m_deviceContext;
}

void D3D::GetWorldMatrix(XMMATRIX &mat)
{
  mat = m_world_matrix;
}

void D3D::GetProjectionMatrix(XMMATRIX &mat)
{
  mat = m_projection_matrix;
}

void D3D::GetOrthoMatrix(XMMATRIX &mat)
{
  mat = m_ortho_matrix;
}

void D3D::GetVideocardInfo(char *card_name, int &memory)
{
  strcpy_s(card_name, 128, m_videocardInfo);
  memory = m_videocardMemory;
}

void D3D::BeginScene(float red, float green, float blue, float alpha)
{
  float color[4];

  color[0] = red;
  color[1] = green;
  color[2] = blue;
  color[3] = alpha;

  m_deviceContext->ClearRenderTargetView(m_rtView, color);
  m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void D3D::EndScene()
{
  if (m_vsync_enabled)
    m_swapChain->Present(1, 0); // lock to screen refresh rate
  else
    m_swapChain->Present(0, 0); // present as fast as possible
}

void D3D::Shutdown()
{
  if (m_swapChain)
    m_swapChain->SetFullscreenState(false, NULL);
  
  SafeRelease(m_rasterState);
  SafeRelease(m_depthStencilView);
  SafeRelease(m_depthStencilState);
  SafeRelease(m_depthStencilBuffer);
  SafeRelease(m_rtView);
  SafeRelease(m_deviceContext);
  SafeRelease(m_device);
  SafeRelease(m_swapChain);
}
