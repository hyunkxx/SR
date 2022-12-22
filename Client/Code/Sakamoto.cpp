#include "stdafx.h"
#include "..\Header\Sakamoto.h"

#include  "Export_Function.h"
#include "RushTank.h"

CSakamoto::CSakamoto(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSakamoto::CSakamoto(const CSakamoto & rhs)
	: CGameObject(rhs)
{
}

CSakamoto::~CSakamoto()
{
}

HRESULT CSakamoto::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	/* 직교투영행렬을 만든다. */

	/* 윈도우좌표 기준의 위치. */
	m_fSizeX = 20.f;
	m_fSizeY = 20.f;
	m_fX = 400.f;
	m_fY = 300.f;

	m_pTransformCom->Set_Scale(m_fSizeY * 1.7f, m_fSizeY, 1.f);
	m_pTransformCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 1.f);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.0f);
	return S_OK;
}

_int CSakamoto::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	
	if (!m_bType)
	{
		if (100.f < m_fY)
			m_fY -= 300.f * fTimeDelta;
		else
		{
			if (!m_bSound)
			{
				//대사 사운드 재생 및 무적 처리 
				Engine::StopAll();
				Engine::PlaySound_SR(L"Sakamoto.mp3", BOSS_BGM, 1.f);
				m_bSound = true;
			}
			m_fSoundCount += fTimeDelta;
			m_fY = 100.f;
			if (3.f < m_fSoundCount)
			{
				Engine::Get_Camera()->Set_Fov(D3DXToRadian(60.f));
				// Nyan Cat 재생
				Engine::StopAll();
				Engine::PlaySound_SR(L"God_Mode_Sound.mp3", BOSS_BGM, 1.f);
				m_bType = true;
			}

		}
	}
	else
	{
		if (1000.f > m_fY)
			m_fY += 300.f * fTimeDelta;
		else
			m_bDead = true;
	}
	
	m_pTransformCom->Set_Scale(300.f, 200.f, 1.f);
	m_pTransformCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 1.f);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);


	__super::Update_Object(fTimeDelta);
	

	return OBJ_NOEVENT;
}

void CSakamoto::LateUpdate_Object(void)
{
	if (m_bDead)
		return;


	__super::LateUpdate_Object();
	Add_RenderGroup(RENDER_UI, this);
}

void CSakamoto::Render_Object(void)
{
	if (m_bDead)
		return;

	_matrix OldViewMatrix, OldProjMatrix;


	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	_matrix			ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

}

HRESULT CSakamoto::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Sakamoto"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Sakamoto", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CSakamoto::Set_Sakamoto(void)
{
	m_bDead = false;
	m_bType = false;
	m_bSound = false;
	m_fSizeX = 20.f;
	m_fSizeY = 200.f;
	m_fX = 500.f;
	m_fY = 600.f;
	m_fSoundCount = 0.f;
}

CSakamoto * CSakamoto::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSakamoto*		pInstance = new CSakamoto(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CSakamoto Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSakamoto::Free(void)
{
	__super::Free();
}
