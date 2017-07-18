#include "Zone.h"

using namespace DirectX;

Zone::Zone():
  m_ui(nullptr),
  m_cam(nullptr),
  m_pos(nullptr),
  m_terrain(nullptr),
  m_light(nullptr)
{
}


Zone::~Zone()
{
}

bool Zone::Initialize(D3D *d3d, HWND hwnd, int width, int height, float depth)
{
  bool result;

  // create ui
  m_ui = new UI;
  if (!m_ui) return false;

  result = m_ui->Initialize(d3d, height, width);

  if (!result)
  {
    MessageBox(hwnd, L"Could not initialize UI", L"Error", MB_OK);
    return false;
  }

  m_cam = new Camera;
  if (!m_cam) return false;

  m_cam->SetPosition(0.f, 0.f, 10.f);
  m_cam->Render();
  m_cam->RenderBaseViewMatrix();

  m_terrain = new Terrain;
  if (!m_terrain) return false;

  result = m_terrain->Initialize(d3d->GetDevice(), "Setup.txt");
  if (!result)
  {
    MessageBox(hwnd, L"Could not initialize terrain", L"Error", MB_OK);
    return false;
  }

  m_pos = new Position;
  if (!m_pos) return false;

  float zone_pos = m_terrain->GetTerrainHeight() / 2.f;
  m_pos->SetPosition(zone_pos, zone_pos/2.f, -zone_pos);
  m_pos->SetRotation(0.f, 0.f, 0.f);

  m_light = new Light;
  if (!m_light) return false;

  m_light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
  m_light->SetAmbientColor(.15f, .15f, .15f, 1.f);
  m_light->SetSpecularColor(1.f, 1.f, 1.f, 1.f);
  m_light->SetDirection(-.5f, 0.f, .5f);
  m_light->SetSpecularPower(.5f);

  m_displayUI = true;
  m_wireframeMode = true;

  return true;
}

void Zone::Shutdown()
{
  SafeDelete(m_terrain);
  SafeDelete(m_pos);
  SafeDelete(m_cam);
  SafeDelete(m_light);
  SafeShutdown(m_ui);
}

bool Zone::Frame(D3D *d3d, Input *in, ShaderManager *shader_manager, TextureManager* texture_manager, float frame_time, int fps)
{
  float posx, posy, posz, rotx, roty, rotz;

  HandleMovementInput(in, frame_time);

  m_pos->GetPosition(posx, posy, posz);
  m_pos->GetRotation(rotx, roty, rotz);

  bool result = m_ui->Frame(d3d->GetDeviceContext(), fps, posx, posy, posz, rotx, roty, rotz);

  if (!result) return false;

  result = Render(d3d, shader_manager, texture_manager);

  return result;
}

void Zone::HandleMovementInput(Input *in, float frame_time)
{
  bool key_down;
  float posx, posy, posz, rotx, roty, rotz;

  m_pos->SetFrameTime(frame_time);

  key_down = in->IsLeftPressed();
  m_pos->TurnLeft(key_down);

  key_down = in->IsRightPressed();
  m_pos->TurnRight(key_down);

  key_down = in->IsUpPressed();
  m_pos->MoveForward(key_down);

  key_down = in->IsDownPressed();
  m_pos->MoveBackward(key_down);

  key_down = in->IsAPressed();
  m_pos->MoveUp(key_down);

  key_down = in->IsZPressed();
  m_pos->MoveDown(key_down);

  key_down = in->IsPgupPressed();
  m_pos->LookUp(key_down);

  key_down = in->IsPgdownPressed();
  m_pos->LookDown(key_down);

  m_pos->GetPosition(posx, posy, posz);
  m_pos->GetRotation(rotx, roty, rotz);

  m_cam->SetPosition(posx, posy, posz);
  m_cam->SetRotation(rotx, roty, rotz);

  if (in->IsF1Toggled())
    m_displayUI = !m_displayUI;
  
  if (in->IsF2Toggled())
    m_wireframeMode = !m_wireframeMode;
}

bool Zone::Render(D3D *d3d, ShaderManager *sm, TextureManager* tm)
{
  XMMATRIX world_mat, view_mat, proj_mat, baseview_mat, ortho_mat;

  m_cam->Render();

  d3d->GetWorldMatrix(world_mat);
  m_cam->GetViewMatrix(view_mat);
  d3d->GetProjectionMatrix(proj_mat);
  m_cam->GetBaseViewMatrix(baseview_mat);
  d3d->GetOrthoMatrix(ortho_mat);

  d3d->BeginScene(0.f, 0.f, 0.f, 1.f);

  if (m_wireframeMode)
    d3d->EnableWireframe();

  m_terrain->Render(d3d->GetDeviceContext());
  
  //bool result = sm->RenderColorShader(d3d->GetDeviceContext(), m_terrain->GetIdxCount(), world_mat, view_mat, proj_mat);
  //bool result = sm->RenderTextureShader(d3d->GetDeviceContext(), m_terrain->GetIdxCount(), world_mat, view_mat, proj_mat, tm->GetTexture(1));
  bool result = sm->RenderLightShader(d3d->GetDeviceContext(), m_terrain->GetIdxCount(), world_mat, view_mat, proj_mat, tm->GetTexture(1), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor(), m_cam->GetPosition(), m_light->GetSpecularColor(), m_light->GetSpecularPower());
  if (!result) return false;

  if (m_wireframeMode)
    d3d->DisableWireframe();
  
  if (m_displayUI)
  {
    if (!m_ui->Render(d3d))//, sm, world_mat, baseview_mat, ortho_mat))
      return false;
  }

  d3d->EndScene();

  return true;
}
