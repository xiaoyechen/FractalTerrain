#include "Timer.h"



Timer::Timer()
{
}


Timer::~Timer()
{
}

bool Timer::Initialize()
{
  // check if system supports high performance timers
  QueryPerformanceFrequency((LARGE_INTEGER*)&m_freq);
  if (m_freq == 0)
    return false;

  m_ticksPerMs = (float)m_freq;

  QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
  
  return true;
}

void Timer::Frame()
{
  INT64 cur_time;
  INT64 time_diff;

  QueryPerformanceCounter((LARGE_INTEGER*)&cur_time);

  time_diff = cur_time - m_startTime;

  m_frameTime = (float)time_diff / m_ticksPerMs;

  m_startTime = cur_time;
}

float Timer::GetTime()
{
  return m_frameTime;
}
