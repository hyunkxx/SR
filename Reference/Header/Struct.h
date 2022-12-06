 #ifndef Struct_h__
#define Struct_h__

BEGIN(Engine)

#pragma region ��������
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
	_vec3			vPos;			// ��ġ
	_ulong			dwColor;		// ����

}VTXCOL;

const _ulong  FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef struct tagVertex_TEXTURE
{
	_vec3			vPos;			// ��ġ
	_vec3			vNormal;		// ���� ����
	_vec2			vTexUV;			// �ؽ�ó uv��ǥ

}VTXTEX;

const _ulong  FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


typedef struct tagVertex_TEXTURECUBE
{
	_vec3			vPos;			// ��ġ
	_vec3			vTexUV;			// �ؽ�ó uv��ǥ

}VTXCUBE;

const _ulong  FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

// D3DFVF_TEXCOORDSIZE3 : �ؽ�ó�� UV���� FLOAT�� 3���� ũ�� ��ŭ�̸� ��ȣ ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؼ��� UV���� �������� �� �� �ִ� �� �� ù ��° �̹��� ���� �����ϰڴٴ� �ǹ�


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
	//�ְ�ӵ� ����
	_float				fAccel_top_speed;
	_float				fBack_top_speed;

	//���� ����, ���� �ִ� ����, ȸ���ӵ� ����
	_float				fPosinDist;
	_float				fLowAngle;
	_float				TopAngle;
	_float				RotSpeed;
	// ���� �ӵ�
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
