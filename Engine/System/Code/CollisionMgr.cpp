#include "..\..\Header\CollisionMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

_bool CCollisionMgr::Sphere_Collision(_vec3 First, _vec3 Second, _float first_range, _float Second_range)
{
	// First 와 Second 사이의 거리가 두개의 충돌 설정 반지름 합보다 작으면(충돌 했다고 판단) true 크면(충돌하지 않음) false 반환
	_vec3 Dir = First - Second;
	return sqrtf((sqrtf((Dir.x*Dir.x) + (Dir.z*Dir.z))*sqrtf((Dir.x*Dir.x) + (Dir.z*Dir.z))) + (Dir.y*Dir.y)) < first_range + Second_range;
}

_vec3 CCollisionMgr::Sphere_Collision_EX(_vec3 First, _vec3 Second, _float first_range, _float Second_range)
{
	// 밀어내야하는 값 반환
	if (Sphere_Collision(First, Second, first_range, Second_range))
	{
		_vec3 Dir = First - Second;
		_float fDifference = (first_range + Second_range) - sqrtf((sqrtf((Dir.x*Dir.x) + (Dir.z*Dir.z))*sqrtf((Dir.x*Dir.x) + (Dir.z*Dir.z))) + (Dir.y*Dir.y));
		return (*D3DXVec3Normalize(&Dir, &Dir)) * -fDifference;
	}
	return _vec3(0.f, 0.f, 0.f);
}

_bool CCollisionMgr::OBB_Collision(OBB* Box1, OBB* Box2)
{
	double c[3][3];
	double absC[3][3];
	double d[3];

	double r0, r1, r;

	const double cutoff = 0.999999;
	bool existsParallelPair = false;

	D3DXVECTOR3 diff = Box1->vPos - Box2->vPos;

	for (_int j = 0; j < 3; j++)
	{
		for (_int i = 0; i < 3; ++i)
		{
			absC[j][i] = abs(c[j][i] = D3DXVec3Dot(&Box1->vDir[j], &Box2->vDir[i]));
			if (absC[0][i] > cutoff)
				existsParallelPair = true;
		}
		r = abs(d[j] = D3DXVec3Dot(&diff, &Box1->vDir[j]));
		r0 = Box1->fLen[j];
		r1 = Box2->fLen[0] * absC[j][0] + Box2->fLen[1] * absC[j][1] + Box2->fLen[2] * absC[j][2];

		if (r > r0 + r1)
			return FALSE;
	}

	for (_int i = 0; i < 3; i++)
	{
		r = abs(D3DXVec3Dot(&diff, &Box2->vDir[i]));
		r0 = Box1->fLen[0] * absC[0][i] + Box1->fLen[1] * absC[1][i] + Box1->fLen[2] * absC[2][i];
		r1 = Box2->fLen[i];

		if (r > r0 + r1)
			return FALSE;
	}

	if (existsParallelPair == true)
		return TRUE;

	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = Box1->fLen[1] * absC[2][0] + Box1->fLen[2] * absC[1][0];
	r1 = Box2->fLen[1] * absC[0][2] + Box2->fLen[2] * absC[0][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = Box1->fLen[1] * absC[2][1] + Box1->fLen[2] * absC[1][1];
	r1 = Box2->fLen[0] * absC[0][2] + Box2->fLen[2] * absC[0][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = Box1->fLen[1] * absC[2][2] + Box1->fLen[2] * absC[1][2];
	r1 = Box2->fLen[0] * absC[0][1] + Box2->fLen[1] * absC[0][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = Box1->fLen[0] * absC[2][0] + Box1->fLen[2] * absC[0][0];
	r1 = Box2->fLen[1] * absC[1][2] + Box2->fLen[2] * absC[1][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = Box1->fLen[0] * absC[2][1] + Box1->fLen[2] * absC[0][1];
	r1 = Box2->fLen[0] * absC[1][2] + Box2->fLen[2] * absC[1][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = Box1->fLen[0] * absC[2][2] + Box1->fLen[2] * absC[0][2];
	r1 = Box2->fLen[0] * absC[1][1] + Box2->fLen[1] * absC[1][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = Box1->fLen[0] * absC[1][0] + Box1->fLen[1] * absC[0][0];
	r1 = Box2->fLen[1] * absC[2][2] + Box2->fLen[2] * absC[2][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = Box1->fLen[0] * absC[1][1] + Box1->fLen[1] * absC[0][1];
	r1 = Box2->fLen[0] * absC[2][2] + Box2->fLen[2] * absC[2][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = Box1->fLen[0] * absC[1][2] + Box1->fLen[1] * absC[0][2];
	r1 = Box2->fLen[0] * absC[2][1] + Box2->fLen[1] * absC[2][0];
	if (r > r0 + r1)
		return FALSE;

	return TRUE;

}


void CCollisionMgr::Free(void)
{
}
