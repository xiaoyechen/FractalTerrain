#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.1f;

#include "D3D.h"
#include "Fps.h"
#include "Input.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Timer.h"
#include "Zone.h"

class App
{
public:
  App();
  App(const App&);
  ~App();

  bool Initialize(HINSTANCE, HWND, int, int);
  void Shutdown();
  bool Frame();
private:
  Input* m_in;
  D3D* m_d3d;
  ShaderManager* m_shaderManager;
  TextureManager* m_textureManager;
  Timer* m_timer;
  Fps* m_fps;
  Zone* m_zone;
};

