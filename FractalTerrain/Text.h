#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
class Text
{
public:
  Text();
  ~Text();
  bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int);
  void Shutdown();
};

