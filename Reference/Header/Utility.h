#pragma once

#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL Utility
{
private:
	Utility();
	~Utility();
public:
	static float Lerp(float A, float B, float Alpha);
	static bool	Cuilling(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
};

END