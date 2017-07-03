#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "stdafx.h"
#include "ColorShader.h"
#include "FontShader.h"
using namespace DirectX;

class ShaderManager
{
public:
  ShaderManager();
  ~ShaderManager();

  bool Initialize(ID3D11Device*, HWND);
  bool RenderColorShader(ID3D11DeviceContext*, int, XMMATRIX&, XMMATRIX&, XMMATRIX&);
  bool RenderFontShader(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*, XMFLOAT4);
  void Shutdown();
private:
  ColorShader* m_colorShader;
  FontShader* m_fontShader;

};

