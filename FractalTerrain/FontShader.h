#pragma once
#include <d3d11.h>
#include <DirectXMath.h>


class FontShader
{
public:
  FontShader();
  ~FontShader();
  bool Initialize(ID3D11Device*, HWND);
  void Shutdown();
  bool Render(ID3D11DeviceContext*, int, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, ID3D11ShaderResourceView*, DirectX::XMFLOAT4);
};

