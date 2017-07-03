#include "Texture.h"



Texture::Texture():
  m_texture(nullptr)
{
}


Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device *device, WCHAR *filename)
{
  HRESULT hr = CreateDDSTextureFromFile(device, filename, NULL, &m_texture, NULL, NULL);
  if (FAILED(hr)) return false;

  return true;
}

void Texture::Shutdown()
{
  SafeRelease(m_texture);
}

ID3D11ShaderResourceView * Texture::GetTexture()
{
  return m_texture;
}
