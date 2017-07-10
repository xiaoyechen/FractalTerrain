#include "System.h"



System::System():
  m_app(0)
{
}


System::~System()
{
}

bool System::Initialize()
{
  int width = 0, 
      height = 0;
  bool result;

  InitializeWindows(width, height);

  m_app = new App;
  if (!m_app) return false;

  result = m_app->Initialize(m_hinstance, m_hwnd, width, height);

  return result;
}

void System::Shutdown()
{
  if (m_app)
  {
    m_app->Shutdown();
    delete m_app;
    m_app = 0;
  }

  ShutdownWindows();
}

void System::RunMsgLoop(HACCEL & hAccelTable)
{
  MSG msg;
  bool done = false;

  ZeroMemory(&msg, sizeof(MSG));

  // seems like GetMessage makes input processing laggy
  while (!done /*&& GetMessage(&msg, nullptr, 0, 0)*/)
  {
    if (PeekMessage(&msg, NULL, 0,0, PM_REMOVE) && !TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT)
      done = true;
    else 
    {
      if (!Frame())
        done = true;
    }
  }
}

bool System::Frame()
{
  return m_app->Frame();
}

void System::InitializeWindows(int &width, int &height)
{
  WNDCLASSEXW wcex;
  DEVMODE dm_screen_settings;
  int posx, posy;

  //app_handle = this;

  m_hinstance = GetModuleHandle(NULL);
  m_appName = L"Engine";

  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wcex.lpfnWndProc = System::WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = sizeof(LONG_PTR);
  wcex.hInstance = m_hinstance;
  wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = m_appName;
  wcex.hIconSm = wcex.hIcon; // required

  RegisterClassEx(&wcex);

  // set screen size
  width = GetSystemMetrics(SM_CXSCREEN);
  height = GetSystemMetrics(SM_CYSCREEN);

  if (FULL_SCREEN)
  {
    memset(&dm_screen_settings, 0, sizeof(dm_screen_settings));
    dm_screen_settings.dmSize = sizeof(dm_screen_settings);
    dm_screen_settings.dmPelsWidth = (unsigned long)width;
    dm_screen_settings.dmPelsHeight = (unsigned long)height;
    dm_screen_settings.dmBitsPerPel = 32;
    dm_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    ChangeDisplaySettings(&dm_screen_settings, CDS_FULLSCREEN);
    posx = posy = 0;
  }
  else
  {
    width = 800;
    height = 600;
    posx = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    posy = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
  }

  m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName, m_appName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posx, posy, width, height, NULL, NULL, m_hinstance, NULL);
  
  if (m_hwnd)
  {
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);

    ShowCursor(false);
  }
}

void System::ShutdownWindows()
{
  ShowCursor(true);

  if (FULL_SCREEN)
  {
    ChangeDisplaySettings(NULL, 0);
  }

  DestroyWindow(m_hwnd);
  m_hwnd = nullptr;

  UnregisterClass(m_appName, m_hinstance);
  m_hinstance = nullptr;

  //app_handle = nullptr;
}

LRESULT CALLBACK System::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  case WM_DESTROY:
  case WM_CLOSE:
    PostQuitMessage(0);
    return 0;
  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
}
