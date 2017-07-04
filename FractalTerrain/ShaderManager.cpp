#include "ShaderManager.h"

using namespace DirectX;

ShaderManager::ShaderManager():
  m_colorShader(nullptr),
  m_fontShader(nullptr)
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
  
  m_fontShader = new FontShader;
  if (!m_fontShader) return false;

  if (!m_fontShader->Initialize(device, hwnd))
    return false;

  return true;
}

bool ShaderManager::RenderColorShader(ID3D11DeviceContext *device_context, int idx_count, XMMATRIX& world_mat, XMMATRIX& view_mat, XMMATRIX& proj_mat)
{
  return m_colorShader->Render(device_context, idx_count, world_mat, view_mat, proj_mat);
}

bool ShaderManager::RenderFontShader(ID3D11DeviceContext *device_context, int idx_count, const XMMATRIX& world_mat, const XMMATRIX& view_mat, const XMMATRIX& proj_mat, ID3D11ShaderResourceView *texture, XMFLOAT4 color)
{
  return m_fontShader->Render(device_context, idx_count, world_mat, view_mat, proj_mat, texture, color);
}

void ShaderManager::Shutdown()
{
  SafeShutdown(m_colorShader);
  SafeShutdown(m_fontShader);
}
