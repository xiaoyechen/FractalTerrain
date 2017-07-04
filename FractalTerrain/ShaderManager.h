#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "stdafx.h"
#include "ColorShader.h"
#include "FontShader.h"

class ShaderManager
{
public:
  ShaderManager();
  ~ShaderManager();

  bool Initialize(ID3D11Device*, HWND);
  bool RenderColorShader(ID3D11DeviceContext*, int, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&);
  bool RenderFontShader(ID3D11DeviceContext*, int, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, ID3D11ShaderResourceView*, DirectX::XMFLOAT4);
  void Shutdown();
private:
  ColorShader* m_colorShader;
  FontShader* m_fontShader;

};

