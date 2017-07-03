#pragma once
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>

class Fps
{
public:
  Fps();
  ~Fps();

  void Initialize();
  void Frame();
  int GetFps();
private:
  int m_fps, m_count;
  unsigned long m_startTime;
};

