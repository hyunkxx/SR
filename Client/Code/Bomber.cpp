#include "stdafx.h"
#include "..\Header\Bomber.h"

#include "Export_Function.h"
#include "BoomCamera.h"
#include "UI_Volume.h"

CBomber::CBomber(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBomber::CBomber(const CBomber & rhs)
	: CGameObject(rhs)
{
}

CBomber::~CBomber()
{

}

_int CBomber::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return 0;

	m_fStrike_Count += fTimeDelta;

	m_fScale = 10.f;
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

		Engine::Reuse_Object(Pos, Inst, 200.f, 0.f, 0.f, BULLET_ID::BOOM_BULLET);
		if (dynamic_cast<CBoomCamera*>(Engine::Get_Camera()))
		{

			auto& iter = Engine::Get_Bullet_List(BULLET_ID::BOOM_BULLET)->begin();
			dynamic_cast<CBoomCamera*>(Engine::Get_Camera())->Set_Target(static_cast<CTransform*>((*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC)));
		}
		m_bBoomShoot = true;
	}

	if (sqrtf((m_vMileage.x*m_vMileage.x) + (m_vMileage.z*m_vMileage.z)) > 2000.f)
		m_bDead = true;

	Add_RenderGroup(RENDER_NONALPHA, this);

	return CGameObject::Update_Object(fTimeDelta);
}

void CBomber::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	return CGameObject::LateUpdate_Object();
}

void CBomber::Render_Object(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBody->Get_WorldMatrix());
	m_pBody->Render(m_pTransformBody->Get_WorldMatrix());

}

void CBomber::RenderGUI(void)
{
}

HRESULT CBomber::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransformBody = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Bomber_Transform", pComponent });

	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"air_ally_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	return S_OK;
}

HRESULT CBomber::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

CBomber * CBomber::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBomber* pInstance = new CBomber(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;

}

void CBomber::Free(void)
{
	CGameObject::Free();
}

void CBomber::Strike(_vec3 _Strike_Pos)
{
	m_vStrike_Pos = _Strike_Pos;
	m_pTransformBody->Reset_Trans();
	m_pTransformBody->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_vDir = _Strike_Pos - m_vStart_Pos;
	m_vDir.y = 0.f;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vMileage = _vec3(0.f, 0.f, 0.f);
	m_FAngle_Accum = 0.f;
	m_fStrike_Count = 0.f;
	m_bDead = false;
	m_bBoomShoot = false;
	_vec3 Look(0.f, 0.f, 1.f);
	_float Radian = acosf(D3DXVec3Dot(&m_vDir, &Look));

	if (m_vDir.x < Look.x)
		Radian *= -1;
	Engine::StopSound(AIRPLANE_SOUND);
	Engine::PlaySound_SR(L"airplane.wav", AIRPLANE_SOUND, CUI_Volume::s_fBGMSound);
	
	m_pTransformBody->Rotation(ROT_Y, Radian);
	m_pTransformBody->Set_Pos(m_vStart_Pos.x, m_vStart_Pos.y, m_vStart_Pos.z);
	Engine::Camera_Change(L"BoomCamera");
	dynamic_cast<CBoomCamera*>(Engine::Get_Camera())->Reset_Pos();
	dynamic_cast<CBoomCamera*>(Engine::Get_Camera())->Set_Target(m_pTransformBody);

}
