
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
    matrix WV;  //only change
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

    //Ligthting
    float diffuseAmount = dot(directionalLightDir.xyz, input.norm);
    diffuseAmount = saturate(diffuseAmount);
    float3 directionalFinal = saturate(directionalLightCol * diffuseAmount);
    float3 pointFinal = float3(0, 0, 0);

    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        if (!pointLights[i].enabled)
            continue;

        //PointLighting
        float4 pointLightDir = normalize(pointLights[i].position - float4(input.position, 1));
        float pointLightDistance = length(pointLights[i].position.xyz - input.position);
        float pointLightAttenuation = pointLights[i].strength / (pointLightDistance * pointLightDistance + pointLights[i].strength);
        float pointAmount = dot(pointLightDir.xyz, input.norm) * pointLightAttenuation;
        pointAmount = saturate(pointAmount);
        pointFinal += pointLights[i].colour * pointAmount * 5;
        
    }
   
    //Final colour
    output.colour = saturate(saturate(ambientLightCol) + float4(directionalFinal, 1) + float4(pointFinal, 1));
    return output;

}