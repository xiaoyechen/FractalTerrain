#pragma once
#include "D3D.h"
#include "ShaderManager.h"
#include "Text.h"
#include "Font.h"

class UI
{
public:
  UI();
  ~UI();

  bool Initialize(D3D*, int, int);
  void Shutdown();
  bool Frame(ID3D11DeviceContext*, int, float, float, float, float, float, float);
  bool Render(D3D*, ShaderManager*, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&);
private:
  Text* m_text;
  Font* m_font;
  int m_width, m_height;
};

