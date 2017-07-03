#pragma once
#include <Windows.h>

class Timer
{
public:
  Timer();
  ~Timer();

  bool Initialize();
  void Frame();
  float GetTime();
private:
  INT64 m_freq;
  float m_ticksPerMs;
  INT64 m_startTime;
  float m_frameTime;
};

