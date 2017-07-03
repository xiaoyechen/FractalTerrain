#include "Position.h"

Position::Position():
  m_posX(0.f), m_posY(0.f), m_posZ(0.f),
  m_rotX(0.f), m_rotY(0.f), m_rotZ(0.f),
  m_frametime(0.f),
  m_forwardSpeed(0.f), m_backwardSpeed(0.f),
  m_upwardSpeed(0.f), m_downwardSpeed(0.f),
  m_leftSpeed(0.f), m_rightSpeed(0.f),
  m_lookupSpeed(0.f), m_lookdownSpeed(0.f)
{
}


Position::~Position()
{
}

void Position::SetPosition(float x, float y, float z)
{
  m_posX = x;
  m_posY = y;
  m_posZ = z;
}

void Position::SetRotation(float x, float y, float z)
{
  m_rotX = x;
  m_rotY = y;
  m_rotZ = z;
}

void Position::GetPosition(float &x, float &y, float &z)
{
  x = m_posX;
  y = m_posY;
  z = m_posZ;
}

void Position::GetRotation(float &x, float &y, float &z)
{
  x = m_rotX;
  y = m_rotY;
  z = m_rotZ;
}

void Position::SetFrameTime(float t)
{
  m_frametime = t;
}

void Position::MoveForward(bool keydown)
{
  if (keydown)
  {
    m_forwardSpeed += m_frametime*1.f;
    if (m_forwardSpeed > (m_frametime*50.f))
      m_forwardSpeed = m_frametime*50.f;
  }
  else
  {
    m_forwardSpeed -= m_frametime*0.5f;
    if (m_forwardSpeed < 0.f)
      m_forwardSpeed = 0.f;
  }

  float radian = m_rotY*DEG_RAD_COEFF;
  m_posX += sinf(radian)*m_forwardSpeed;
  m_posZ += cosf(radian)*m_forwardSpeed;
}

void Position::MoveBackward(bool keydown)
{
  if (keydown)
  {
    m_backwardSpeed += m_frametime*1.f;
    if (m_backwardSpeed > (m_frametime*50.f))
      m_backwardSpeed = m_frametime*50.f;
  }
  else
  {
    m_backwardSpeed -= m_frametime*0.5f;
    if (m_backwardSpeed < 0.f)
      m_backwardSpeed = 0.f;
  }

  float radian = m_rotY*DEG_RAD_COEFF;
  m_posX -= sinf(radian)*m_backwardSpeed;
  m_posZ -= cosf(radian)*m_backwardSpeed;
}

void Position::MoveUp(bool keydown)
{
  if (keydown)
  {
    m_upwardSpeed += m_frametime*1.5f;
    if (m_upwardSpeed > (m_frametime*15.f))
      m_upwardSpeed = m_frametime*15.f;
  }
  else
  {
    m_upwardSpeed -= m_frametime*0.5f;
    if (m_upwardSpeed < 0.f)
      m_upwardSpeed = 0.f;
  }

  m_posY += m_upwardSpeed;
}

void Position::MoveDown(bool keydown)
{
  if (keydown)
  {
    m_downwardSpeed += m_frametime*1.5f;
    if (m_downwardSpeed > (m_frametime*15.f))
      m_downwardSpeed = m_frametime*15.f;
  }
  else
  {
    m_downwardSpeed -= m_frametime*0.5f;
    if (m_downwardSpeed < 0.f)
      m_downwardSpeed = 0.f;
  }

  m_posY -= m_downwardSpeed;
}

void Position::TurnLeft(bool keydown)
{
  if (keydown)
  {
    m_leftSpeed += m_frametime*5.f;
    if (m_leftSpeed > (m_frametime*150.f))
      m_leftSpeed = m_frametime*150.f;
  }
  else
  {
    m_leftSpeed -= m_frametime*3.5f;
    if (m_leftSpeed < 0.f)
      m_leftSpeed = 0.f;
  }

  m_rotY -= m_leftSpeed;

  if (m_rotY < 0.f)
    m_rotY += 360.f;
}

void Position::TurnRight(bool keydown)
{
  if (keydown)
  {
    m_rightSpeed += m_frametime*5.f;
    if (m_rightSpeed >(m_frametime*150.f))
      m_rightSpeed = m_frametime*150.f;
  }
  else
  {
    m_rightSpeed -= m_frametime*3.5f;
    if (m_rightSpeed < 0.f)
      m_rightSpeed = 0.f;
  }

  m_rotY += m_rightSpeed;

  if (m_rotY > 360.f)
    m_rotY -= 360.f;
}

void Position::LookUp(bool keydown)
{
  if (keydown)
  {
    m_lookupSpeed += m_frametime*7.5f;
    if (m_lookupSpeed >(m_frametime*75.f))
      m_lookupSpeed = m_frametime*75.f;
  }
  else
  {
    m_lookupSpeed -= m_frametime*2.f;
    if (m_lookupSpeed < 0.f)
      m_lookupSpeed = 0.f;
  }

  m_rotX -= m_lookupSpeed;

  if (m_rotX > 90.f)
    m_rotX = 90.f;
}

void Position::LookDown(bool keydown)
{
  if (keydown)
  {
    m_lookdownSpeed += m_frametime*7.5f;
    if (m_lookdownSpeed >(m_frametime*75.f))
      m_lookdownSpeed = m_frametime*75.f;
  }
  else
  {
    m_lookdownSpeed -= m_frametime*2.f;
    if (m_lookdownSpeed < 0.f)
      m_lookdownSpeed = 0.f;
  }

  m_rotX += m_lookdownSpeed;

  if (m_rotX < -90.f)
    m_rotX = -90.f;
}
