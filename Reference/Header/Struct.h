 #ifndef Struct_h__
#define Struct_h__

BEGIN(Engine)

#pragma region 복셀전용
typedef struct tagCubeData
{
	_vec3 scale;
	_vec3 rotation;
	_vec3 position;
	D3DXCOLOR originColor;

}CUBE_DATA;

struct Vertex
{
	_vec3 pos;
	D3DCOLOR color;
};
const _ulong FVF_TRI = D3DFVF_XYZ | D3DFVF_DIFFUSE;

struct Index32
{
	_ulong _0;
	_ulong _1;
	_ulong _2;
};
const _ulong FVF_VOX = D3DFVF_XYZ | D3DFVF_DIFFUSE;

struct Rotation
{
	float x;
	float y;
	float z;

	Rotation(_vec3 v) : x(v.x), y(v.y), z(v.z)
	{}
};
#pragma endregion

typedef struct tagVertex_COLOR
{
	_vec3			vPos;			// 위치
	_ulong			dwColor;		// 색상

}VTXCOL;

const _ulong  FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef struct tagVertex_TEXTURE
{
	_vec3			vPos;			// 위치
	_vec3			vNormal;		// 법선 벡터
	_vec2			vTexUV;			// 텍스처 uv좌표

}VTXTEX;

const _ulong  FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


typedef struct tagVertex_TEXTURECUBE
{
	_vec3			vPos;			// 위치
	_vec3			vTexUV;			// 텍스처 uv좌표

}VTXCUBE;

const _ulong  FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

// D3DFVF_TEXCOORDSIZE3 : 텍스처의 UV값이 FLOAT형 3개의 크기 만큼이며 괄호 안의 숫자 0의 의미는 본래 버텍스의 텍서츠 UV값이 여러개가 올 수 있는 그 중 첫 번째 이미지 값을 지정하겠다는 의미


typedef	struct tagIndex16
{
	_ushort			_0;
	_ushort			_1;
	_ushort			_2;

}INDEX16;

typedef	struct tagIndex32
{
	_ulong			_0;
	_ulong			_1;
	_ulong			_2;

}INDEX32;

struct tagKey
{
	KEY_STATE eKeyState;
	bool bPrevPush;
	float Tap_Count;
	float Double_Count;
};

typedef struct RayINFO
{
	_vec3 Pos;
	_vec3 Dir;
}MYRAY;

typedef struct TANK_STATE
{
	//HP
	_float				fMaxHP;
	_float				fCurHP;

	_float				fSpeed = 0.f;;
	_float				fAccum = 0.f;
	_bool				bAdvance = false;
	_bool				bBack = false;

	_float				fAccel_Ad;
	_float				fAccel_Back;
	//최고속도 제한
	_float				fAccel_top_speed;
	_float				fBack_top_speed;

	//포신 길이, 포신 최대 각도, 회전속도 설정
	_float				fPosinDist;
	_float				fLowAngle;
	_float				TopAngle;
	_float				RotSpeed;
	// 장전 속도
	_float				fReload;
	_float				fReloadTime;
	_int				iCannonSpeed;

	std::wstring		strName;
	VEHICLE				eID;
}Tank_State;

typedef struct TAG_OBB
{
	_vec3  vPos;
	_vec3  vDir[OBB_END];
	_float fLen[OBB_END];
}OBB;


END


#endif // Struct_h__
