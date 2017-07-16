#include "Terrain.h"
using namespace DirectX;

Terrain::Terrain():
  m_vertexBuffer(nullptr),
  m_idxBuffer(nullptr),
  m_heightMap(nullptr),
  m_terrainModel(nullptr)
{
}


Terrain::~Terrain()
{
}

bool Terrain::Initialize(ID3D11Device *device, char* setup_filename)
{
  if (!LoadSetupFile(setup_filename))
    return false;

  if (!LoadHeightMap())
    return false;

  SetTerrainCoordinates();

  if (!CalculateNormals())
    return false;

  if (!BuildTerrainModel())
    return false;

  ShutdownHeightMap();

  if (!InitializeBuffers(device))
    return false;

  ShutdownTerrainModel();

  return true;
}

void Terrain::Shutdown()
{
  ShutdownBuffers();
  ShutdownTerrainModel();
  ShutdownHeightMap();
}

bool Terrain::Render(ID3D11DeviceContext *device_context)
{
  RenderBuffers(device_context);
  return true;
}

int Terrain::GetIdxCount()
{
  return m_idxCount;
}

int Terrain::GetTerrainHeight()
{
  return m_terrainHeight;
}

bool Terrain::InitializeBuffers(ID3D11Device *device)
{

  D3D11_BUFFER_DESC vertexBufferDesc, idxBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, idxData;
  HRESULT hr;

  XMFLOAT4 color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

  m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;
  m_idxCount = m_vertexCount;

  Vertex_T* vertices = new Vertex_T[m_vertexCount];
  if (!vertices) return false;

  unsigned long* indices = new unsigned long[m_idxCount];
  if (!indices) return false;

  for (int i = 0; i < m_vertexCount; ++i)
  {
    vertices[i].pos = XMFLOAT3(m_terrainModel[i].x, m_terrainModel[i].y, m_terrainModel[i].z);
    //vertices[i].color = color;
    vertices[i].tex = XMFLOAT2(m_terrainModel[i].tu, m_terrainModel[i].tv);
    vertices[i].norm = XMFLOAT3(m_terrainModel[i].nx, m_terrainModel[i].ny, m_terrainModel[i].nz);
    indices[i] = i;
  }
  
  // create vertex buffer
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(Vertex_T)* m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  vertexData.pSysMem = vertices;
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

  if (FAILED(hr)) return false;

  // create index buffer
  idxBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  idxBufferDesc.ByteWidth = sizeof(unsigned long)* m_idxCount;
  idxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  idxBufferDesc.CPUAccessFlags = 0;
  idxBufferDesc.MiscFlags = 0;
  idxBufferDesc.StructureByteStride = 0;

  idxData.pSysMem = indices;
  idxData.SysMemPitch = 0;
  idxData.SysMemSlicePitch = 0;

  hr = device->CreateBuffer(&idxBufferDesc, &idxData, &m_idxBuffer);

  if (FAILED(hr)) return false;

  // release arrays
  delete[] vertices;
  vertices = 0;
  delete[] indices;
  indices = 0;

  return true;
}

void Terrain::ShutdownBuffers()
{
  SafeRelease(m_vertexBuffer);
  SafeRelease(m_idxBuffer);
}

void Terrain::RenderBuffers(ID3D11DeviceContext *device_context)
{
  unsigned int stride = sizeof(Vertex_T), 
               offset = 0;

  device_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
  device_context->IASetIndexBuffer(m_idxBuffer, DXGI_FORMAT_R32_UINT, 0);
  device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Terrain::LoadSetupFile(char *setup_filename)
{
  std::ifstream ifs;
  char input;

  ifs.open(setup_filename);
  
  if (ifs.fail()) return false;
  
  ifs.get(input);
  while (input != ':')
    ifs.get(input);

  ifs >> m_terrainHeight;

  ifs.get(input);
  while (input != ':')
    ifs.get(input);

  ifs >> m_terrainWidth;

  ifs.get(input);
  while (input != ':')
    ifs.get(input);

  ifs >> m_heightScale;

  ifs.get(input);
  while (input != ':')
    ifs.get(input);

  ifs >> m_terrainRoughness;

  ifs.get(input);
  while (input != ':')
    ifs.get(input);

  ifs >> m_maxRandom;

  ifs.close();

  return true;
}

bool Terrain::LoadHeightMap()
{
  Fractal fractal_heightmap;
  if (!fractal_heightmap.Initialize(m_terrainWidth, m_terrainHeight, m_maxRandom, m_terrainRoughness))
    return false;

  if (!fractal_heightmap.Generate())
    return false;

  m_heightMap = new HeightMap_T[m_terrainWidth*m_terrainHeight];
  if (!m_heightMap) return false;

  int i, j;
  for (j = 0; j < m_terrainHeight; ++j)
  {
    for (i = 0; i < m_terrainWidth; ++i)
    {
      m_heightMap[j*m_terrainWidth + i].y = fractal_heightmap(j, i);
    }
  }

  fractal_heightmap.Release();

  return true;
}

void Terrain::ShutdownHeightMap()
{
  if (m_heightMap)
  {
    delete[] m_heightMap;
    m_heightMap = 0;
  }
}

void Terrain::SetTerrainCoordinates()
{
  int i, j, idx;

  for (j = 0; j < m_terrainHeight; ++j)
  {
    for (i = 0; i < m_terrainWidth; ++i)
    {
      idx = j*m_terrainWidth + i;
      m_heightMap[idx].x = (float)i;
      m_heightMap[idx].z = m_terrainHeight - 1 -(float)j;

      m_heightMap[idx].y /= m_heightScale;
    }
  }
}

bool Terrain::CalculateNormals()
{
  Vector_T* normals = new Vector_T[(m_terrainHeight - 1)*(m_terrainWidth - 1)];
  if (!normals)  return false;

  int i, j, idx, bottom_left, bottom_right, top_left;
  float u[3], v[3], magnitude;

  // calculate normals for each 
  for (j = 0; j < m_terrainHeight - 1; ++j)
  {
    for (i = 0; i < m_terrainWidth - 1; ++i)
    {
      bottom_left = (j + 1)*m_terrainHeight + i;
      bottom_right = (j + 1)*m_terrainHeight + (i + 1);
      top_left = j*m_terrainHeight + i;

      u[0] = m_heightMap[bottom_left].x - m_heightMap[top_left].x;
      u[1] = m_heightMap[bottom_left].y - m_heightMap[top_left].y;
      u[2] = m_heightMap[bottom_left].z - m_heightMap[top_left].z;

      v[0] = m_heightMap[top_left].x - m_heightMap[bottom_right].x;
      v[1] = m_heightMap[top_left].y - m_heightMap[bottom_right].y;
      v[2] = m_heightMap[top_left].z - m_heightMap[bottom_right].z;

      // u X v to get normal
      idx = j*(m_terrainWidth - 1) + i;
      normals[idx].x = u[1] * v[2] - u[2] * v[1];
      normals[idx].y = u[2] * v[0] - u[0] * v[2];
      normals[idx].z = u[0] * v[1] - u[1] * v[0];

      // normalize normal
      magnitude = sqrt(normals[idx].x*normals[idx].x + normals[idx].y*normals[idx].y + normals[idx].z*normals[idx].z);
      normals[idx].x /= magnitude;
      normals[idx].y /= magnitude;
      normals[idx].z /= magnitude;
    }
  }

  // calculate shared normals
  float sum[3];
  for (j = 0; j < m_terrainHeight; ++j)
  {
    for (i = 0; i < m_terrainWidth; ++i)
    {
      sum[0] = 0.f;
      sum[1] = 0.f;
      sum[2] = 0.f;

      // bottom left - check if it's top left corner
      if (i - 1 >= 0 && j - 1 >= 0)
      {
        idx = (j - 1)*(m_terrainWidth-1)+(i - 1);
        sum[0] += normals[idx].x;
        sum[1] += normals[idx].y;
        sum[2] += normals[idx].z;
      }

      // bottom right - check if it's top right corner
      if (i < m_terrainWidth - 1 && j - 1 >= 0)
      {
        idx = (j - 1)*(m_terrainWidth-1) + i;
        sum[0] += normals[idx].x;
        sum[1] += normals[idx].y;
        sum[2] += normals[idx].z;
      }

      // top left - check if it's bottom left corner
      if (i - 1 >= 0 && j < m_terrainHeight - 1)
      {
        idx = j*(m_terrainWidth-1) + i-1;
        sum[0] += normals[idx].x;
        sum[1] += normals[idx].y;
        sum[2] += normals[idx].z;
      }

      // top right - check if it's bottom right corner
      if (i < m_terrainWidth - 1 && j < m_terrainHeight - 1)
      {
        idx = j*(m_terrainWidth - 1) + i;
        sum[0] += normals[idx].x;
        sum[1] += normals[idx].y;
        sum[2] += normals[idx].z;
      }

      magnitude = (float)sqrt(sum[0] * sum[0] + sum[1] * sum[1] + sum[2] * sum[2]);

      // final shared normal
      idx = j*m_terrainWidth + i;
      m_heightMap[idx].nx = sum[0] / magnitude;
      m_heightMap[idx].ny = sum[1] / magnitude;
      m_heightMap[idx].nz = sum[2] / magnitude;
    }
  }

  delete[] normals;
  normals = 0;

  return true;
}

bool Terrain::BuildTerrainModel()
{
  m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;

  m_terrainModel = new Model_T[m_vertexCount];
  if (!m_terrainModel) return false;

  int idx = 0, i, j, top_left, top_right, bottom_left, bottom_right;

  for (j = 0; j < m_terrainHeight - 1; ++j)
  {
    for (i = 0; i < m_terrainWidth - 1; ++i)
    {
      top_left = m_terrainWidth*j + i;
      top_right = m_terrainWidth*j + (i + 1);
      bottom_left = m_terrainWidth*(j + 1) + i;
      bottom_right = m_terrainWidth*(j + 1) + (i + 1);

      // 2 triangles for 1 square
      // triangle1
      m_terrainModel[idx].x = m_heightMap[top_left].x;
      m_terrainModel[idx].y = m_heightMap[top_left].y;
      m_terrainModel[idx].z = m_heightMap[top_left].z;
      m_terrainModel[idx].tu = 0.f;
      m_terrainModel[idx].tv = 0.f;
      m_terrainModel[idx].nx = m_heightMap[top_left].nx;
      m_terrainModel[idx].ny = m_heightMap[top_left].ny;
      m_terrainModel[idx].nz = m_heightMap[top_left].nz;
      ++idx;

      m_terrainModel[idx].x = m_heightMap[top_right].x;
      m_terrainModel[idx].y = m_heightMap[top_right].y;
      m_terrainModel[idx].z = m_heightMap[top_right].z;
      m_terrainModel[idx].tu = 1.f;
      m_terrainModel[idx].tv = 0.f;
      m_terrainModel[idx].nx = m_heightMap[top_right].nx;
      m_terrainModel[idx].ny = m_heightMap[top_right].ny;
      m_terrainModel[idx].nz = m_heightMap[top_right].nz;
      ++idx;

      m_terrainModel[idx].x = m_heightMap[bottom_left].x;
      m_terrainModel[idx].y = m_heightMap[bottom_left].y;
      m_terrainModel[idx].z = m_heightMap[bottom_left].z;
      m_terrainModel[idx].tu = 0.f;
      m_terrainModel[idx].tv = 1.f;
      m_terrainModel[idx].nx = m_heightMap[bottom_left].nx;
      m_terrainModel[idx].ny = m_heightMap[bottom_left].ny;
      m_terrainModel[idx].nz = m_heightMap[bottom_left].nz;
      ++idx;

      // triangle2
      m_terrainModel[idx].x = m_heightMap[bottom_left].x;
      m_terrainModel[idx].y = m_heightMap[bottom_left].y;
      m_terrainModel[idx].z = m_heightMap[bottom_left].z;
      m_terrainModel[idx].tu = 0.f;
      m_terrainModel[idx].tv = 1.f;
      m_terrainModel[idx].nx = m_heightMap[bottom_left].nx;
      m_terrainModel[idx].ny = m_heightMap[bottom_left].ny;
      m_terrainModel[idx].nz = m_heightMap[bottom_left].nz;
      ++idx;

      m_terrainModel[idx].x = m_heightMap[top_right].x;
      m_terrainModel[idx].y = m_heightMap[top_right].y;
      m_terrainModel[idx].z = m_heightMap[top_right].z;
      m_terrainModel[idx].tu = 1.f;
      m_terrainModel[idx].tv = 0.f;
      m_terrainModel[idx].nx = m_heightMap[top_right].nx;
      m_terrainModel[idx].ny = m_heightMap[top_right].ny;
      m_terrainModel[idx].nz = m_heightMap[top_right].nz;
      ++idx;

      m_terrainModel[idx].x = m_heightMap[bottom_right].x;
      m_terrainModel[idx].y = m_heightMap[bottom_right].y;
      m_terrainModel[idx].z = m_heightMap[bottom_right].z;
      m_terrainModel[idx].tu = 1.f;
      m_terrainModel[idx].tv = 1.f;
      m_terrainModel[idx].nx = m_heightMap[bottom_right].nx;
      m_terrainModel[idx].ny = m_heightMap[bottom_right].ny;
      m_terrainModel[idx].nz = m_heightMap[bottom_right].nz;
      ++idx;
    }
  }

  return true;
}

void Terrain::ShutdownTerrainModel()
{
  if (m_terrainModel) 
  {
    delete[] m_terrainModel;
    m_terrainModel = 0;
  }
}
