#pragma once

#define SINGLE(classtype) private:\
							classtype();\
							~classtype();\
							friend class CSingleton<classtype>;

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define KEY_CHECK(Key, State) CKeyMgr::GetInst()->GetKeyState(KEY::Key) == KEY_STATE::State

#define KEY_TAP(Key)		KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key)	KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key)	KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key)		KEY_CHECK(Key, NONE)

#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define DT_ENGINE CTimeMgr::GetInst()->GetEngineDeltaTime()

#define LAYER_MAX 32

#define CLONE(Type) virtual Type* Clone() { return new Type(*this); }
#define CLONE_DISABLE(Type) Type* Clone() { return nullptr; assert(nullptr); }\
							Type(const Type& _OriginBuffer) = delete;

enum class ASSET_TYPE
{
	MESH,
	MESHDATA,
	PREFAB,
	TEXTURE,
	MATERIAL,
	SOUND,
	COMPUTE_SHADER,
	GRAPHICS_SHADER,
	FSM,
	END,
};

extern const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END];

enum class COMPONENT_TYPE
{
	TRANSFORM,	// 오브젝트 위치, 크기, 회전

	COLLIDER2D, // 2차원 충돌체
	COLLIDER3D, // 3차원 충돌체

	ANIMATOR2D, // 스트라이트 Animation
	ANIMATOR3D, // Bone Skinning Animation

	LIGHT2D,	// 2차원 광원
	LIGHT3D,	// 3차원 광원

	CAMERA,		// 카메라 기능

	STATEMACHINE, // 상태머신

	// Render Component
	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,

	END,

	SCRIPT,
};


enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
};

// 상수버퍼 종류
enum class CB_TYPE
{
	TRANSFORM,
	MATERIAL_CONST,
	ANIM2D_DATA,
	GLOBAL_DATA,
	ANIMATION,

	END,
};


// Rasterizer State Type
enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,

	END,
};

// DepthStencil State Type
enum class DS_TYPE
{
	LESS,				// 작은게 통과			깊이 기록 O
	LESS_EQUAL,			// 작거나 같으면 통과		깊이 기록 O

	GRATER,				// 더 멀면 통과			깊이 기록 O
	GRATER_EQUAL,		// 더 멀거나 같으면 통과	깊이 기록 O

	NO_TEST,			// 깊이 테스트 X			깊이 기록 O
	NO_WRITE,			// 깊이 테스트 O			깊이 기록 X

	NO_TEST_NO_WRITE,	// 깊이 테스트 X			깊이 기록 X

	END,
};

// Blend State
enum class BS_TYPE
{
	DEFAULT,

	ALPHA_BLEND,

	ONE_ONE,

	END,
};

// ScalarParam
enum class SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum class TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEXCUBE_0,
	TEXCUBE_1,

	TEXARR_0,
	TEXARR_1,

	END,
};

enum class SCRIPT_PARAM
{
	INT,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	OBJECT,
};

enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,
	CROSS,
	CUBE,
	SPHERE,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,			// 불투명
	DOMAIN_MASKED,			// 불투명 or 투명
	DOMAIN_TRANSPARENT,		// 반투명
	DOMAIN_POSTPROCESS,		// 후처리

	DOMAIN_DEBUG,
};


enum class PARTICLE_MODULE
{
	SPAWN,	// 파티클 생성 관련
	DRAG,	// 감속 관련 모듈
	SCALE,
	ADD_VELOCITY,
	NOISE_FORCE,
	CALCULATE_FORCE,

	RENDER,	// 렌더링 때의 옵션 관련

	END,
};


enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
	NONE,
};