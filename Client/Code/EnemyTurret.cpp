#include "stdafx.h"
#include "..\Header\EnemyTurret.h"
#include "Export_Function.h"
#include"Default_Ally.h"
#include"BottomDirAlly.h"
#include"EnermyMgr.h"
#include "GameMode.h"

CEnemyTurret::CEnemyTurret(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CEnemyTurret::CEnemyTurret(const CEnemyTurret & rhs)
	: CGameObject(rhs)
{
}

CEnemyTurret::~CEnemyTurret()
{
}

HRESULT CEnemyTurret::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransformCom->Set_Scale(2.f, 2.f, 2.f);
	m_pTransformHead->Set_Scale(2.f, 2.f, 2.f);
	m_pTransformPosin->Set_Scale(2.f, 2.f, 2.f);
	_int random = rand() % 20;
	m_pTransformCom->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90 + random));
	m_pTransformHead->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90 + random));
	m_pTransformPosin->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90 + random));

	m_pTransformCom->Set_Pos(605.f, 2.f, 580.f);
	m_pTransformHead->Set_Pos(605.f, 2.f, 580.f);
	m_pTransformPosin->Set_Pos(605.f, 2.f, 580.f);


	return S_OK;
}

HRESULT CEnemyTurret::Ready_Object(void * pArg)
{
	vPos = (_vec3*)pArg;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(2.f, 2.f, 2.f);
	m_pTransformHead->Set_Scale(2.f, 2.f, 2.f);
	m_pTransformPosin->Set_Scale(2.f, 2.f, 2.f);
	_int random = rand() % 20;
	m_pTransformCom->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90 + random));
	m_pTransformHead->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90 + random));
	m_pTransformPosin->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90 + random));
	//580,600,605.f,580.f
	m_pTransformCom->Set_Pos(vPos->x, 2.f, vPos->z);
	m_pTransformHead->Set_Pos(vPos->x, 2.f, vPos->z);
	m_pTransformPosin->Set_Pos(vPos->x, 2.f, vPos->z);
	return S_OK;
}

_int CEnemyTurret::Update_Object(const _float & fTimeDelta)
{
	if (CGameMode::GetInstance()->m_bGameEnd)
		return 0;

	__super::Update_Object(fTimeDelta);

	m_fTime += fTimeDelta;
	m_fReloadTime += fTimeDelta;
	if (m_Action != AIACTION::AIACTION_BATTLE)
	{
		if (m_fTime <= 5 && !m_bTurn)
		{
			m_pTransformHead->Rotation(ROTATION::ROT_Y, -D3DXToRadian(20.f * fTimeDelta));
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, -D3DXToRadian(20.f * fTimeDelta));
		}
		else if (m_fTime <= 5 && m_bTurn)
		{
			m_pTransformHead->Rotation(ROTATION::ROT_Y, D3DXToRadian(20.f * fTimeDelta));
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, D3DXToRadian(20.f * fTimeDelta));
		}
		if (m_fTime >= 5 && !m_bTurn)
		{
			m_fTime = 0.f;
			m_bTurn = true;
		}
		else if (m_fTime >= 5 && m_bTurn)
		{
			m_fTime = 0.f;
			m_bTurn = false;
		}
	}
	Detect(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CEnemyTurret::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

}

void CEnemyTurret::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pHead->Render(m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pBody->Render(m_pTransformHead->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());
}


void CEnemyTurret::Detect(_float fTimeDelta)
{

	CTransform*		pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	if (pPlayerTransform == nullptr)
		return;
	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ALLY);
	vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDALLY);
	_float fPreAllyCol = 3000.f;
	_float fSourCol = 3000.f;
	_float fPlayerCol = Dist(pPlayerTransform);
	CTransform* pCompareTransform = nullptr;
	CTransform* pSourCompareTransform = nullptr;
	if (Temps.size() != 0)
	{
		_bool bReady = false;
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			CTransform*	pAllyTransform = static_cast<CTransform*> (static_cast<CDefault_Ally*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
			if (bReady == false)
			{
				fPreAllyCol = Dist(pAllyTransform);
				pCompareTransform = pAllyTransform;
				bReady = true;
			}
			else
			{
				_float AllyCol = Dist(pAllyTransform);
				if (AllyCol < fPreAllyCol)
				{
					fPreAllyCol = AllyCol;
					pCompareTransform = pAllyTransform;
				}
			}
		}
	}
	if (Sours.size() != 0)
	{
		_bool bReady = false;
		for (auto& iter = Sours.begin(); iter < Sours.end(); ++iter)
		{
			CTransform*	pAllyTransform = static_cast<CTransform*> (static_cast<CBottomDirAlly* > (*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
			if (bReady == false)
			{
				fSourCol = Dist(pAllyTransform);
				pSourCompareTransform = pAllyTransform;
				bReady = true;
			}
			else
			{
				_float AllyCol = Dist(pAllyTransform);
				if (AllyCol < fSourCol)
				{
					fSourCol = AllyCol;
					pSourCompareTransform = pAllyTransform;
				}
			}
		}
	}
	if (Temps.size() != 0 || Sours.size() != 0)
	{
		if (fPlayerCol > fPreAllyCol || fPlayerCol > fSourCol)
		{
			if (fPreAllyCol < fSourCol)
			{
				if (fPreAllyCol <= Range)
				{
					m_Action = AIACTION::AIACTION_BATTLE;
					_vec3 Pos, Dir, vLook, vEHPos, TargetPos;
					m_pTransformHead->Get_Info(INFO::INFO_POS, &vEHPos);
					pCompareTransform->Get_Info(INFO::INFO_POS, &TargetPos);
					Dir = TargetPos - vEHPos;

					m_pTransformHead->Get_Info(INFO::INFO_LOOK, &vLook);

					D3DXVec3Normalize(&vLook, &vLook);
					D3DXVec3Normalize(&Dir, &Dir);

					Left_RightCheck(Dir, vLook);
					_float Dot = D3DXVec3Dot(&vLook, &Dir);
					_float Angle = (float)acosf(Dot);
					if (isnan(Angle))
					{
						Angle = 0;
					}
					if (LeftCheck == false)
					{
						m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
						m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
					}
					else
					{
						m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);
						m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);
					}
					m_pTransformHead->Get_Info(INFO_POS, &Pos);
					m_pTransformHead->Get_Info(INFO_LOOK, &vLook);
					Dir = TargetPos - Pos;
					D3DXVec3Normalize(&Dir, &Dir);
					D3DXVec3Normalize(&vLook, &vLook);
					Pos += Dir* 3.f*fPosinDist;
					Pos.y += 2.f;
					if (m_fReloadTime > m_fReload)
					{
						if (abs(D3DXToDegree(Angle)) < 2.f)
						{
							Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::ENEMY_BIG_CANNONBALL);
							m_fReloadTime = 0.f;
						}
					}
				}
			}
			else if (fPreAllyCol > fSourCol)
			{
				if (fSourCol <= Range)
				{
					m_Action = AIACTION::AIACTION_BATTLE;
					_vec3 Pos, Dir, vLook, vEHPos, TargetPos;
					m_pTransformHead->Get_Info(INFO::INFO_POS, &vEHPos);
					pSourCompareTransform->Get_Info(INFO::INFO_POS, &TargetPos);
					Dir = TargetPos - vEHPos;

					m_pTransformHead->Get_Info(INFO::INFO_LOOK, &vLook);

					D3DXVec3Normalize(&vLook, &vLook);
					D3DXVec3Normalize(&Dir, &Dir);

					Left_RightCheck(Dir, vLook);
					_float Dot = D3DXVec3Dot(&vLook, &Dir);
					_float Angle = (float)acosf(Dot);
					if (isnan(Angle))
					{
						Angle = 0;
					}
					if (LeftCheck == false)
					{
						m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
						m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
					}
					else
					{
						m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);
						m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);
					}
					m_pTransformHead->Get_Info(INFO_POS, &Pos);
					m_pTransformHead->Get_Info(INFO_LOOK, &vLook);
					Dir = TargetPos - Pos;
					D3DXVec3Normalize(&Dir, &Dir);
					D3DXVec3Normalize(&vLook, &vLook);
					Pos += Dir* 3.f*fPosinDist;

					if (m_fReloadTime > m_fReload)
					{
						if (abs(D3DXToDegree(Angle)) < 2.f)
						{
							Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::ENEMY_BIG_CANNONBALL);
							m_fReloadTime = 0.f;
						}
					}
				}
			}
		}
		else if (fPlayerCol <= Range)
		{
			m_Action = AIACTION::AIACTION_BATTLE;
			_vec3 Pos, Dir, vLook, vEHPos, TargetPos;
			pPlayerTransform->Get_Info(INFO::INFO_POS, &TargetPos);
			m_pTransformHead->Get_Info(INFO::INFO_POS, &vEHPos);

			Dir = TargetPos - vEHPos;

			m_pTransformHead->Get_Info(INFO::INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			D3DXVec3Normalize(&Dir, &Dir);

			Left_RightCheck(Dir, vLook);

			_float Dot = D3DXVec3Dot(&vLook, &Dir);
			_float Angle = (float)acosf(Dot);
			if (isnan(Angle))
			{
				Angle = 0;
			}
			if (LeftCheck == false)
			{
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
			}
			else
			{
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
			}
			m_pTransformHead->Get_Info(INFO_POS, &Pos);
			m_pTransformHead->Get_Info(INFO_LOOK, &vLook);
			Dir = TargetPos - Pos;
			D3DXVec3Normalize(&Dir, &Dir);
			D3DXVec3Normalize(&vLook, &vLook);
			Pos += Dir* 3.f*fPosinDist;
			if (m_fReloadTime > m_fReload)
			{
			
					Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::ENEMY_BIG_CANNONBALL);
					m_fReloadTime = 0.f;
				
			}
		}
	}

	if (fPlayerCol <= Range)
	{
		m_Action = AIACTION::AIACTION_BATTLE;
		_vec3 Pos, Dir, vLook, vEHPos, TargetPos;
		pPlayerTransform->Get_Info(INFO::INFO_POS, &TargetPos);
		m_pTransformHead->Get_Info(INFO::INFO_POS, &vEHPos);

		Dir = TargetPos - vEHPos;

		m_pTransformHead->Get_Info(INFO::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Normalize(&Dir, &Dir);

		Left_RightCheck(Dir, vLook);

		_float Dot = D3DXVec3Dot(&vLook, &Dir);
		_float Angle = (float)acosf(Dot);
		if (isnan(Angle))
		{
			Angle = 0;
		}
		if (LeftCheck == false)
		{
			m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
		}
		else
		{
			m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
		}
		m_pTransformHead->Get_Info(INFO_POS, &Pos);
		m_pTransformHead->Get_Info(INFO_LOOK, &vLook);
		Dir = TargetPos - Pos;
		D3DXVec3Normalize(&Dir, &Dir);
		D3DXVec3Normalize(&vLook, &vLook);
		Pos += Dir* 3.f*fPosinDist;
		if (m_fReloadTime > m_fReload)
		{
		
				Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::ENEMY_BIG_CANNONBALL);
				m_fReloadTime = 0.f;
			
		}
	}
}



_float CEnemyTurret::Dist(CTransform * _Target)
{
	_vec3 vTargetPos, vPos, vCol, _vAPos;
	_Target->Get_Info(INFO::INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	vCol = vPos - vTargetPos;
	_float Dist = D3DXVec3Length(&vCol);
	return Dist;
}

_bool CEnemyTurret::Left_RightCheck(_vec3 _vDir, _vec3 _vLook)
{
	_vec3 Up = { 0.f,1.f,0.f }, Temp;
	D3DXVec3Normalize(&Up, &Up);
	D3DXVec3Cross(&Temp, &_vDir, &_vLook);
	_float TempTest = D3DXVec3Dot(&Up, &Temp);

	if (TempTest > 0)
		LeftCheck = false;
	else
		LeftCheck = true;
	return LeftCheck;
}

HRESULT CEnemyTurret::Add_Component(void)
{
	CComponent*		pComponent = nullptr;


	//�̵�
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTransformPosin = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PosinTransform", pComponent });

	pComponent = m_pTransformHead = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_HeadTransform", pComponent });

	//����

	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Turret_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Turret_body", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Turret_head");
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Turret_head", pComponent });

	pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Turret_posin");
	NULL_CHECK_RETURN(m_pPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Turret_posin", pComponent });

	return S_OK;
}

CEnemyTurret * CEnemyTurret::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEnemyTurret*		pInstance = new CEnemyTurret(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CTurret Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEnemyTurret * CEnemyTurret::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CEnemyTurret*		pInstance = new CEnemyTurret(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		MSG_BOX("CTurret Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemyTurret::Free(void)
{
	__super::Free();
}
