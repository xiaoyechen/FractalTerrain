#pragma once
#include <d3d11.h>
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"

#include "stdafx.h"

using namespace DirectX;

class Texture
{
public:
  Texture();
  ~Texture();
  bool Initialize(ID3D11Device*, WCHAR*);
  void Shutdown();
  ID3D11ShaderResourceView* GetTexture();
private:
  ID3D11ShaderResourceView* m_texture;
};

