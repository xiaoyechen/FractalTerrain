#include "ShaderManager.h"

using namespace DirectX;

ShaderManager::ShaderManager():
  m_colorShader(nullptr),
  m_textureShader(nullptr)
{
}


ShaderManager::~ShaderManager()
{
}

bool ShaderManager::Initialize(ID3D11Device *device, HWND hwnd)
{
  m_colorShader = new ColorShader;
  if (!m_colorShader) return false;

  if (!m_colorShader->Initialize(device, hwnd))
    return false;
  
  m_textureShader = new TextureShader;
  if (!m_textureShader)  return false;

  if (!m_textureShader->Initialize(device, hwnd))
    return false;

  return true;
}

bool ShaderManager::RenderColorShader(ID3D11DeviceContext *device_context, int idx_count, XMMATRIX& world_mat, XMMATRIX& view_mat, XMMATRIX& proj_mat)
{
  return m_colorShader->Render(device_context, idx_count, world_mat, view_mat, proj_mat);
}

bool ShaderManager::RenderTextureShader(ID3D11DeviceContext *device_context, int idx_count, XMMATRIX &world_mat, XMMATRIX &view_mat, XMMATRIX &proj_mat, ID3D11ShaderResourceView *texture)
{
  return m_textureShader->Render(device_context, idx_count, world_mat, view_mat, proj_mat, texture);
}


void ShaderManager::Shutdown()
{
  SafeShutdown(m_colorShader);
  SafeShutdown(m_textureShader);
}
