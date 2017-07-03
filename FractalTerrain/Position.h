#pragma once
#include "stdafx.h"

class Position
{
public:
  Position();
  ~Position();

  void SetPosition(float, float, float);
  void SetRotation(float, float, float);
  void GetPosition(float&, float&, float&);
  void GetRotation(float&, float&, float&);

  void SetFrameTime(float);

  void MoveForward(bool);
  void MoveBackward(bool);
  void MoveUp(bool);
  void MoveDown(bool);
  void TurnLeft(bool);
  void TurnRight(bool);
  void LookUp(bool);
  void LookDown(bool);
private:
  float m_posX, m_posY, m_posZ;
  float m_rotX, m_rotY, m_rotZ;

  float m_frametime;

  float m_forwardSpeed, m_backwardSpeed,
        m_upwardSpeed, m_downwardSpeed,
        m_leftSpeed, m_rightSpeed,
        m_lookupSpeed, m_lookdownSpeed;
};

