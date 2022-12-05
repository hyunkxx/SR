#include "..\..\Header\Calculator.h"

#include "Export_Function.h"

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CCalculator::CCalculator(const CCalculator & rhs)
	:CComponent(rhs)
{
}

CCalculator::~CCalculator()
{
}

HRESULT CCalculator::Ready_Calculator(void)
{
	return S_OK;
}

HRESULT CCalculator::Compute_HeightOnTerrain(_vec3 * pPos, const _vec3 * pTerrainVtxPos, const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong& dwItv)
{
	// 평면의 방정식

	if (pPos->x< 0 || pPos->z < 0 || pPos->x * pPos->y >dwCntX * dwCntZ)
		return false;

	D3DXPLANE		Plane;

	_ulong		dwIndex = _ulong(pPos->z / dwItv)* dwCntX + _ulong(pPos->x / dwItv);

	_float		fRatioX = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwItv;
	_float		fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwItv;



	// 오른쪽 위 삼각형
	if (fRatioX >fRatioZ)
	{
		D3DXPlaneFromPoints(&Plane, 
			&pTerrainVtxPos[dwCntX + dwIndex],
			&pTerrainVtxPos[dwCntX + dwIndex + 1],
			&pTerrainVtxPos[dwIndex + 1]);
	}
	// 왼쪽 아래 삼각형
	else
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwCntX + dwIndex],
			&pTerrainVtxPos[dwIndex + 1],
			&pTerrainVtxPos[dwIndex]);
	}

	//ax + by + cz + d = 0;
	// by = -ax - cz - d 
	// y = (-ax - cz - d) / b

	pPos->y = (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;

	return S_OK;
}

_vec3 Engine::CCalculator::Picking_OnTerrain(HWND hWnd, const CTerrainTex* pTerrainVtxCom, const CTransform* pTerrainTransform)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 -> 투영
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	//로 * 월 * 뷰 * (I)

	// 투영 -> 뷰 스페이스
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3	vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };		// 뷰 스페이스 공간의 원점
	vRayDir = vMousePos - vRayPos;

	// 뷰 스페이스 -> 월드
	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬
	_matrix		matWorld;
	pTerrainTransform->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, 0, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	_float		fU, fV, fDist;

	if (D3DXIntersectTri(&_vec3(1000.f, 0.f, 1000.f),
		&_vec3(1000.f, 0.f, 0.1f),
		&_vec3(0.1f, 0.f, 1000.f),
		&vRayPos, &vRayDir, &fU, &fV, &fDist))
	{
		return _vec3((1000.f + 0.1f - 1000.f) * fV,
			0.f,
			(1000.f + 0.1f - 1000.f)* fU);
	}

	if (D3DXIntersectTri(&_vec3(0.1f, 0.f, 0.1f),
		&_vec3(0.1f, 0.f, 1000.f),
		&_vec3(1000.f, 0.f, 0.1f),
		&vRayPos, &vRayDir, &fU, &fV, &fDist))
	{
		return _vec3((0.1f + 1000.f - 0.1f)* fV,
			0.f,
			(0.1f + 1000.f - 0.1f) * fU);

	}
	return _vec3(0.f, 0.f, 0.f);

	return _vec3(0.f, 0.f, 0.f);
}

CCalculator * CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator *	pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
	{
		Safe_Release(pInstance);	
	}

	return pInstance;
}

CComponent* Engine::CCalculator::Clone(void)
{
	return new CCalculator(*this);
}

void Engine::CCalculator::Free(void)
{
	CComponent::Free();
}

