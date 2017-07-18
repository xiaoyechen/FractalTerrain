// globals
cbuffer MatrixBuffer
{
  matrix world;
  matrix view;
  matrix projection;
};

cbuffer CameraBuffer
{
  float3 camera_position;
  float padding;
};

struct VertexInputType
{
  float4 position: POSITION;
  float2 tex: TEXCOORD0;
  float3 normal: NORMAL;
};

struct PixelInputType
{
  float4 position: SV_POSITION;
  float2 tex: TEXCOORD0;
  float3 normal: NORMAL;
  float3 view_direction: TEXCOORD1;
};

PixelInputType LightVertexShader(VertexInputType input)
{
  PixelInputType output;

  input.position.w = 1.f;

  // transformation pipeline
  output.position = mul(input.position, world);
  output.position = mul(output.position, view);
  output.position = mul(output.position, projection);

  output.tex = input.tex;

  // calculate normal
  output.normal = mul(input.normal, (float3x3)world);
  output.normal = normalize(output.normal);

  // calculate viewing direction
  float4 world_position = mul(input.position, world);

  output.view_direction = camera_position.xyz - world_position.xyz;
  output.view_direction = normalize(output.view_direction);

  return output;
}