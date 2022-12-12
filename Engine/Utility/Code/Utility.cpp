#include "..\Header\Utility.h"

USING(Engine)

Utility::Utility()
{
}


Utility::~Utility()
{
}

float Utility::Lerp(float A, float B, float Alpha)
{
	return A * (1 - Alpha) + B * Alpha;
}

bool Utility::Cuilling(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	_matrix matView;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3 vCamDir = { matView._31, matView._32, matView._33 };
	_vec3 vCamPos = { matView._41, matView._42, matView._43 };

	D3DXVec3Normalize(&vCamDir, &vCamDir);

	_vec3 vTargetDir = (vPos - vCamPos) - (vCamDir) * 2.f;
	float fLength = D3DXVec3Length(&vTargetDir);

	if (fLength > 200.f)
		return true;

	D3DXVec3Normalize(&vTargetDir, &vTargetDir);
	float value = D3DXVec3Dot(&vCamDir, &vTargetDir);

	if (value < 0.8f)
		return true;
	else
		return false;
}
