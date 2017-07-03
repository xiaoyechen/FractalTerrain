#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include "stdafx.h"
using namespace DirectX;

class Terrain
{
public:
  Terrain();
  ~Terrain();

  bool Initialize(ID3D11Device*);
  void Shutdown();
  bool Render(ID3D11DeviceContext*);
  int GetIdxCount();
private:
  struct Vertex_T
  {
    XMFLOAT3 pos;
    XMFLOAT4 color;
  };
  ID3D11Buffer *m_vertexBuffer, *m_idxBuffer;
  int m_vertexCount, m_idxCount;

  bool InitializeBuffers(ID3D11Device*);
  void ShutdownBuffers();
  void RenderBuffers(ID3D11DeviceContext*);
};

