#include "stdafx.h"
#include "..\Header\Boss_Bomber.h"

#include "Export_Function.h"
#include "UI_Volume.h"

CBoss_Bomber::CBoss_Bomber(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoss_Bomber::CBoss_Bomber(const CBoss_Bomber & rhs)
	: CGameObject(rhs)
{
}

CBoss_Bomber::~CBoss_Bomber()
{

}

_int CBoss_Bomber::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return 0;

	m_fStrike_Count += fTimeDelta;
	m_fSoundCheckTime += fTimeDelta;

	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(200 * fTimeDelta));

	m_fScale = 5.f;
	m_vMileage += m_vDir * m_fSpeed * fTimeDelta;
	m_pTransformBody->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));
	m_pTransformBody->Set_Scale(m_fScale, m_fScale, m_fScale);



	m_FAngle_Accum += 90.f *fTimeDelta;

	_vec3 Pos;
	m_pTransformBody->Get_Info(INFO_POS, &Pos);

	_vec3 Inst = m_vStrike_Pos - Pos;
	_float Dist = sqrtf((Inst.x * Inst.x) + (Inst.z * Inst.z));

	if (200.f > Dist && !m_bBoomShoot && 1.f < m_fStrike_Count)
	{
		Engine::Reuse_Object(Pos, Inst, 300.f, 0.f, 0.f, BULLET_ID::BOOM_BULLET);
		m_bBoomShoot = true;
	}

	if (sqrtf((m_vMileage.x*m_vMileage.x) + (m_vMileage.z*m_vMileage.z)) > 2000.f)
		m_bDead = true;
		
	Add_RenderGroup(RENDER_NONALPHA, this);

	return CGameObject::Update_Object(fTimeDelta);
}

void CBoss_Bomber::LateUpdate_Object(void)
{
	if (m_bDead)
		return;


	return CGameObject::LateUpdate_Object();
}

void CBoss_Bomber::Render_Object(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBody->Get_WorldMatrix());
	m_pBody->Render(m_pTransformBody->Get_WorldMatrix());

	if (!m_bBoomShoot)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}


}

void CBoss_Bomber::RenderGUI(void)
{
}

HRESULT CBoss_Bomber::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransformBody = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Boss_Bomber_Transform", pComponent });

	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"air_ally_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });


	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"target_3"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"target_3", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

HRESULT CBoss_Bomber::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}


CBoss_Bomber * CBoss_Bomber::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_Bomber* pInstance = new CBoss_Bomber(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;

}

void CBoss_Bomber::Free(void)
{
	CGameObject::Free();
}

void CBoss_Bomber::Strike(_vec3 Start_Pos, _vec3 _Strike_Pos, _int SoundType)
{
	m_vStart_Pos = Start_Pos;
	m_vStrike_Pos = _Strike_Pos;
	m_pTransformBody->Reset_Trans();
	m_pTransformCom->Reset_Trans();
	m_pTransformCom->Set_Scale(10.f, 10.f, 10.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pTransformCom->Set_Pos(_Strike_Pos.x, 0.1f, _Strike_Pos.z);
	m_pTransformBody->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_vDir = _Strike_Pos - m_vStart_Pos;
	m_vDir.y = 0.f;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vMileage = _vec3(0.f, 0.f, 0.f);
	m_FAngle_Accum = 0.f;
	m_fStrike_Count = 0.f;
	m_bDead = false;
	m_bBoomShoot = false;
	m_iSoundType = SoundType;
	_vec3 Look(0.f, 0.f, 1.f);
	_float Radian = acosf(D3DXVec3Dot(&m_vDir, &Look));

	if (m_vDir.x < Look.x)
		Radian *= -1;

	m_pTransformBody->Rotation(ROT_Y, Radian);
	m_pTransformBody->Set_Pos(m_vStart_Pos.x, m_vStart_Pos.y, m_vStart_Pos.z);

	if (m_iSoundType == 0)
	{
		Engine::StopSound(BOSS_BOMBER_SOUND1);
		Engine::PlaySound_SR(L"airplane.wav", BOSS_BOMBER_SOUND1, 1.f);
	}
	else if (m_iSoundType == 1)
	{
		Engine::StopSound(BOSS_BOMBER_SOUND2);
		Engine::PlaySound_SR(L"airplane.wav", BOSS_BOMBER_SOUND2, 1.f);
	}
	else if (m_iSoundType == 2)
	{
		Engine::StopSound(BOSS_BOMBER_SOUND3);
		Engine::PlaySound_SR(L"airplane.wav", BOSS_BOMBER_SOUND3, 1.f);
	}
	else if (m_iSoundType == 3)
	{
		Engine::StopSound(BOSS_BOMBER_SOUND4);
		Engine::PlaySound_SR(L"airplane.wav", BOSS_BOMBER_SOUND4, 1.f);
	}
	else if (m_iSoundType == 4)
	{
		Engine::StopSound(BOSS_BOMBER_SOUND5);
		Engine::PlaySound_SR(L"airplane.wav", BOSS_BOMBER_SOUND5, 1.f);
	}


}
	
	
