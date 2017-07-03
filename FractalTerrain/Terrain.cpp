#include "Terrain.h"


Terrain::Terrain():
  m_vertexBuffer(nullptr),
  m_idxBuffer(nullptr)
{
}


Terrain::~Terrain()
{
}

bool Terrain::Initialize(ID3D11Device *device)
{
  if (InitializeBuffers(device))
    return true;
  else
    return false;
}

void Terrain::Shutdown()
{
  ShutdownBuffers();
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

bool Terrain::InitializeBuffers(ID3D11Device *device)
{

  D3D11_BUFFER_DESC vertexBufferDesc, idxBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, idxData;
  HRESULT hr;

  int terrain_w = 256, 
      terrain_h = 256;

  XMFLOAT4 color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

  m_vertexCount = (terrain_w - 1) * (terrain_h - 1) * 8;
  m_idxCount = m_vertexCount;

  Vertex_T* vertices = new Vertex_T[m_vertexCount];
  if (!vertices) return false;

  unsigned long* indices = new unsigned long[m_idxCount];
  if (!indices) return false;

  int idx = 0;

  float pos_x, pos_z;
  for (int j = 0; j < terrain_h - 1; ++j)
  {
    for (int i = 0; i < terrain_w - 1; ++i)
    {
      // line1 upper left
      pos_x = (float)i;
      pos_z = (float)(j + 1);

      vertices[idx].pos = XMFLOAT3(pos_x, 0.f, pos_z);
      vertices[idx].color = color;
      indices[idx] = idx;
      ++idx;

      // line1 upper right
      pos_x = (float)(i + 1);
      pos_z = (float)(j + 1);

      vertices[idx].pos = XMFLOAT3(pos_x, 0.f, pos_z);
      vertices[idx].color = color;
      indices[idx] = idx;
      ++idx;

      // line2 upper right (same as previous
      vertices[idx].pos = XMFLOAT3(pos_x, 0.f, pos_z);
      vertices[idx].color = color;
      indices[idx] = idx;
      ++idx;

      // line2 bottom right
      pos_x = (float)(i + 1);
      pos_z = (float)j;

      vertices[idx].pos = XMFLOAT3(pos_x, 0.f, pos_z);
      vertices[idx].color = color;
      indices[idx] = idx;
      ++idx;

      // line3 bottom right (same as previous
      vertices[idx].pos = XMFLOAT3(pos_x, 0.f, pos_z);
      vertices[idx].color = color;
      indices[idx] = idx;
      ++idx;

      // line3 bottom left
      pos_x = (float)i;
      pos_z = (float)j;

      vertices[idx].pos = XMFLOAT3(pos_x, 0.f, pos_z);
      vertices[idx].color = color;
      indices[idx] = idx;
      ++idx;

      // line4 bottom left (same as previous
      vertices[idx].pos = XMFLOAT3(pos_x, 0.f, pos_z);
      vertices[idx].color = color;
      indices[idx] = idx;
      ++idx;

      // line4 upper left
      pos_x = (float)i;
      pos_z = (float)(j + 1);

      vertices[idx].pos = XMFLOAT3(pos_x, 0.f, pos_z);
      vertices[idx].color = color;
      indices[idx] = idx;
      ++idx;
    }
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
  device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
