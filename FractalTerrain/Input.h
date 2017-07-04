#pragma once

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define DIRECTINPUT_VERSION DIRECTINPUT_HEADER_VERSION

#include <dinput.h>
#include "stdafx.h"

class Input
{
public:
  Input();
  ~Input();

  bool Initialize(HINSTANCE, HWND, int, int);
  void Shutdown();
  bool Frame();
  bool IsLeftPressed();
  bool IsRightPressed();
  bool IsUpPressed();
  bool IsDownPressed();
  bool IsAPressed();
  bool IsZPressed();
  bool IsPgupPressed();
  bool IsPgdownPressed();
  bool IsF1Toggled();
  bool IsF2Toggled();
  bool IsEscapePressed();
private:
  bool ReadKeyboard();
  bool ReadMouse();
  void ProcessInput();

  IDirectInput8* m_dinput;
  IDirectInputDevice8* m_keyboard;
  IDirectInputDevice8* m_mouse;
  unsigned char m_keyboardState[256];
  DIMOUSESTATE m_mouseState;
  int m_width, m_height;
  int m_mouseX, m_mouseY;
  bool m_f1_released, m_f2_released;
};

