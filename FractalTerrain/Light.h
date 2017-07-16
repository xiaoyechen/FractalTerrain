#pragma once
#include <DirectXMath.h>
class Light
{
public:
  Light();
  ~Light();
  void SetDiffuseColor(float, float, float, float);
  void SetDirection(float, float, float);
  DirectX::XMFLOAT4 GetDiffuseColor();
  DirectX::XMFLOAT3 GetDirection();
private:
  DirectX::XMFLOAT4 m_diffuse;
  DirectX::XMFLOAT3 m_direction;
};

