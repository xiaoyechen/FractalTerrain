#include "Camera.h"



Camera::Camera():
  m_posx(0.f), m_posy(0.f), m_posz(0.f),
  m_rotx(0.f), m_roty(0.f), m_rotz(0.f)
{
}


Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
  m_posx = x;
  m_posy = y;
  m_posz = z;
}

void Camera::SetRotation(float x, float y, float z)
{
  m_rotx = x;
  m_roty = y;
  m_rotz = z;
}

void Camera::Render()
{
  XMFLOAT3 up, position, lookat;
  XMVECTOR up_vec, pos_vec, lookat_vec;
  float yaw, pitch, roll;
  XMMATRIX rotation_matrix;

  up.x = 0.f;
  up.y = 1.f;
  up.z = 0.f;
  up_vec = XMLoadFloat3(&up);

  position.x = m_posx;
  position.y = m_posy;
  position.z = m_posz;
  pos_vec = XMLoadFloat3(&position);

  lookat.x = 0.f;
  lookat.y = 0.f;
  lookat.z = 1.f;
  lookat_vec = XMLoadFloat3(&lookat);

  // set yaw (y axis) pitch (x axis) and roll (z axis) rotation in radians
  pitch = m_rotx*DEG_RAD_COEFF;
  yaw = m_roty*DEG_RAD_COEFF;
  roll = m_rotz*DEG_RAD_COEFF;

  rotation_matrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
  lookat_vec = XMVector3TransformCoord(lookat_vec, rotation_matrix);
  up_vec = XMVector3TransformCoord(up_vec, rotation_matrix);

  //lookat_vec = pos_vec + lookat_vec;
  lookat_vec = XMVectorAdd(pos_vec, lookat_vec);
  m_view_matrix = XMMatrixLookAtLH(pos_vec, lookat_vec, up_vec);
}

void Camera::RenderBaseViewMatrix()
{
  XMFLOAT3 up, position, lookat;
  XMVECTOR up_vec, pos_vec, lookat_vec;
  float yaw, pitch, roll;
  XMMATRIX rotation_matrix;

  up.x = 0.f;
  up.y = 1.f;
  up.z = 0.f;
  up_vec = XMLoadFloat3(&up);

  position.x = m_posx;
  position.y = m_posy;
  position.z = m_posz;
  pos_vec = XMLoadFloat3(&position);

  lookat.x = 0.f;
  lookat.y = 0.f;
  lookat.z = 1.f;
  lookat_vec = XMLoadFloat3(&lookat);

  // set yaw (y axis) pitch (x axis) and roll (z axis) rotation in radians
  pitch = m_rotx*DEG_RAD_COEFF;
  yaw = m_roty*DEG_RAD_COEFF;
  roll = m_rotz*DEG_RAD_COEFF;

  rotation_matrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
  lookat_vec = XMVector3TransformCoord(lookat_vec, rotation_matrix);
  up_vec = XMVector3TransformCoord(up_vec, rotation_matrix);

  //lookat_vec = pos_vec + lookat_vec;
  lookat_vec = XMVectorAdd(pos_vec, lookat_vec);
  m_baseview_matrix = XMMatrixLookAtLH(pos_vec, lookat_vec, up_vec);
}

void Camera::GetViewMatrix(XMMATRIX &mat)
{
  mat = m_view_matrix;
}

void Camera::GetBaseViewMatrix(XMMATRIX &mat)
{
  mat = m_baseview_matrix;
}
