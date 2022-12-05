#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr :
	public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();
public:
	_bool Sphere_Collision(_vec3 First, _vec3 Second, _float first_range, _float Second_range);
	_vec3 Sphere_Collision_EX(_vec3 First, _vec3 Second, _float first_range, _float Second_range);
	_bool OBB_Collision(OBB* Box1, OBB* Box2);
public:
	virtual void		Free(void);
};
END

