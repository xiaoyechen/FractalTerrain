#include "Light.h"

using namespace DirectX;

Light::Light()
{
}


Light::~Light()
{
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
  m_diffuse = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
  m_ambient = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
  m_specular = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetDirection(float x, float y, float z)
{
  m_direction = XMFLOAT3(x, y, z);
}

void Light::SetSpecularPower(float power)
{
  m_specularPower = power;
}

DirectX::XMFLOAT4 Light::GetDiffuseColor()
{
  return m_diffuse;
}

DirectX::XMFLOAT4 Light::GetAmbientColor()
{
  return m_ambient;
}

DirectX::XMFLOAT4 Light::GetSpecularColor()
{
  return m_specular;
}

DirectX::XMFLOAT3 Light::GetDirection()
{
  return m_direction;
}

float Light::GetSpecularPower()
{
  return m_specularPower;
}
