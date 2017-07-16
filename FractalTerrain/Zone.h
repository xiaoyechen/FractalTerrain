/**
Terrain processing
*/
#pragma once

#include "D3D.h"
#include "Input.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Timer.h"
#include "UI.h"
#include "Camera.h"
#include "Position.h"
#include "Terrain.h"
#include "Light.h"

class Zone
{
public:
  Zone();
  ~Zone();

  bool Initialize(D3D*, HWND, int, int, float);
  void Shutdown();
  bool Frame(D3D*, Input*, ShaderManager*, TextureManager*, float, int);
private:
  void HandleMovementInput(Input*, float);
  bool Render(D3D*, ShaderManager*, TextureManager*);

  UI* m_ui;
  Camera* m_cam;
  Position* m_pos;
  Terrain* m_terrain;
  Light* m_light;
  bool m_displayUI, m_wireframeMode;
};

