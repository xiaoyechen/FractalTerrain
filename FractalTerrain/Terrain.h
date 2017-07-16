#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <stdio.h>
#include "stdafx.h"
#include "Fractal.h"

class Terrain
{
public:
  Terrain();
  ~Terrain();

  bool Initialize(ID3D11Device*, char*);
  void Shutdown();
  bool Render(ID3D11DeviceContext*);
  int GetIdxCount();
  int GetTerrainHeight();
private:
  struct Vertex_T
  {
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 texture;
  };
  struct HeightMap_T
  {
    float x, y, z;
  };
  struct Model_T
  {
    float x, y, z;
    float tu, tv;
  };
  ID3D11Buffer *m_vertexBuffer, *m_idxBuffer;
  int m_vertexCount, m_idxCount;
  int m_terrainHeight, m_terrainWidth;
  float m_maxRandom;
  float m_heightScale;
  float m_terrainRoughness;
  HeightMap_T* m_heightMap;
  Model_T* m_terrainModel;

  bool InitializeBuffers(ID3D11Device*);
  void ShutdownBuffers();
  void RenderBuffers(ID3D11DeviceContext*);
  bool LoadSetupFile(char*);
  bool LoadHeightMap();
  void ShutdownHeightMap();
  void SetTerrainCoordinates();
  bool BuildTerrainModel();
  void ShutdownTerrainModel();
};

