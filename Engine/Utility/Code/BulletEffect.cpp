#include "..\..\Header\BulletEffect.h"
#include "MeshLoader.h"
#include "BulletEffect.h"
#include "TimerMgr.h"
#include "Utility.h"

USING(Engine)

CBulletEffect::CBulletEffect(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPos)
	: CEffector(pGraphicDevice, vPos)
{
	m_vecDir.reserve(MaxObjectCount);

	m_color[0] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
	m_color[1] = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f);
	m_color[2] = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);

	for (int i = 0; i < MaxObjectCount; ++i)
	{
		CCube* pCube = CCube::Create(pGraphicDevice);
		/* ·£´ý»ö */
		//CCube::SetColor(pCube, D3DXCOLOR(rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f, 1.f));
		CCube::SetColor(pCube, m_color[i % 3]);
		m_vecEffect.push_back(pCube);
	}

	for (int i = 0; i < MaxObjectCount; ++i)
	{
		m_vecDir.push_back(_vec3(cosf(float(rand() % 4)), cos(float(rand() % 4)), cosf(float(rand() % 4))));
	}

	for (auto iter = m_vecEffect.begin(); iter != m_vecEffect.end(); ++iter)
	{
		(*iter)->SetScale(_vec3((float(rand() % 2)) * 0.5f, 0.5f, (float(rand() % 2)) * 0.5f));
		(*iter)->SetRotation({ (float)(rand() % 90) ,(float)(rand() % 90),(float)(rand() % 90) });
	}
}

CBulletEffect::CBulletEffect(const CBulletEffect & rhs)
	: CEffector(rhs.m_pGraphicDev, rhs.m_vPosition)
{
	m_vecDir.reserve(MaxObjectCount);

}

CBulletEffect::~CBulletEffect()
{
}

CComponent* CBulletEffect::Clone(void)
{
	return new CBulletEffect(*this);
}

void CBulletEffect::Free(void)
{
	for (auto iter = m_vecEffect.begin(); iter != m_vecEffect.end(); ++iter)
	{
		Safe_Release(*iter);
	}
	__super::Free();
}

_int CBulletEffect::Update_Component(const _float & fTimeDelta)
{
	if (!m_bRunning)
		return 0;

	m_fLocalTime += fTimeDelta;
	m_fGravityLocal += fTimeDelta * 10.f;

	if (m_fLocalTime > m_fDuration)
	{
		Reset();
	}

	_matrix matScale, matAxis[3], matRot, matTrans;
	D3DXMatrixIdentity(&matRot);
	_matrix matOrigin;

	int i = 0;
	for (auto iter = m_vecEffect.begin(); iter != m_vecEffect.end(); ++iter)
	{
		_matrix matWorld = (*iter)->GetWorldMatix();

		D3DXMatrixScaling(
			&matScale,
			(*iter)->GetScale().x,
			(*iter)->GetScale().y,
			(*iter)->GetScale().z);

		D3DXMatrixRotationX(&matAxis[0], (*iter)->GetRotation().x);
		D3DXMatrixRotationY(&matAxis[1], (*iter)->GetRotation().y);
		D3DXMatrixRotationZ(&matAxis[2], (*iter)->GetRotation().z);
		matRot = matAxis[0] * matAxis[1] * matAxis[2];

		if ((*iter)->GetPosition().y + m_vPosition.y < 0.f)
		{
			D3DXMatrixTranslation(
				&matTrans,
				(*iter)->GetPosition().x,
				(*iter)->GetPosition().y,
				(*iter)->GetPosition().z);
		}
		else
		{
			D3DXMatrixTranslation(
				&matTrans,
				(*iter)->GetPosition().x += m_vecDir[i].x * m_fSpeed * fTimeDelta,
				(*iter)->GetPosition().y += (m_vecDir[i].y - m_fGravityLocal) * m_fSpeed * fTimeDelta,
				(*iter)->GetPosition().z += m_vecDir[i].z * m_fSpeed * fTimeDelta);
		}

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

void CBulletEffect::RenderEffect()
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

void CBulletEffect::Reset()
{
	m_bRunning = false;
	m_fLocalTime = 0.0f;
	m_fGravityLocal = -2.f;

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

CBulletEffect* CBulletEffect::Create(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition)
{
	CBulletEffect* pInstance = new CBulletEffect(pGraphicDevice, vPosition);

	if (!pInstance)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed BulletEffect Create()");
	}

	return pInstance;
}

void CBulletEffect::SetColor(D3DXCOLOR color[])
{
	m_color[0] = color[0];
	m_color[1] = color[1];
	m_color[2] = color[2];

	for (int i = 0; i < MaxObjectCount; ++i)
	{
		CCube* pCube = CCube::Create(m_pGraphicDev);

		CCube::SetColor(pCube, m_color[i % 3]);
		m_vecEffect.push_back(pCube);
	}
}

