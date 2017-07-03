#include "Input.h"

Input::Input():
  m_dinput(nullptr),
  m_keyboard(nullptr),
  m_mouse(nullptr)
{
}


Input::~Input()
{
}

bool Input::Initialize(HINSTANCE hinst, HWND hwnd, int w, int h)
{
  HRESULT result;

  m_width = w;
  m_height = h;

  m_mouseX = 0;
  m_mouseY = 0;

  // initialize interface to Direct input
  result = DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_dinput, NULL);
  if (FAILED(result)) return false;

  // initialize keyboard
  result = m_dinput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
  if (FAILED(result)) return false;

  result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
  if (FAILED(result)) return false;

  result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
  if (FAILED(result)) return false;

  result = m_keyboard->Acquire();
  if (FAILED(result)) return false;

  // initialize mouse
  result = m_dinput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
  if (FAILED(result)) return false;

  result = m_mouse->SetDataFormat(&c_dfDIMouse);
  if (FAILED(result)) return false;

  result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
  if (FAILED(result)) return false;

  result = m_mouse->Acquire();
  if (FAILED(result)) return false;

  m_f1_released = true;
 // m_f2_released = true;

  return true;
}

void Input::Shutdown()
{
  if (m_mouse)
  {
    m_mouse->Unacquire();
    m_mouse->Release();
    m_mouse = 0;
  }

  if (m_keyboard)
  {
    m_keyboard->Unacquire();
    m_keyboard->Release();
    m_keyboard = 0;
  }

  SafeRelease(m_dinput);
}

bool Input::Frame()
{
  if (!ReadKeyboard())
    return false;

  if (!ReadMouse())
    return false;

  ProcessInput();

  return true;
}

bool Input::IsLeftPressed()
{
  if (m_keyboardState[DIK_LEFT] & 0x80)
    return true;

  return false;
}

bool Input::IsRightPressed()
{
  if (m_keyboardState[DIK_RIGHT] & 0x80)
    return true;

  return false;
}

bool Input::IsUpPressed()
{
  if (m_keyboardState[DIK_UP] & 0x80)
    return true;

  return false;
}

bool Input::IsDownPressed()
{
  if (m_keyboardState[DIK_DOWN] & 0x80)
    return true;

  return false;
}

bool Input::IsAPressed()
{
  if (m_keyboardState[DIK_A] & 0x80)
    return true;

  return false;
}

bool Input::IsZPressed()
{
  if (m_keyboardState[DIK_Z] & 0x80)
    return true;

  return false;
}

bool Input::IsPgupPressed()
{
  if (m_keyboardState[DIK_PGUP] & 0x80)
    return true;

  return false;
}

bool Input::IsPgdownPressed()
{
  if (m_keyboardState[DIK_PGDN] & 0x80)
    return true;

  return false;
}

bool Input::IsF1Toggled()
{
  if (m_keyboardState[DIK_F1] & 0x80)
  {
    if (m_f1_released)
    {
      m_f1_released = false;
      return true;
    }
  }
  else
  {
    m_f1_released = true;
  }

  return false;
}

bool Input::IsEscapePressed()
{
  if (m_keyboardState[DIK_ESCAPE] & 0x80)
    return true;

  return false;
}

bool Input::ReadKeyboard()
{
  HRESULT hr = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
  if (FAILED(hr))
  {
    if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
      m_keyboard->Acquire();
    else
      return false;
  }

  return true;
}

bool Input::ReadMouse()
{
  HRESULT hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
  if (FAILED(hr))
  {
    if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
      m_mouse->Acquire();
    else
      return false;
  }
  return true;
}

void Input::ProcessInput()
{
  m_mouseX += m_mouseState.lX;
  m_mouseY += m_mouseState.lY;

  if (m_mouseX < 0) m_mouseX = 0;
  if (m_mouseY < 0) m_mouseY = 0;

  if (m_mouseX > m_width) m_mouseX = m_width;
  if (m_mouseY > m_height) m_mouseY = m_height;
}
