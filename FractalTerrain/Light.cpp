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

void Light::SetDirection(float x, float y, float z)
{
  m_direction = XMFLOAT3(x, y, z);
}

DirectX::XMFLOAT4 Light::GetDiffuseColor()
{
  return m_diffuse;
}

DirectX::XMFLOAT3 Light::GetDirection()
{
  return m_direction;
}
