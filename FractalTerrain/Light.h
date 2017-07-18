#pragma once
#include <DirectXMath.h>
class Light
{
public:
  Light();
  ~Light();
  void SetDiffuseColor(float, float, float, float);
  void SetAmbientColor(float, float, float, float);
  void SetSpecularColor(float, float, float, float);
  void SetDirection(float, float, float);
  void SetSpecularPower(float);
  DirectX::XMFLOAT4 GetDiffuseColor();
  DirectX::XMFLOAT4 GetAmbientColor();
  DirectX::XMFLOAT4 GetSpecularColor();
  DirectX::XMFLOAT3 GetDirection();
  float GetSpecularPower();
private:
  DirectX::XMFLOAT4 m_diffuse, m_ambient, m_specular;
  DirectX::XMFLOAT3 m_direction;
  float m_specularPower;
};

