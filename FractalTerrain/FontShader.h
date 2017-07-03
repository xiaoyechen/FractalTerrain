#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class FontShader
{
public:
  FontShader();
  ~FontShader();
  bool Initialize(ID3D11Device*, HWND);
  void Shutdown();
  bool Render(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*, XMFLOAT4);
};

