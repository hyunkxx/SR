#include "..\..\Header\Bullet.h"

#include "Export_Function.h"

USING(Engine)

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	m_bDead = true;
}
CBullet::CBullet(const CBullet& rhs)
	: CGameObject(rhs)
{
}

CBullet::~CBullet()
{
	Free();
}

HRESULT CBullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_stBody.fLen[x] = 0.1f * m_fScale;
	m_stBody.fLen[y] = 0.1f * m_fScale;
	m_stBody.fLen[z] = 1.f * m_fScale;

	if (m_eID == BULLET_ID::AH_64A_BULLET)
	{
		m_stBody.fLen[x] = 2.f * m_fScale;
		m_stBody.fLen[y] = 2.f  * m_fScale;
		m_stBody.fLen[z] = 2.f * m_fScale;

	}

	CGameObject::Ready_Object();

	return S_OK;
}

_int CBullet::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAccum += fTimeDelta;
	m_pTransform->Set_Scale(m_fScale, m_fScale, m_fScale);
	_vec3 Move, Dir1, Dir2;
	Move.x = m_vDir.x * m_fSpeed * fTimeDelta;
	Move.z = m_vDir.z * m_fSpeed * fTimeDelta;

	if(0.f < m_fAngleX)
		Move.y = ((m_vDir.y * m_fSpeed * fTimeDelta) * sinf(m_fAngleX) - (0.5f * 9.8f * (m_fAccum * m_fAccum)));
	else
		Move.y = ((m_vDir.y * m_fSpeed * fTimeDelta) * sinf(-m_fAngleX) - (0.5f * 9.8f * (m_fAccum * m_fAccum)));

	Dir2 = Dir1 = Move;
	Dir1.y = 0.f;
	D3DXVec3Normalize(&Dir1, &Dir1);
	D3DXVec3Normalize(&Dir2, &Dir2);
	_float Scalar = D3DXVec3Dot(&Dir2, &Dir1);
	
	Scalar = acosf(Scalar);
	if (Dir2.y < Dir1.y)
		Scalar *= -1;

	if(m_eID == CANNONBALL_RELOAD || m_eID == MASHINE_BULLET_RELOAD)
		m_pTransform->Rotation(ROT_X,D3DXToRadian(-1000.f *fTimeDelta));
	else if (m_eID == AH_64A_BULLET)
		m_pTransform->Rotation(ROT_X, -(m_pTransform->Get_Angle(ROT_X)) + Scalar);
	else
		m_pTransform->Rotation(ROT_X, -(m_pTransform->Get_Angle(ROT_X)) - Scalar);

	if (m_eID == BULLET_ID::AH_64A_BULLET)
	{
		Move = m_vDir * m_fSpeed * fTimeDelta;
	}

	m_pTransform->Move_Pos(&Move);
	__super::Update_Object(fTimeDelta);
	Update_OBB();
	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CBullet::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	CGameObject::LateUpdate_Object();

	m_pTransform->Get_Info(INFO_POS, &m_vPos);

	if (m_vPos.y < 0.f)
		m_bDead = true;

	if (BULLET_ID::AH_64A_BULLET == m_eID)
	{
		if (m_vPos.z < 0.f || m_vPos.z > 5000.f)
			m_bDead = true;
		if (m_vPos.x < 0.f || m_vPos.x > 5000.f)
			m_bDead = true;
	}
}

void CBullet::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());


	if(m_eID == CANNONBALL_RELOAD || m_eID == MASHINE_BULLET_RELOAD)
		m_pBUllet_Re_Buffer->Render_Buffer();
	else
		m_pBUlletBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CBullet::Bullet_Setting(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float fAngleY)
{
	m_stBody.vPos = { -100.f,-100.f,-100.f };
	Update_OBB();
	m_vPos = vPos;
	m_vDir = vDir;
	m_fSpeed = fSpeed;
	m_fAccum = 0.f;
	m_bDead = false;
	m_fAngleX = fAngleX;
	m_fAngleY = fAngleY;
	m_pTransform->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransform->Rotation(ROT_Y, fAngleY);
}

HRESULT CBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Bullet_Transform", pComponent });

	pComponent = m_pBUlletBuffer = static_cast<CVIBullet*>(Clone_Prototype(L"Proto_VIBullet"));
	NULL_CHECK_RETURN(m_pBUlletBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_VIBullet", pComponent });

	pComponent = m_pBUllet_Re_Buffer = static_cast<CVIBullet_Re*>(Clone_Prototype(L"Proto_VIBullet_Re"));
	NULL_CHECK_RETURN(m_pBUlletBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_VIBullet_Re", pComponent });

	return S_OK;
}
void CBullet::Update_OBB(void)
{
	_vec3 Pos, Right, Up, Look;

	m_pTransform->Get_Info(INFO_POS, &m_stBody.vPos);
	m_pTransform->Get_Info(INFO_RIGHT, &m_stBody.vDir[x]);
	m_pTransform->Get_Info(INFO_UP, &m_stBody.vDir[y]);
	m_pTransform->Get_Info(INFO_LOOK, &m_stBody.vDir[z]);

	for (_int i = 0; i < 3; i++)
		D3DXVec3Normalize(&m_stBody.vDir[i], &m_stBody.vDir[i]);
}
void CBullet::Set_ID(BULLET_ID eID)
{
	m_eID = eID;
	if (eID == BULLET_ID::MASHINE_BULLET || eID == BULLET_ID::ENEMY_MASHINE_BULLET)
	{
		m_fHitRange = 0.f;
		m_fScale = 2.f;
	}
	else if (eID == BULLET_ID::CANNONBALL || eID == BULLET_ID::ENEMY_CANNONBALL)
	{
		m_fHitRange = 3.f;
		m_fScale = 10.f;
	}
	else if (eID == BULLET_ID::SMALL_CANNONBALL|| eID == BULLET_ID::ENEMY_SMALL_CANNONBALL)
	{
		m_fHitRange = 3.f;
		m_fScale = 8.f;
	}
	else if (eID == BULLET_ID::MIDDLE_CANNONBALL  ||eID == BULLET_ID::ENEMY_MIDDLE_CANNONBALL)
	{
		m_fHitRange = 2.f;
		m_fScale = 9.f;
	}
	else if (eID == BULLET_ID::BIG_CANNONBALL || eID == BULLET_ID::ENEMY_BIG_CANNONBALL)
	{
		m_fHitRange = 2.5f;
		m_fScale = 10.f;
	}
	else if (eID == BULLET_ID::AH_64A_BULLET)
	{
		m_fHitRange = 0.f;
		m_fScale = 5.f;
		m_stBody.fLen[x] = 2.f * m_fScale;
		m_stBody.fLen[y] = 2.f  * m_fScale;
		m_stBody.fLen[z] = 2.f * m_fScale;
	}
	else if (eID == BULLET_ID::SHIP_BULLET)
	{
		m_fHitRange = 3.f;
		m_fScale = 20.f;
	}
	else if (eID == BULLET_ID::MASHINE_BULLET_RELOAD)
	{
		m_fHitRange = 0.f;
		m_fScale = 1.f;
	}
	else if (eID == BULLET_ID::CANNONBALL_RELOAD)
	{
		m_fHitRange = 3.f;
		m_fScale = 3.f;
	}
}

void CBullet::Reset_Trans(void)
{
	m_pTransform->Reset_Trans();
	m_pTransform->Set_Pos(-100.f, 50.f, -100.f);
	m_stBody.vPos = { -100.f,-50.f ,-100.f };
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBullet*	pInstance = new CBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CBullet Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBullet::Free(void)
{
	CGameObject::Free();
}
