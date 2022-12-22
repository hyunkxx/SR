#include "stdafx.h"
#include "..\Header\StdEnemy.h"
#include "Export_Function.h"
#include"TankManager.h"
#include "RushMode.h"
CStdEnemy::CStdEnemy(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	ZeroMemory(&m_EData, sizeof(EData*));
}

CStdEnemy::CStdEnemy(const CStdEnemy & rhs)
	: CGameObject(rhs)
{
}

CStdEnemy::~CStdEnemy()
{
	Free();
}

HRESULT CStdEnemy::Ready_Object(void)
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

HRESULT CStdEnemy::Ready_Object(void * pArg)
{

	m_EData = (EData*)pArg;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	switch (m_EData->TankType)
	{
	case TANKTYPE::HUMVEE:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::HUMVEE);
		Temp = BULLET_ID::ENEMY_MASHINE_BULLET;
		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		Range = 50.f;
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformCom->Set_Pos(m_EData->vPos.x, 2.f*0.7f, m_EData->vPos.z);
		m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformHead->Set_Pos(m_EData->vPos.x, 2.f*0.7f, m_EData->vPos.z);
		m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformPosin->Set_Pos(m_EData->vPos.x, 2.f*0.7f, m_EData->vPos.z);
		break;
	}
	case TANKTYPE::SMALL_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::SMALL_TANK);
		Temp = BULLET_ID::ENEMY_SMALL_CANNONBALL;
		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		Range = 65.f;
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformCom->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);
		m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformHead->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);
		m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformPosin->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);
		break;
	}
	case TANKTYPE::MIDDLE_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::MIDDLE_TANK);
		Temp = BULLET_ID::ENEMY_MIDDLE_CANNONBALL;
		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		Range = 75.f;
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformCom->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);
		m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformHead->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);
		m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformPosin->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);
		break;
	}
	case TANKTYPE::BIG_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::BIG_TANK);
		Temp = BULLET_ID::ENEMY_BIG_CANNONBALL;
		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = 500.f;
		Range = 130.f;
		m_pTransformCom->Set_Scale(1.2f, 1.2f, 1.2f);
		m_pTransformCom->Set_Pos(m_EData->vPos.x, 1.f*1.2f, m_EData->vPos.z);
		m_pTransformHead->Set_Scale(1.2f, 1.2f, 1.2f);
		m_pTransformHead->Set_Pos(m_EData->vPos.x, 1.f*1.2f, m_EData->vPos.z);
		m_pTransformPosin->Set_Scale(1.2f, 1.2f, 1.2f);
		m_pTransformPosin->Set_Pos(m_EData->vPos.x, 1.f*1.2f, m_EData->vPos.z);

		m_stBody.fLen[x] = 5.f;
		m_stBody.fLen[y] = 6.f;
		m_stBody.fLen[z] = 8.f;
		break;
	}
	case TANKTYPE::LONG_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::LONG_TANK);

		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		break;
	}
	default:
		break;

	}


	m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(180.f));
	m_pTransformHead->Rotation(ROTATION::ROT_Y, D3DXToRadian(180.f));
	m_pTransformPosin->Rotation(ROTATION::ROT_Y, D3DXToRadian(180.f));

	__super::Ready_Object();
	return S_OK;
}

_int CStdEnemy::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);
	if (DeadMotionCheck)
	{
		if (!m_bDeadCountAdd)
		{
			CRushMode::GetInstance()->m_nDeadCount++;
			if (CRushMode::GetInstance()->m_nDeadCount >= 8)
			{
				CRushMode::GetInstance()->m_bBossReady = true;
			}
			m_bDeadCountAdd = true;
		}

		Dead_Motion(fTimeDelta);
	}
	else
	{
		m_fReloadTime += fTimeDelta;
		Detect(fTimeDelta);
	}

	if (m_bDeadTime >= 3.f)
	{
		DeadMotionCheck = false;
		m_bDead = true;

	}
	Update_OBB();
	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CStdEnemy::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	if (Deadtest)
	{

		DeadMotionCheck = true;
		m_stBody.fLen[x] = 0.f;
		m_stBody.fLen[y] = 0.f;
		m_stBody.fLen[z] = 0.f;

		m_stHead.fLen[x] = 0.f;
		m_stHead.fLen[y] = 0.f;
		m_stHead.fLen[z] = 0.f;
	}

}

void CStdEnemy::Render_Object(void)
{
	if (DeadMotionCheck)
	{
		m_pHead->Change_Color_Dead();
		m_pBody->Change_Color_Dead();
		m_pPosin->Change_Color_Dead();
	}
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pHead->Render(m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pBody->Render(m_pTransformHead->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());

	if (DeadMotionCheck)
	{
		m_pHead->Return_Color();
		m_pBody->Return_Color();
		m_pPosin->Return_Color();
	}
}

OBB * CStdEnemy::Get_OBB(void)
{
	return &m_stBody;
}

void CStdEnemy::Move_Info(_vec3 _Info)
{
	m_pTransformCom->Move_Pos(&_Info);
	m_pTransformHead->Move_Pos(&_Info);
	m_pTransformPosin->Move_Pos(&_Info);
}

void CStdEnemy::OBB_Collision_EX(void)
{
	//건물 충돌
}

const _vec3 CStdEnemy::Get_Info(void)
{
	_vec3 Info;
	m_pTransformCom->Get_Info(INFO_POS, &Info);

	return Info;
}

void CStdEnemy::Update_OBB(void)
{
	_vec3 Pos, Right, Up, Look;

	m_pTransformCom->Get_Info(INFO_POS, &m_stBody.vPos);
	m_pTransformCom->Get_Info(INFO_RIGHT, &m_stBody.vDir[x]);
	m_pTransformCom->Get_Info(INFO_UP, &m_stBody.vDir[y]);
	m_pTransformCom->Get_Info(INFO_LOOK, &m_stBody.vDir[z]);

	for (_int i = 0; i < 3; i++)
		D3DXVec3Normalize(&m_stBody.vDir[i], &m_stBody.vDir[i]);

	m_pTransformHead->Get_Info(INFO_POS, &m_stHead.vPos);
	m_pTransformHead->Get_Info(INFO_RIGHT, &m_stHead.vDir[x]);
	m_pTransformHead->Get_Info(INFO_UP, &m_stHead.vDir[y]);
	m_pTransformHead->Get_Info(INFO_LOOK, &m_stHead.vDir[z]);

	for (_int i = 0; i < 3; i++)
		D3DXVec3Normalize(&m_stHead.vDir[i], &m_stHead.vDir[i]);
}

void CStdEnemy::Detect(_float fTimeDelta)
{

	CTransform*		pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	if (pPlayerTransform == nullptr)
		return;
	_float fPlayerCol = Dist(pPlayerTransform);
	if (fPlayerCol <= Range)
	{
		m_Action = AIACTION::AIACTION_BATTLE;
		_vec3 Pos, Dir, vLook, vEHPos, TargetPos;
		pPlayerTransform->Get_Info(INFO::INFO_POS, &TargetPos);
		m_pTransformPosin->Get_Info(INFO::INFO_POS, &vEHPos);

		Dir = TargetPos - vEHPos;

		m_pTransformPosin->Get_Info(INFO::INFO_LOOK, &vLook);
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
			m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta* 5.f);
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 5.f);
		}
		else
		{
			m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta* 5.f);
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 5.f);
		}
		m_pTransformPosin->Get_Info(INFO_POS, &Pos);
		m_pTransformPosin->Get_Info(INFO_LOOK, &vLook);
		Dir = TargetPos - Pos;
		D3DXVec3Normalize(&Dir, &Dir);
		D3DXVec3Normalize(&vLook, &vLook);
		Pos += Dir* 3.f*fPosinDist;
		Pos.y += 1.f;

		_float fRadian = acosf(D3DXVec3Dot(&Dir, &vLook));

		if (m_fReloadTime > m_fReload && abs(fRadian) < D3DXToRadian(10.f))
		{
			if (fabs(Angle) < 4.f)
			{
				Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::ENEMY_CANNONBALL);
				m_fReloadTime = 0.f;

			}
		}
	}
}



_float CStdEnemy::Dist(CTransform * _Target)
{
	_vec3 vTargetPos, vPos, vCol, _vAPos;
	_Target->Get_Info(INFO::INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	vCol = vPos - vTargetPos;
	_float Dist = D3DXVec3Length(&vCol);
	return Dist;
}

_bool CStdEnemy::Left_RightCheck(_vec3 _vDir, _vec3 _vLook)
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

HRESULT CStdEnemy::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	if (m_EData->TankType == TANKTYPE::HUMVEE)
	{
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Humvee_enemy_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Humvee_enemy_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Humvee_enemy_posin");
		NULL_CHECK_RETURN(m_pPosin, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });
	}
	else if (m_EData->TankType == TANKTYPE::SMALL_TANK)
	{
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Small_enemy_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Small_enemy_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Small_enemy_posin");
		NULL_CHECK_RETURN(m_pPosin, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });
	}
	else if (m_EData->TankType == TANKTYPE::MIDDLE_TANK)
	{
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Middle_enemy_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Middle_enemy_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Middle_enemy_posin");
		NULL_CHECK_RETURN(m_pPosin, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });
	}
	else if (m_EData->TankType == TANKTYPE::BIG_TANK)
	{
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"M1_enemy_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"M1_enemy_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"M1_enemy_posin");
		NULL_CHECK_RETURN(m_pPosin, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });
	}
	else if (m_EData->TankType == TANKTYPE::LONG_TANK)
	{
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Long_enemy_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Long_enemy_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Long_enemy_posin");
		NULL_CHECK_RETURN(m_pPosin, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });
	}

	//이동
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTransformPosin = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PosinTransform", pComponent });

	pComponent = m_pTransformHead = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_HeadTransform", pComponent });



	return S_OK;
}

void CStdEnemy::Dead_Motion(const _float & fTimeDelta)
{
	_vec3 Pos;
	m_pTransformCom->Get_Info(INFO_POS, &Pos);
	m_bDeadTime += fTimeDelta;
	fAccum += fTimeDelta;
	if (0.4f < Pos.y && 0 == m_iMotionCount)
	{

		_float x = 5.f * fTimeDelta;
		_float y = (100.f * fTimeDelta) - (0.5f * 9.8f * fAccum*fAccum);
		_vec3 Move = { x , y ,0.f };
		m_pTransformCom->Move_Pos(&Move);
		m_pTransformPosin->Move_Pos(&Move);
		m_pTransformCom->Rotation(ROT_X, 500 * fTimeDelta);
		m_pTransformPosin->Rotation(ROT_X, 500 * fTimeDelta);
	}

	else
	{
		m_iMotionCount++;
		Pos.y = 0.8f;
		m_pTransformCom->Set_Pos(Pos.x, Pos.y, Pos.z);
		m_pTransformPosin->Set_Pos(Pos.x, Pos.y, Pos.z);
	}
}

CStdEnemy * CStdEnemy::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStdEnemy*		pInstance = new CStdEnemy(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CStdEnemy Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CStdEnemy * CStdEnemy::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CStdEnemy*		pInstance = new CStdEnemy(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		MSG_BOX("CTurret Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStdEnemy::Free(void)
{
	__super::Free();
}
