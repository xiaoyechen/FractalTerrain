#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "DDSTextureLoader.h"
//#include "WICTextureLoader.h"

#include "stdafx.h"


class Texture
{
public:
  Texture();
  ~Texture();
  bool Initialize(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
  void Shutdown();
  ID3D11ShaderResourceView* GetTexture();
private:
  //bool LoadJpeg(char*, int&, int&);

  //unsigned char* m_jpegData;
  ID3D11Texture2D* m_texture;
  ID3D11ShaderResourceView* m_textureView;
};

