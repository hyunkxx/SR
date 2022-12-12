#include "..\..\Header\Boom_Bullet.h"

#include "Voxel.h"
#include "Export_Function.h"

USING(Engine)

CBoom_Bullet::CBoom_Bullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoom_Bullet::CBoom_Bullet(const CBoom_Bullet & rhs)
	: CGameObject(rhs)
{
}

CBoom_Bullet::~CBoom_Bullet()
{
	Free();
}

HRESULT CBoom_Bullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CBoom_Bullet::Update_Object(const _float & fTimeDelta)
{


	_vec3 Move, Dir1, Dir2, Dir3;
	Move.x = m_vDir.x * m_fSpeed * fTimeDelta;
	Move.z = m_vDir.z * m_fSpeed * fTimeDelta;
	Move.y = ((m_vDir.y * m_fSpeed * fTimeDelta) - (0.5f * 9.8f * (m_fAccum * m_fAccum)));

	Dir3 = Dir2 = Dir1 = Move;
	Dir1.y = 0.f;
	Dir3.x = 0.f;
	D3DXVec3Normalize(&Dir1, &Dir1);
	D3DXVec3Normalize(&Dir2, &Dir2);
	D3DXVec3Normalize(&Dir3, &Dir3);

	_float RadianX = acosf(D3DXVec3Dot(&Dir2, &Dir1));
	_float RadianY = acosf(D3DXVec3Dot(&Dir2, &Dir3));

	if (Dir2.y < Dir1.y)
		RadianX *= -1;
	if (Dir2.x < Dir3.x)
		RadianY *= -1;
	//m_pTransformCom->Rotation(ROT_Z, -100 * fTimeDelta);
	m_pTransformCom->Rotation(ROT_X, -(m_pTransformCom->Get_Angle(ROT_X)) - RadianX);
	m_pTransformCom->Rotation(ROT_Y, -(m_pTransformCom->Get_Angle(ROT_Y)) + RadianY);

	m_pTransformCom->Move_Pos(&Move);
	m_vPos += Move;


	/*m_fAccum += fTimeDelta;
	_vec3 Move;


	Move.x = 0.f;
	Move.z = 0.f;
	Move.y = -m_fSpeed * fTimeDelta;
	m_vPos += Move;

	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-500.f *fTimeDelta));
	m_pTransformCom->Move_Pos(&Move);*/

	Add_RenderGroup(RENDER_NONALPHA, this);



	return 	__super::Update_Object(fTimeDelta);
}

void CBoom_Bullet::LateUpdate_Object(void)
{

	if (0.f > m_vPos.y)
	{
		m_bDead = true;
		Engine::Camera_Change(L"TankCamera");
		Engine::Get_Camera()->Camera_Setting(m_vPos);
		Engine::Get_Camera()->Shake_On();
		_float fShootSound = 8.f;
		Engine::PlaySound_SR(L"Boom_Sound.wav", PLAYER_SHOT_SOUND1, fShootSound);
	}
	__super::LateUpdate_Object();
}

void CBoom_Bullet::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBody->Render(m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
}

HRESULT CBoom_Bullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Boom_ally_body");

	if (!m_pBody)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));

	if (!m_pTransformCom)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CBoom_Bullet::Set_ID(BULLET_ID eID)
{
	m_eID = BULLET_ID::BOOM_BULLET;
}

void CBoom_Bullet::Bullet_Setting(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float AngleY)
{
	m_pTransformCom->Reset_Trans();

	m_fSpeed = 100.f;
	m_vDir = vDir;
	m_vPos = vPos;
	m_fScale = 5.f;
	m_fAccum = 0.f;
	m_bDead = false;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale * 2);
	m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	//m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

}

CBoom_Bullet * CBoom_Bullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoom_Bullet* pInstance = new CBoom_Bullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoom_Bullet::Free(void)
{
	CGameObject::Free();
}
