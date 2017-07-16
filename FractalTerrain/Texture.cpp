#include "Texture.h"

using namespace DirectX;

Texture::Texture():
  m_texture(nullptr),
  m_textureView(nullptr)
{
}


Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device *device, ID3D11DeviceContext* device_context, WCHAR *filename)
{
  /*wchar_t* filename_wchar;

  size_t newsize = strlen(filename) + 1;
  size_t converted_chars = 0;

  mbstowcs_s(&converted_chars, filename_wchar, newsize, filename, _TRUNCATE);
  */
  HRESULT hr = CreateDDSTextureFromFile(device, filename, nullptr, &m_textureView, 0);

  if (FAILED(hr))
    return false;

  /*
  int height, width;
  unsigned int row_pitch;
  HRESULT hr;

  if (!LoadJpeg(filename, height, width))
    return false;
  

  D3D11_TEXTURE2D_DESC texture_desc;
  texture_desc.Height = height;
  texture_desc.Width = width;
  texture_desc.MipLevels = 0;
  texture_desc.ArraySize = 1;
  texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  texture_desc.SampleDesc.Count = 1;
  texture_desc.SampleDesc.Quality = 0;
  texture_desc.Usage = D3D11_USAGE_DEFAULT;
  texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
  texture_desc.CPUAccessFlags = 0;
  texture_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

  hr = device->CreateTexture2D(&texture_desc, NULL, &m_texture);
  if (FAILED(hr)) return false;
  
  row_pitch = 4 * width * sizeof(unsigned char);

  device_context->UpdateSubresource(m_texture, 0, NULL, m_jpegData, row_pitch, 0);

  D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
  srv_desc.Format = texture_desc.Format;
  srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srv_desc.Texture2D.MostDetailedMip = 0;
  srv_desc.Texture2D.MipLevels = -1;

  hr = device->CreateShaderResourceView(m_texture, &srv_desc, &m_textureView);
  if (FAILED(hr)) return false;

  device_context->GenerateMips(m_textureView);

  delete[] m_jpegData;
  m_jpegData = 0;
  */
  return true;
}

void Texture::Shutdown()
{
  SafeRelease(m_textureView);
  SafeRelease(m_texture);
}

ID3D11ShaderResourceView * Texture::GetTexture()
{
  return m_textureView;
}
