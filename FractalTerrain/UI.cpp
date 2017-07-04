#include "UI.h"

using namespace DirectX;

UI::UI():
  m_text(nullptr),
  m_font(nullptr)
{
}


UI::~UI()
{
}

bool UI::Initialize(D3D *d3d, int h, int w)
{
  m_width = w;
  m_height = h;

  m_text = new Text;
  if (!m_text) return false;

  m_text->Initialize(d3d->GetDevice(), d3d->GetDeviceContext(), m_width, m_height);
  m_font = new Font;
  if (!m_font) return false;

  return true;
}

void UI::Shutdown()
{
  SafeShutdown(m_text);
  SafeShutdown(m_font);
}

bool UI::Frame(ID3D11DeviceContext *device_context, int fps, float posx, float posy, float posz, float rotx, float roty, float rotz)
{
  return true;
}

bool UI::Render(D3D *d3d, ShaderManager *sm, const XMMATRIX &world_mat, const XMMATRIX& view_mat, const XMMATRIX& proj_mat)
{
  return true;
}
