#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "App.h"

class System
{
public:
  System();
  ~System();

  bool Initialize();
  void Shutdown();
  void RunMsgLoop(HACCEL& hAccelTable);
private:
  bool Frame();
  void InitializeWindows(int&, int&);
  void ShutdownWindows();
  static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

  LPCWSTR m_appName;
  HINSTANCE m_hinstance;
  HWND m_hwnd;
  App* m_app;
};

//static System* app_handle = 0;
