#pragma once

#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL CPipeline
{
public:
	explicit CPipeline();
	~CPipeline();

public:
	static void	MakeTransformMatrix(_matrix* _pMatrix, const _vec3* pRight, const _vec3* pUp, const _vec3* pLook, const _vec3* pPos);
	static void MakeViewMatrix(_matrix* _pMatrix, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp);
	static void MakePerspectiveMatrix(_matrix* _pMatrix, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);

};

END