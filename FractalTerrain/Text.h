#pragma once
#include "stdafx.h"
#include <d3d11.h>
#include <DirectXMath.h>

class Text
{
public:
  Text();
  ~Text();
  bool Initialize(char*, char*, int, int, const DirectX::XMVECTORF32&, bool shadow = false);
  void Shutdown();
  bool UpdateText(int);
  int GetPositionX() const;
  int GetPositionY() const;
  wchar_t* GetText() const;
  DirectX::XMVECTORF32 GetColor() const;
  void SetColor(const DirectX::XMVECTORF32&);
private:
  char* m_label;
  wchar_t* m_outputText;
  DirectX::XMVECTORF32 m_color;
  int m_posx, m_posy;
  bool m_shadow;
};

