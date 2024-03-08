#ifndef _STRUCT
#define _STRUCT

struct tLightColor
{
    float4 vColor;      // 광원의 순수 색상
    float4 vSpecular;   // 반사광 - 광원이 물체 표면에 반사된 색상
    float4 vAmbient;    // 환경광(주변광) - 광원에 의해서 보장되는 최소한의 빛
};

struct tLightInfo
{
    tLightColor Color;

    float3  vWorldPos;  // 광원의 위치
    float3  vWorldDir;  // 광원이 향하는 방향
    float   fRadius;    // 광원의 영향 반경, 거리 정보
    float   fAngle;     // 광원의 범위 각도

    int     LightType;  // 광원 타입 (0 : Directional, 1 : Point, 2 : Spot)
	
    float3  vPadding;
};


struct tTileInfo
{
    float2  vLeftTopUV;
    int     bRender;
    int     padding;
};


struct tParticle
{
    float4 vLocalPos;
    float4 vWorldPos;       // 위치
    float4 vWorldInitScale; // 초기 크기
    float4 vWorldScale;     // 크기
    float4 vWorldRotation;  // 회전
    float3 vVelocity;       // 속도
    float4 vColor;          // 색상
    float4 vForce;
    
    float3 vNoiseForce;
    float NoiseForceTime;
    
    float NormalizeAge;
    float Mass;             // 질량
    float Age;              // 현재 나이
    float Life;             // 수명
    int Active;             // 활성화, 비활성화
};

struct tParticleModule
{
	// Spawn 모듈
    float4 vSpawnColor;     // 생성 시 컬러
    float4 vSpawnMinScale;  // 생성 시 최소 크기
    float4 vSpawnMaxScale;  // 생성 시 최대 크기
	
    float   MinMass;
    float   MaxMass;
    float   MinLife;        // 최소 수명
    float   MaxLife;        // 최대 수명
    int     SpawnRate;      // 초당 생성 개수
    int     SpaceType;      // 좌표계(0 : LocalSpace, 1 : WorldSpace)
    
    int     SpawnShape;     // 스폰 범위 (0 : Sphere, 1 : Box)
    float   Radius;         // 스폰쉐이프가 Sphere인 경우, 반지름 길이
    float4  vSpawnBoxScale; // 스폰쉐이프가 Box인 경우, Box의 크기
    
    // Add Velocity
    int     AddVelocityType;    // 0 : From Center, 1 : To Center, 2 : Fix Direction
    float   MinSpeed;
    float   MaxSpeed;
    float   FixedAngle;         // 해당 방향에서 랜덤범위 각도	
    float4  FixedDirection;     // 지정 방향
    
    // Scale
    float4  vScaleRatio;
    
    // Noise Force
    float   NoiseForceScale;
    float   NoiseForceTerm;
    
   	// Drag
    float   DragTime;
    
    int     VelocityAlignment; // 0 : Off, 1 : On
    int     AlphaBasedLife; // 0 : Off, 1 : NormalizeAge, 2 : Age
    float   AlphaMaxAge;
    
    int     arrModuleCheck[7];
    
    int3    modulepadding;
};

struct tSpawnCount
{
    int     iSpawnCount;
    int3    iPadding;
};

#endif