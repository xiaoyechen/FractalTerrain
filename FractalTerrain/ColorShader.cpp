#include "ColorShader.h"

using namespace DirectX;

ColorShader::ColorShader():
  m_vertexShader(nullptr),
  m_pixelShader(nullptr),
  m_layout(nullptr),
  m_matrixBuffer(nullptr)
{
}


ColorShader::~ColorShader()
{
}

bool ColorShader::Initialize(ID3D11Device *device, HWND hwnd)
{
  return InitializeShader(device, hwnd, L"ColorVertexShader.vs", L"ColorPixelShader.ps");
}

void ColorShader::Shutdown()
{
  ShutdownShader();
}

bool ColorShader::Render(ID3D11DeviceContext *device_context, int idx_count, XMMATRIX& world_mat, XMMATRIX& view_mat, XMMATRIX& proj_mat)
{
  if (!SetShaderParams(device_context, world_mat, view_mat, proj_mat))
    return false;

  RenderShader(device_context, idx_count);

  return true;
}

bool ColorShader::InitializeShader(ID3D11Device *device, HWND hwnd, WCHAR *vertexshader_path, WCHAR *pixelshader_path)
{
  HRESULT hr;
  ID3D10Blob* err_msg = 0;
  ID3D10Blob* vertexshader_buffer = 0;
  ID3D10Blob* pixelshader_buffer = 0;

  hr = D3DCompileFromFile(vertexshader_path, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexshader_buffer, &err_msg);
  if (FAILED(hr))
  {
    if (err_msg)
      OutputShaderError(err_msg, hwnd, vertexshader_path);
    else
      MessageBox(hwnd, vertexshader_path, L"Shader File Not Found", MB_OK);
  
    return false;
  }

  hr = D3DCompileFromFile(pixelshader_path, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelshader_buffer, &err_msg);
  if (FAILED(hr))
  {
    if (err_msg)
      OutputShaderError(err_msg, hwnd, pixelshader_path);
    else
      MessageBox(hwnd, pixelshader_path, L"Shader File Not Found", MB_OK);

    return false;
  }

  hr = device->CreateVertexShader(vertexshader_buffer->GetBufferPointer(), vertexshader_buffer->GetBufferSize(), NULL, &m_vertexShader);
  if (FAILED(hr)) return false;

  hr = device->CreatePixelShader(pixelshader_buffer->GetBufferPointer(), pixelshader_buffer->GetBufferSize(), NULL, &m_pixelShader);
  if (FAILED(hr)) return false;


  D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
  
  polygon_layout[0].SemanticName = "POSITION";
  polygon_layout[0].SemanticIndex = 0;
  polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygon_layout[0].InputSlot = 0;
  polygon_layout[0].AlignedByteOffset = 0;
  polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygon_layout[0].InstanceDataStepRate = 0;

  polygon_layout[1].SemanticName = "COLOR";
  polygon_layout[1].SemanticIndex = 0;
  polygon_layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  polygon_layout[1].InputSlot = 0;
  polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygon_layout[1].InstanceDataStepRate = 0;

  unsigned int num_elem = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

  hr = device->CreateInputLayout(polygon_layout, num_elem, vertexshader_buffer->GetBufferPointer(), vertexshader_buffer->GetBufferSize(), &m_layout);
  if (FAILED(hr))  return false;

  vertexshader_buffer->Release();
  vertexshader_buffer = 0;
  pixelshader_buffer->Release();
  pixelshader_buffer = 0;


  D3D11_BUFFER_DESC matrixbuffer_desc;
  matrixbuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
  matrixbuffer_desc.ByteWidth = sizeof(MatrixBuffer_T);
  matrixbuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixbuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixbuffer_desc.MiscFlags = 0;
  matrixbuffer_desc.StructureByteStride = 0;

  hr = device->CreateBuffer(&matrixbuffer_desc, NULL, &m_matrixBuffer);
  if (FAILED(hr))  return false;

  return true;
}

void ColorShader::ShutdownShader()
{
  SafeRelease(m_matrixBuffer);
  SafeRelease(m_layout);
  SafeRelease(m_pixelShader);
  SafeRelease(m_vertexShader);
}

void ColorShader::OutputShaderError(ID3D10Blob *msg, HWND hwnd, WCHAR *filepath)
{
  char* compile_errors = (char*)msg->GetBufferPointer();
  int buffer_size = msg->GetBufferSize();
  
  std::ofstream fout;
  fout.open("shader_error.txt");

  for (int i = 0; i < buffer_size; ++i)
  {
    fout << compile_errors[i];
  }
  fout.close();

  msg->Release();
  msg = 0;

  MessageBox(hwnd, L"Error compiling shader. Check shader_error.txt for message.", filepath, MB_OK);
}

bool ColorShader::SetShaderParams(ID3D11DeviceContext *device_context, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &projection)
{
  // transpose before sending to shader
  world = XMMatrixTranspose(world);
  view = XMMatrixTranspose(view);
  projection = XMMatrixTranspose(projection);

  D3D11_MAPPED_SUBRESOURCE mapped_resource;
  MatrixBuffer_T* data_ptr;

  // lock the constant buffer
  if (FAILED(device_context->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource)))
    return false;

  data_ptr = (MatrixBuffer_T*)mapped_resource.pData;

  data_ptr->world = world;
  data_ptr->view = view;
  data_ptr->projection = projection;

  // unlock the constant buffer
  device_context->Unmap(m_matrixBuffer, 0);

  device_context->VSSetConstantBuffers(0, 1, &m_matrixBuffer);
  
  return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext *device_context, int idx_count)
{
  device_context->IASetInputLayout(m_layout);

  device_context->VSSetShader(m_vertexShader, NULL, 0);
  device_context->PSSetShader(m_pixelShader, NULL, 0);

  device_context->DrawIndexed(idx_count, 0, 0);
}
