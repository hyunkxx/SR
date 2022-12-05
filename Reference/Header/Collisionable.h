#pragma once

#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL ICollisionable
{

public:
	explicit ICollisionable();
	virtual ~ICollisionable();

public:
	virtual const   _vec3		Get_Info(void)PURE;
	virtual			void		Move_Info(_vec3 _Info)PURE;
	virtual			void		OBB_Collision_EX(void)PURE;
	virtual			void		Update_OBB(void)PURE;
	virtual			OBB*        Get_OBB(void)PURE;
};

END
