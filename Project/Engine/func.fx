#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "value.fx"

void CalLight2D(float3 _WorldPos, int _LightIdx, inout tLightColor _output)
{
    // 빛을 적용시킬 광원의 정보
    tLightInfo info = g_Light2D[_LightIdx];
    
    // Directional
    if(0 == info.LightType)
    {
        _output.vAmbient += info.Color.vAmbient;
    }
    // Point
    else if (1 == info.LightType)
    {
        float fAttenu = 1.f;
        
        float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);
        
        if(fDist < info.fRadius)
        {
            // 현재 픽셀에서 광원까지의 거리를 광원의 반지름으로 나눠 비율을 구하고
            // (PI / 2.f)를 곱함으로써 해당 값을 각도로 변환한다
            float fTheta = (fDist / info.fRadius) * (PI / 2.f);
            fAttenu = saturate(cos(fTheta));
        }
        else
        {
            fAttenu = saturate(1.f - fDist / g_Light2D[0].fRadius);
        }
        
        _output.vColor += info.Color.vColor * fAttenu;
    }
    // Spot
    else
    {
        float fAttenu = 1.f;    // 각도 감쇠
        float fAttenu2 = 1.f;   // 거리 감쇠
        
        float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);
        
        float2 lightDir = normalize(info.vWorldDir.xy);                 // 초기 광원의 방향 벡터
        float2 targetDir = normalize(_WorldPos.xy - info.vWorldPos.xy); // 타겟 벡터
        
        
        // 광원 방향과 타겟 방향 사이의 각도를 계산
        float fTheta = acos(dot(lightDir, targetDir));
        
        // 타겟 각도가 광원의 각도 범위 안에 있을 때
        if (fTheta < info.fAngle)
        {
            // 각도에 따른 감쇠 계산
            fAttenu = saturate(1.f - fTheta / info.fAngle);
            
            if (fDist < info.fRadius)
            {            
                float fTheta2 = (fDist / info.fRadius) * (PI / 2.f);
                fAttenu2 = saturate(cos(fTheta2));
            }
            else
            {
                fAttenu2 = saturate(1.f - fDist / g_Light2D[0].fRadius);
            }
        }
        // 타겟 각도가 광원 각도 범위 밖이면 보이지 않음
        else
        {
            fAttenu = 0.f;
        }
        
        _output.vColor += info.Color.vColor * fAttenu * fAttenu2;
    }
}

#endif