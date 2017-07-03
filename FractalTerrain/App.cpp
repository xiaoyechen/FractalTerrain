#include "App.h"



App::App():
  m_in(nullptr),
  m_d3d(nullptr),
  m_shaderManager(nullptr),
  m_timer(nullptr),
  m_fps(nullptr),
  m_zone(nullptr)
{
}

App::App(const App &)
{
}


App::~App()
{
}

bool App::Initialize(HINSTANCE hInst, HWND hwnd, int width, int height)
{
  bool result;

  // create input
  m_in = new Input;

  if (!m_in) return false;
  
  result = m_in->Initialize(hInst, hwnd, width, height);

  if (!result)
  {
    MessageBox(hwnd, L"Could not initialize input object.", L"Error", MB_OK);
    return false;
  }

  // create d3d
  m_d3d = new D3D;
  if (!m_d3d) return false;

  result = m_d3d->Initialize(width, height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
  if (!result)
  {
    MessageBox(hwnd, L"Could not initialize Direct3D object.", L"Error", MB_OK);
    return false;
  }

  // create shader manager
  m_shaderManager = new ShaderManager;
  if (!m_shaderManager) return false;

  result = m_shaderManager->Initialize(m_d3d->GetDevice(), hwnd);
  if (!result)
  {
    MessageBox(hwnd, L"Could not initialize shader manager object.", L"Error", MB_OK);
    return false;
  }

  // create timer
  m_timer = new Timer;
  if (!m_timer) return false;

  result = m_timer->Initialize();
  if (!result)
  {
    MessageBox(hwnd, L"Could not initialize timer object.", L"Error", MB_OK);
    return false;
  }

  // create fps
  m_fps = new Fps;
  if (!m_fps) return false;

  m_fps->Initialize();

  // create Zone
  m_zone = new Zone;
  if (!m_zone) return false;

  result = m_zone->Initialize(m_d3d, hwnd, width, height, SCREEN_DEPTH);
  if (!result)
  {
    MessageBox(hwnd, L"Could not initialize zone object.", L"Error", MB_OK);
    return false;
  }

  return true;
}

void App::Shutdown()
{
  SafeShutdown(m_zone);
  SafeDelete(m_fps);
  SafeDelete(m_timer);
  SafeShutdown(m_shaderManager);
  SafeShutdown(m_d3d);
  SafeShutdown(m_in);
}

bool App::Frame()
{
  m_fps->Frame();
  m_timer->Frame();

  if (!m_in->Frame()) 
    return false;

  if (m_in->IsEscapePressed()) 
    return false;

  if (!m_zone->Frame(m_d3d, m_in, m_shaderManager, m_timer->GetTime(), m_fps->GetFps()))
    return false;

  return true;
}
