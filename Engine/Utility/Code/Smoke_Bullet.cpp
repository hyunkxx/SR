#include "..\..\Header\Smoke_Bullet.h"

#include "Export_Function.h"

CSmoke_Bullet::CSmoke_Bullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSmoke_Bullet::CSmoke_Bullet(const CSmoke_Bullet & rhs)
	: CGameObject(rhs)
{
}

CSmoke_Bullet::~CSmoke_Bullet()
{
	Free();
}

HRESULT CSmoke_Bullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bDead = true;
	m_fScale = 3.f;

	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);

	return S_OK;
}

_int CSmoke_Bullet::Update_Object(const _float & fTimeDelta)
{
	m_fSpeed += 2000 * fTimeDelta;

	if (m_bPage)
	{
		if (m_fFrame < 12.f)
			m_fFrame += 10.f * fTimeDelta;
		else
			m_fFrame += 6.f * fTimeDelta;

		if (m_fFrame > 63.f)
		{
			m_bPage = false;
			m_bDead = true;
			m_fFrame = 0.f;
			m_fSpeed = 0.f;
			m_fAccum = 0.f;
			m_fScale = 3.f;
			m_fAngleX = 0.f;
			m_fAngleY = 0.f;
			m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
		}
	}
	else if (!m_bPage)
	{
		m_fAccum += fTimeDelta;
		m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
		_vec3 Move, Dir1, Dir2;
		Move.x = m_vDir.x * m_fSpeed * fTimeDelta * cosf(-m_fAngleX);
		Move.z = m_vDir.z * m_fSpeed * fTimeDelta * cosf(-m_fAngleX);
		Move.y = ((m_vDir.y * m_fSpeed * fTimeDelta) * sinf(-m_fAngleX) - (0.5f * 9.8f * (m_fAccum * m_fAccum)));

		Dir2 = Dir1 = Move;
		Dir1.y = 0.f;
		D3DXVec3Normalize(&Dir1, &Dir1);
		D3DXVec3Normalize(&Dir2, &Dir2);
		_float Scalar = D3DXVec3Dot(&Dir2, &Dir1);

		Scalar = acosf(Scalar);
		if (Dir2.y < Dir1.y)
			Scalar *= -1;


		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(-1000.f *fTimeDelta));
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-1000.f *fTimeDelta));
		m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(-1000.f *fTimeDelta));
		m_pTransformCom->Move_Pos(&Move);

		if (0.2f <= m_fAccum)
		{
			_vec3 Pos;
			m_pTransformCom->Get_Info(INFO_POS, &Pos);
			m_pTransformCom->Reset_Trans();
			m_fScale = 40.f;
			m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
			m_pTransformCom->Set_Pos(Pos.x, Pos.y, Pos.z);
			m_pTransformCom->Move_Pos(&_vec3(0.f, 22.f, 0.f));
			m_bPage = true;
		}

	}
	__super::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CSmoke_Bullet::LateUpdate_Object(void)
{
	if (m_bPage)
	{
		_vec3	vTemp;
		m_pTransformCom->Get_Info(INFO_POS, &vTemp);

		__super::Compute_ViewZ(&vTemp);

		_matrix		matWorld, matView, matBill;
		m_pTransformCom->Get_WorldMatrix(&matWorld);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixIdentity(&matBill);

		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		D3DXMatrixInverse(&matBill, 0, &matBill);

		m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
	}
	__super::LateUpdate_Object();
}

void CSmoke_Bullet::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_bPage)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture(_ulong(m_fFrame));
		m_pBufferCom->Render_Buffer();
	}
	else
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pSmokeBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CSmoke_Bullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;



	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Gun_Smoke_Shell"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Gun_Smoke_Shell", pComponent });

	pComponent = m_pSmokeBufferCom = dynamic_cast<CVISmoke*>(Clone_Prototype(L"Proto_VISmoke"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_VISmoke", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CSmoke_Bullet::Set_ID(BULLET_ID eID)
{
	m_eID = eID;
}

void CSmoke_Bullet::Bullet_Setting(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float AngleY)
{
	m_pTransformCom->Reset_Trans();
	m_vPos = vPos;
	m_vDir = vDir;
	//m_fSpeed = fSpeed;
	m_fAccum = 0.f;
	m_bDead = false;
	m_fAngleX = fAngleX;
	m_fAngleY = AngleY;
	if (Engine::CBulletMgr::GetInstance()->Get_Bullet_List(BULLET_ID::SMOKE_BULLET)->size() == 0)
	{
		if (abs(m_vDir.x) > abs(m_vDir.z))
			m_vDir.z += m_vDir.z*0.25f;
		else
			m_vDir.x += m_vDir.x*0.25f;
	}
	else if (Engine::CBulletMgr::GetInstance()->Get_Bullet_List(BULLET_ID::SMOKE_BULLET)->size() == 1)
	{
		if (abs(m_vDir.x) > abs(m_vDir.z))
			m_vDir.z += m_vDir.z*0.5f;
		else
			m_vDir.x += m_vDir.x*0.5f;
	}
	else if (Engine::CBulletMgr::GetInstance()->Get_Bullet_List(BULLET_ID::SMOKE_BULLET)->size() == 2)
	{
		if (abs(m_vDir.x) > abs(m_vDir.z))
			m_vDir.z -= m_vDir.z*0.25f;
		else
			m_vDir.x -= m_vDir.x*0.25f;
	}
	else if (Engine::CBulletMgr::GetInstance()->Get_Bullet_List(BULLET_ID::SMOKE_BULLET)->size() == 3)
	{
		if (abs(m_vDir.x) > abs(m_vDir.z))
			m_vDir.z -= m_vDir.z*0.5f;
		else
			m_vDir.x -= m_vDir.x*0.5f;
	}
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransformCom->Rotation(ROT_Y, AngleY);
}

CSmoke_Bullet * CSmoke_Bullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSmoke_Bullet*		pInstance = new CSmoke_Bullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CSmoke_Bullet Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSmoke_Bullet::Free(void)
{
	__super::Free();
}
