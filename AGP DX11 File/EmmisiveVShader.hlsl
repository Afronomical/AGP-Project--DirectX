
#define MAX_POINT_LIGHTS 8
struct VS_Input
{
    float3 position : POSITION;
    float4 colour : COLOR;
    float2 uv : TEXCOORD;
    float3 norm : NORMAL;
};
struct VS_Output
{
    float4 position : SV_Position;
    float4 colour : COLOR;
    float2 uv : TEXCOORD;
};
struct PointLight
{
    float4 position;
    float4 colour;

    float strength;
    bool enabled;
    float2 padding;
};

cbuffer CBUFFER0
{
    matrix WVP; // 64 bytes  
    matrix WV; //only change
    float4 ambientLightCol;
    float4 directionalLightDir;
    float4 directionalLightCol;

    float4 pointLightPos;
    float4 pointLightCol;
    float pointLightStrength;
    PointLight pointLights[MAX_POINT_LIGHTS];

};

VS_Output main(VS_Input input)
{
    VS_Output output;
    //Position
    output.position = mul(WVP, float4(input.position, 1));
    //Texture coords
    output.uv = input.uv;

    
   
    //Final colour
    output.colour = 1;
    return output;

}