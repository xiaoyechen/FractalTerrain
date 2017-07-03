#pragma once
#include <DirectXMath.h>
#include "stdafx.h"
using namespace DirectX;

class Camera
{
public:
  Camera();
  ~Camera();

  void SetPosition(float, float, float);
  void SetRotation(float, float, float);
  void Render();
  void RenderBaseViewMatrix();
  void GetViewMatrix(XMMATRIX&);
  void GetBaseViewMatrix(XMMATRIX&);
private:
  float m_posx, m_posy, m_posz;
  float m_rotx, m_roty, m_rotz;
  XMMATRIX m_view_matrix, m_baseview_matrix;
};

