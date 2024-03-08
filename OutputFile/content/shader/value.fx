#ifndef _VALUE
#define _VALUE

#include "struct.fx"

#define PI 3.14159265358979f

// register 크기 제한 4096
cbuffer TRANSFORM : register(b0)
{
    row_major Matrix g_matWorld;
    row_major Matrix g_matWorldInv;

    row_major Matrix g_matView;
    row_major Matrix g_matViewInv;

    row_major Matrix g_matProj;
    row_major Matrix g_matProjInv;

    row_major Matrix g_matWV;
    row_major Matrix g_matWVP;
}

// 그냥 int arr[4]로 설정할 경우, 16바이트로 설정되어 있기 때문에
// 4바이트 배열 4개를 만드는 것이 아닌 16바이트 배열 4개를 만든다
cbuffer MATERIAL_CONST : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
    
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
   
    int g_btexcube_0;
    int g_btexcube_1;
    
    int g_btexarr_0;
    int g_btexarr_1;
}

cbuffer ANIM_DATA2D : register(b2)
{
    float2  g_vLeftTop;
    float2  g_vSliceSize;
    float2  g_vOffset;
    float2  g_vBackground;
    int     g_UseAnim2D;
    float3  padding;
}

cbuffer GLOBAL_DATA : register(b3)
{
    float2  g_RenderResolution;
    float   g_dt;
    float   g_time;
    int     g_Light2DCount;
    int     g_Light3DCount;
    float2  g_vPadding;
}

// register(t0)에 등록된 텍스처 객체
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);

TextureCube g_texcube_0 : register(t6);
TextureCube g_texcube_1 : register(t7);

Texture2DArray g_texarr_0 : register(t8);
Texture2DArray g_texarr_1 : register(t9);

Texture2D g_anim2d_tex : register(t10);

StructuredBuffer<tLightInfo> g_Light2D : register(t11);
StructuredBuffer<tLightInfo> g_Light3D : register(t12);

Texture2D g_postprocess : register(t13);

Texture2D g_NoiseTex : register(t14);


// register(s0)에 등록된 샘플러
// 샘플링은 텍스처에서 특정 위치의 값을 읽어오는 과정으로
// 이미지 데이터를 저장하고 있는 텍스처를 텍스처 좌표(일반적으로 UV좌표)를 사용하여
// 이미지의 특정 위치에 해당하는 색상 값을 가져오는 과정이다
SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);


#endif