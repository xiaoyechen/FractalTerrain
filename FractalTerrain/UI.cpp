#include "UI.h"

using namespace DirectX;

UI::UI():
  m_fpsText(nullptr),
  m_positionText(nullptr),
  m_spriteBatch(nullptr),
  m_spriteFont(nullptr)
{
}


UI::~UI()
{
}

bool UI::Initialize(D3D *d3d, int h, int w)
{
  m_width = w;
  m_height = h;

  // initialize SpriteFont
  m_spriteBatch = new SpriteBatch(d3d->GetDeviceContext());
  if (!m_spriteBatch) return false;

  m_spriteFont = new SpriteFont(d3d->GetDevice(), L"cn_font.spritefont");

  if (!m_spriteFont) return false;

  m_spriteFont->SetDefaultCharacter('-');

  // initialize display strings
  m_fpsText = new Text;
  if (!m_fpsText) return false;

  m_fpsText->Initialize("FPS", "0", 5, 5, Colors::White);
  
  m_positionText = new Text[6];
  if (!m_positionText) return false;

  m_positionText[0].Initialize("X", "0", 5, 18, Colors::White);
  m_positionText[1].Initialize("Y", "0", 5, 31, Colors::White);
  m_positionText[2].Initialize("Z", "0", 5, 44, Colors::White);
  m_positionText[3].Initialize("rX", "0", 5, 57, Colors::White);
  m_positionText[4].Initialize("rY", "0", 5, 70, Colors::White);
  m_positionText[5].Initialize("rZ", "0", 5, 83, Colors::White);

  return true;
}

void UI::Shutdown()
{
  SafeDelete(m_spriteBatch);
  SafeDelete(m_spriteFont);

  SafeShutdown(m_fpsText);

  if (m_positionText)
  {
    for (int i = 0; i < 6; ++i)
    {
      m_positionText[i].Shutdown();
    }
    delete[] m_positionText;
    m_positionText = 0;
  }

}

bool UI::Frame(ID3D11DeviceContext *device_context, int fps, float posx, float posy, float posz, float rotx, float roty, float rotz)
{
  if (!UpdateFpsString(device_context, fps))
    return false;

  if (!UpdatePositionString(device_context, posx, posy, posz, rotx, roty, rotz))
    return false;

  return true;
}

bool UI::Render(D3D *d3d)//, ShaderManager *sm, const XMMATRIX &world_mat, const XMMATRIX& view_mat, const XMMATRIX& proj_mat)
{
  m_spriteBatch->Begin();
  m_spriteFont->DrawString(m_spriteBatch, m_fpsText->GetText(), XMFLOAT2(m_fpsText->GetPositionX(), m_fpsText->GetPositionY()), m_fpsText->GetColor(), 0);

  for (int i = 0; i < 6; ++i)
  {
    m_spriteFont->DrawString(m_spriteBatch, m_positionText[i].GetText(), XMFLOAT2(m_positionText[i].GetPositionX(), m_positionText[i].GetPositionY()), m_positionText[i].GetColor(), 0);
  }
  
  m_spriteBatch->End();

  return true;
}

bool UI::UpdateFpsString(ID3D11DeviceContext *device_context, int fps)
{
  if (m_fpsPrevious == fps)
    return true;

  m_fpsPrevious = fps;

  if (m_fpsPrevious > 99999)
    m_fpsPrevious = 99999;

  if (!m_fpsText->UpdateText(m_fpsPrevious))
    return false;

  if (m_fpsPrevious >= 60)
    m_fpsText->SetColor(Colors::Green);
  else if (m_fpsPrevious < 30)
    m_fpsText->SetColor(Colors::Red);
  else
    m_fpsText->SetColor(Colors::Yellow);
  
  return true;
}

bool UI::UpdatePositionString(ID3D11DeviceContext *device_context, float posx, float posy, float posz, float rotx, float roty, float rotz)
{
  int position_x = (int)posx,
    position_y = (int)posy,
    position_z = (int)posz,
    rotation_x = (int)rotx,
    rotation_y = (int)roty,
    rotation_z = (int)rotz;

  if (m_posPrevious[0] != position_x && !m_positionText[0].UpdateText(position_x))
    return false;

  if (m_posPrevious[1] != position_y && !m_positionText[1].UpdateText(position_y))
    return false;

  if (m_posPrevious[2] != position_z && !m_positionText[2].UpdateText(position_z))
    return false;

  if (m_posPrevious[3] != rotation_x && !m_positionText[3].UpdateText(rotation_x))
    return false;

  if (m_posPrevious[4] != rotation_y && !m_positionText[4].UpdateText(rotation_y))
    return false;

  if (m_posPrevious[5] != rotation_z && !m_positionText[5].UpdateText(rotation_z))
    return false;

  return true;
}
