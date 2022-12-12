#include "..\..\Header\FireEffect.h"

#include "MeshLoader.h"
#include "TimerMgr.h"
#include "Utility.h"

USING(Engine)

CFireEffect::CFireEffect(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPos)
	: CEffector(pGraphicDevice, vPos)
{
	m_fDuration = 0.5f;

	m_color[0] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	m_color[1] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
	m_color[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.f);

	for (int i = 0; i < MaxObjectCount; ++i)
	{
		CCube* pCube = CCube::Create(pGraphicDevice);
		CCube::SetColor(pCube, m_color[rand() % 3]);
		m_vecEffect.push_back(pCube);
	}

	for (int i = 0; i < MaxObjectCount; ++i)
	{
		m_vDir.push_back(_vec3(cos(float(rand() % 10)), cosf(float(rand() % 10)), cosf(float(rand() % 10))));
		//D3DXVec3Normalize(&m_vDir[i], &m_vDir[i]);
	}

	for (auto iter = m_vecEffect.begin(); iter != m_vecEffect.end(); ++iter)
	{
		(*iter)->SetScale(_vec3(float(rand() % 3 + 1 * 0.5f), float(rand() % 3 + 1 * 0.5f), float(rand() % 3 + 1 * 0.5f)));
		(*iter)->SetRotation({ (float)(rand() % 90) ,(float)(rand() % 90),(float)(rand() % 90) });
	}
}

CFireEffect::CFireEffect(const CFireEffect & rhs)
	: CEffector(rhs.m_pGraphicDev, rhs.m_vPosition)
{
}

CFireEffect::~CFireEffect()
{
}

CComponent * CFireEffect::Clone(void)
{
	return new CFireEffect(*this);
}

void CFireEffect::Free(void)
{
	for (auto iter = m_vecEffect.begin(); iter != m_vecEffect.end(); ++iter)
	{
		Safe_Release(*iter);
	}
	__super::Free();
}

_int CFireEffect::Update_Component(const _float & fTimeDelta)
{
	if (!m_bRunning)
		return 0;

	m_fLocalTime += fTimeDelta;
	m_fGravityLocal += fTimeDelta * 3.f;

	if (m_fLocalTime > m_fDuration)
	{
		Reset();
		m_fGravityLocal = -2.f;
	}

	_matrix matScale, matAxis[3], matRot, matTrans;
	D3DXMatrixIdentity(&matRot);
	_matrix matOrigin;

	int i = 0;
	for (auto iter = m_vecEffect.begin(); iter != m_vecEffect.end(); ++iter)
	{
		D3DXMatrixScaling(
			&matScale,
			(*iter)->GetScale().x,
			(*iter)->GetScale().y,
			(*iter)->GetScale().z);

		D3DXMatrixRotationX(&matAxis[0], (*iter)->GetRotation().x);
		D3DXMatrixRotationY(&matAxis[1], (*iter)->GetRotation().y);
		D3DXMatrixRotationZ(&matAxis[2], (*iter)->GetRotation().z);
		matRot = matAxis[0] * matAxis[1] * matAxis[2];

		D3DXMatrixTranslation(
			&matTrans,
			(*iter)->GetPosition().x += m_vDir[i].x * m_fSpeed * fTimeDelta,
			(*iter)->GetPosition().y += m_vDir[i].y * m_fSpeed * fTimeDelta,
			(*iter)->GetPosition().z += m_vDir[i].z * m_fSpeed * fTimeDelta);

		D3DXMatrixTranslation(
			&matOrigin,
			m_vPosition.x,
			m_vPosition.y,
			m_vPosition.z);

		(*iter)->SetWorldMatrix(matScale * matRot * matTrans * matOrigin);
		i++;
	}

	return _int();
}

void CFireEffect::RenderEffect()
{
	if (!m_bRunning)
		return;

	for (auto iter = m_vecEffect.begin(); iter != m_vecEffect.end(); ++iter)
	{
		_matrix matWorld = (*iter)->GetWorldMatix();

		_vec3 vPos = { matWorld._41, matWorld._42, matWorld._43 };
		if (Utility::Cuilling(m_pGraphicDev, vPos))
			continue;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &(*iter)->GetWorld());
		(*iter)->Render_Buffer();
	}
}

void CFireEffect::Reset()
{
	m_bRunning = false;
	m_fLocalTime = 0.0f;

	m_vDir.clear();

	for (int i = 0; i < MaxObjectCount; ++i)
	{
		m_vDir.push_back(_vec3(cos(float(rand() % 10)), cosf(float(rand() % 10)), cosf(float(rand() % 10))));
		D3DXVec3Normalize(&m_vDir[i], &m_vDir[i]);
	}

	_matrix matWorld;
	_matrix matOrigin;

	for (auto iter = m_vecEffect.begin(); iter != m_vecEffect.end(); ++iter)
	{
		(*iter)->SetPosition(_vec3(0.f, 0.f, 0.f));

		D3DXMatrixTranslation(
			&matOrigin,
			m_vPosition.x,
			m_vPosition.y,
			m_vPosition.z
		);

		(*iter)->SetWorldMatrix(matWorld * matOrigin);
	}
}

CFireEffect* CFireEffect::Create(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition)
{
	CFireEffect* pInstance = new CFireEffect(pGraphicDevice, vPosition);

	if (!pInstance)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed CFireEffect Create()");
	}

	return pInstance;
}