#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

// mesh : RectMesh
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


// GrayFilter
VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    //// 로컬 스페이스에 2배를 하면 전체화면의 해상도와 같다
    //output.vPosition = float4(_in.vPos * 2.f, 1.f);
    //output.vUV = _in.vUV;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    
    float aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    
    vColor.rgb = float3(aver, aver, aver);
    
    return vColor;
}

// Distortion
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    // 픽셀 셰이더에 SV_Position으로 입력된 값은 픽셀 셰이더를 호출한 해당 픽셀의 좌표가 들어있음
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    ////                                  시간      속도 / 주파수 / 진폭
    //vScreenUV.y += cos((vScreenUV.x + (g_time * 0.1f)) * 3.f) * 0.1f;
    
    if (g_btex_0)
    {
        float vUV = _in.vUV;
        vUV.x += g_time * 0.1f;
        
        float2 vNoise = g_tex_0.Sample(g_sam_0, vUV);
        vNoise = (vNoise.xy - 0.5f) * 0.03f;
 
        vScreenUV += vNoise;
    }
    
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    
    return vColor;
}

// VCRDistortion
VS_OUT VS_VCRDistortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 로컬 스페이스에 2배를 하면 전체화면의 해상도와 같다
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
    
    //VS_OUT output = (VS_OUT) 0.f;
    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    //output.vUV = _in.vUV;
    
    //return output;
}

float mod(float _m, float _n)
{
    float result = _m % _n;

    return result;
}

float noise(float2 p)
{    
    float s = g_tex_0.Sample(g_sam_0, p);
    return s;
}


float onOff(float a, float b, float c)
{
    return step(c, sin(g_time + a * cos(g_time * b)));
}

float ramp(float y, float start, float end)
{
    float inside = step(start, y) - step(end, y);
    float fact = (y - start) / (end - start) * inside;
    return (1.f - fact) * inside;
}


float4 PS_VCRDistortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    float2 vScreenUV = /* _in.vUV;*/_in.vPosition.xy / g_RenderResolution;

    float vUV = _in.vUV;
        
    float2 vNoise = g_tex_0.Sample(g_sam_0, vUV);
    vNoise = (vNoise.xy - 0.5f) * 0.03f;

    vScreenUV -= float2(0.5f, 0.5f);
    vScreenUV = vScreenUV * 1.2f * (1.f / 1.2f + 2.f * vScreenUV.x * vScreenUV.x * vScreenUV.y * vScreenUV.y);
    vScreenUV += float2(0.5f, 0.5f);
        
    // Apply stripes
    float noi = g_tex_0.Sample(g_sam_0, vScreenUV * float2(0.5f, 1.f) + float2(1.f, 3.f)).r;
    float stripesEffect = ramp(mod(vScreenUV.y * 1.5f + g_time / 2.f + sin(g_time + sin(g_time * 0.6f)), 1.f), 0.5f, 0.6f) * noi;
        
    // Get video color
    float2 look = vNoise;
    float window = 1.f / (1.f + 20.f * (look.y - mod(g_time / 4.f, 1.f)) * (look.y - mod(g_time / 4.f, 1.f)));
    look.x = look.x + sin(look.y * 10.f + g_time) / 50.f * onOff(4.f, 4.f, 0.9f) * (1.f + cos(g_time * 80.f)) * window;
    float vShift = 0.4f * onOff(2.f, 3.f, 0.9f) * (sin(g_time) * sin(g_time * 2.f) + (0.5f + 0.1f * sin(g_time * 1.f) * cos(g_time)));
    look.y = mod(look.y + vShift, 1.f);
    float2 video = g_tex_0.Sample(g_sam_0, look);
        
    // Apply noise
    float noiseEffect = noise(vScreenUV * 2.0) * 0.5;

    // Apply vignette
    float vigAmt = 10.f + 0.3f * sin(g_time + 5.f * cos(g_time * 5.f));
    float vignette = (1.f - vigAmt * (vScreenUV.y - 0.5f)) * (vScreenUV.x - 0.5);

    // Final color calculation
    float finalColor = video + stripesEffect + noiseEffect;
    finalColor *= vignette;
    finalColor *= (12.0 + mod(vScreenUV.y * 30.0 + g_time, 1.0)) / 13.0;

    vScreenUV.y += saturate(sin(vigAmt * g_time * 0.1f)) * onOff(2.f, 3.f, 0.9f);
        
    vScreenUV += (stripesEffect * finalColor);
    
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    
    float aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    
    vColor.rgb = float3(aver, aver, aver);
    
    return vColor;
}


// Outline
VS_OUT VS_Outline(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
    
    //VS_OUT output = (VS_OUT) 0.f;
    
    //// 로컬 스페이스에 2배를 하면 전체화면의 해상도와 같다
    //output.vPosition = float4(_in.vPos * 2.f, 1.f);
    //output.vUV = _in.vUV;
    
    //return output;
}


float4 PS_Outline(VS_OUT _in) : SV_Target
{    
    float Thick = 0.003f;
    
    float4 vColor = (float4) 0.f;
    
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    
    float3 vColorUp = g_postprocess.Sample(g_sam_0, vScreenUV - float2(0.f, Thick));
    float3 vColorDown = g_postprocess.Sample(g_sam_0, vScreenUV + float2(0.f, Thick));
    float3 vColorLeft = g_postprocess.Sample(g_sam_0, vScreenUV - float2(Thick, 0.f));
    float3 vColorRight = g_postprocess.Sample(g_sam_0, vScreenUV + float2(Thick, 0.f));
    
    float fColor = (vColor.r + vColor.g + vColor.b) / 3.f;
    float fColorUp = fColor - ((vColorUp.r + vColorUp.g + vColorUp.b) / 3.f);
    float fColorDown = fColor - ((vColorDown.r + vColorDown.g + vColorDown.b) / 3.f);
    float fColorLeft = fColor - ((vColorLeft.r + vColorLeft.g + vColorLeft.b) / 3.f);
    float fColorRight = fColor - ((vColorRight.r + vColorRight.g + vColorRight.b) / 3.f);

    
    if (fColorUp > 0.09f || fColorDown > 0.09f || fColorLeft > 0.09f || fColorRight > 0.09f)
    {
        vColor = float4(1.f, 0.f, 0.f, 1.f);
    }
    
    return vColor;
}



#endif
