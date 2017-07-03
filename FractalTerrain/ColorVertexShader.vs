// globals
cbuffer MatrixBuffer
{
  matrix world;
  matrix view;
  matrix projection;
};

struct VertexInputType
{
  float4 position: POSITION;
  float4 color: COLOR;
};

struct PixelInputType
{
  float4 position: SV_POSITION;
  float4 color: COLOR;
};

// vertex shader method is called for every vertex
PixelInputType ColorVertexShader(VertexInputType input)
{
  PixelInputType output;

  input.position.w = 1.f;

  // transformation pipeline
  output.position = mul(input.position, world);
  output.position = mul(output.position, view);
  output.position = mul(output.position, projection);

  output.color = input.color;

  return output;
}