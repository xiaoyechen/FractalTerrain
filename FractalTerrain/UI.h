#pragma once
#include "D3D.h"
//#include "ShaderManager.h"
#include "Text.h"
//#include "Font.h"
#include "stdafx.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>

class UI
{
public:
  UI();
  ~UI();

  bool Initialize(D3D*, int, int);
  void Shutdown();
  bool Frame(ID3D11DeviceContext*, int, float, float, float, float, float, float);
  bool Render(D3D*);//, ShaderManager*, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&);
private:
  bool UpdateFpsString(ID3D11DeviceContext*, int);
  bool UpdatePositionString(ID3D11DeviceContext*, float, float, float, float, float, float);

  Text* m_fpsText, *m_positionText;
  //Font* m_font;
  int m_fpsPrevious;
  int m_posPrevious[6];
  int m_width, m_height;
  DirectX::SpriteBatch* m_spriteBatch;
  DirectX::SpriteFont* m_spriteFont;
};

