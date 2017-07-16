#pragma once
#include "Texture.h"

class TextureManager
{
public:
  TextureManager();
  ~TextureManager();
  bool Initialize(int);
  void Shutdown();
  bool LoadTexture(ID3D11Device*,ID3D11DeviceContext*, WCHAR*, int);
  ID3D11ShaderResourceView* GetTexture(int);
private:
  Texture* m_textureArray;
  int m_textureCount;
};

