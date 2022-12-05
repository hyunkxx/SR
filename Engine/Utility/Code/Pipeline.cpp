#include "..\..\Header\Pipeline.h"



CPipeline::CPipeline()
{
}


CPipeline::~CPipeline()
{
}

void Engine::CPipeline::MakeTransformMatrix(_matrix* _pMatrix, const _vec3* pRight, const _vec3* pUp, const _vec3* pLook, const _vec3* pPos)
{
	memcpy(&_pMatrix->m[0][0], pRight, sizeof(_vec3));
	memcpy(&_pMatrix->m[1][0], pUp, sizeof(_vec3));
	memcpy(&_pMatrix->m[2][0], pLook, sizeof(_vec3));
	memcpy(&_pMatrix->m[3][0], pPos, sizeof(_vec3));
}

void CPipeline::MakeViewMatrix(_matrix * _pMatrix, 
								const _vec3 * pEye, 
								const _vec3 * pAt, 
								const _vec3 * pUp)
{
	D3DXMatrixIdentity(_pMatrix);

	_vec3	vRight	{ 1.f, 0.f, 0.f };
	_vec3	vUp		{ 0.f, 1.f, 0.f };
	_vec3	vLook	{ 0.f, 0.f, 1.f };
	_vec3	vPos	{ 0.f, 0.f, 0.f };

	// look
	vLook = *pAt - *pEye;
	D3DXVec3Normalize(&vLook, &vLook);

	// right
	D3DXVec3Cross(&vRight, pUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	// up
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	// pos
	vPos = *pEye;
	
	MakeTransformMatrix(_pMatrix, &vRight, &vUp, &vLook, &vPos);

	// 뷰스페이스 변환 행렬
	D3DXMatrixInverse(_pMatrix, 0, _pMatrix);
}

void CPipeline::MakePerspectiveMatrix(_matrix * _pMatrix, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	D3DXMatrixIdentity(_pMatrix);

	_pMatrix->_11 =  (1.f / tanf(fFov / 2.f)) / fAspect;
	_pMatrix->_22 = 1.f / tanf(fFov / 2.f);

	_pMatrix->_33 = fFar / (fFar - fNear);
	_pMatrix->_34 = 1.f;
	
	_pMatrix->_43 = (-fNear * fFar) / (fFar - fNear);
	_pMatrix->_44 = 0.f;

}
