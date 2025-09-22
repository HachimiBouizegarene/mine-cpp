cbuffer CameraCB : register(b0)
{
    matrix proj;
    matrix view;
}


float4 main( float3 pos : POSITION ) : SV_POSITION
{
    float4 worldPos = float4(pos, 1.0f);
    worldPos = mul(worldPos, view);
    worldPos = mul(worldPos, proj);
    return worldPos;
}