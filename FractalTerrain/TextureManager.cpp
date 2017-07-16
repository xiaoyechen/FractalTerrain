#include "TextureManager.h"



TextureManager::TextureManager()
  :m_textureArray(nullptr)
{
}


TextureManager::~TextureManager()
{
}

bool TextureManager::Initialize(int texture_count)
{
  m_textureCount = texture_count;

  m_textureArray = new Texture[m_textureCount];
  if (!m_textureArray) return false;

  return true;
}

void TextureManager::Shutdown()
{
  if (m_textureArray)
  {
    for (int i = 0; i < m_textureCount; ++i)
      m_textureArray[i].Shutdown();

    delete[] m_textureArray;
    m_textureArray = 0;
  }
}

bool TextureManager::LoadTexture(ID3D11Device *device, ID3D11DeviceContext *device_context, WCHAR *filename, int location)
{
  return m_textureArray[location].Initialize(device, device_context, filename);
}

ID3D11ShaderResourceView * TextureManager::GetTexture(int idx)
{
  return m_textureArray[idx].GetTexture();
}
