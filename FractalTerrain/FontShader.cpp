#include "FontShader.h"

using namespace DirectX;

FontShader::FontShader()
{
}


FontShader::~FontShader()
{
}

bool FontShader::Initialize(ID3D11Device *device, HWND hwnd)
{
  return true;
}

void FontShader::Shutdown()
{
}

bool FontShader::Render(ID3D11DeviceContext *device_context, int idx_count, const XMMATRIX& world_mat, const XMMATRIX& view_mat, const XMMATRIX& proj_mat, ID3D11ShaderResourceView *texture, XMFLOAT4 color)
{
  return true;
}
