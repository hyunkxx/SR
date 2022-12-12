#include "stdafx.h"
#include "..\Header\BottomDirAlly.h"
#include "Export_Function.h"
//Location
#include"RightLocation.h"
#include"LeftTopLocation.h"
#include"LeftLocation.h"
#include"RightTopLocation.h"
#include"BottomDirEnermy.h"
#include"Default_Enermy.h"	
#include "TankCamera.h"
#include "StaticCamera.h"
#include "AimCamera.h"

CBottomDirAlly::CBottomDirAlly(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBottomDirAlly::CBottomDirAlly(const CBottomDirAlly & rhs)
	: CGameObject(rhs)
{
}

CBottomDirAlly::~CBottomDirAlly()
{
	Free();
}

HRESULT CBottomDirAlly::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);

	m_pTransformCom->Set_Pos(10.f, 1.f, 10.f);
	m_pTransformHead->Set_Pos(10.f, 1.f, 10.f);
	m_pTransformPosin->Set_Pos(10.f, 1.f, 10.f);

	//UI_HP
	UI_Orgin_HP = UI_fHP = 300.f;    // tankData.fMaxHP;
	UI_fOrgin_ScaleX = UI_fScaleX = 2.f;
	UI_fScaleY = 0.2f;
	UI_fScaleZ = 1.f;

	return S_OK;
}

HRESULT CBottomDirAlly::Ready_Object(void * pArg)
{
	m_EData = (EData*)pArg;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	switch (m_EData->TankType)
	{
	case TANKTYPE::HUMVEE:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::HUMVEE);
		TempBullet = 1;
		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fAccel_top_speed = data.fAccel_top_speed*0.5f;
		RotSpped = data.RotSpeed;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		Range = 50.f;
		break;
	}
	case TANKTYPE::SMALL_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::SMALL_TANK);

		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fAccel_top_speed = data.fAccel_top_speed*0.5f;
		RotSpped = data.RotSpeed;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		Range = 65.f;
		break;
	}
	case TANKTYPE::MIDDLE_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::MIDDLE_TANK);

		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fAccel_top_speed = data.fAccel_top_speed*0.5f;
		RotSpped = data.RotSpeed;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		Range = 75.f;
		break;
	}
	case TANKTYPE::BIG_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::BIG_TANK);

		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fAccel_top_speed = data.fAccel_top_speed*0.5f;
		RotSpped = data.RotSpeed;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		Range = 85.f;
		break;
	}
	case TANKTYPE::LONG_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::LONG_TANK);

		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fAccel_top_speed = data.fAccel_top_speed*0.5f;
		RotSpped = data.RotSpeed;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		break;
	}
	default:
		break;

	}
	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformCom->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);
	m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformHead->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);
	m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformPosin->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);

	//UI_HP
	UI_Orgin_HP = UI_fHP = 300.f;    // tankData.fMaxHP;
	UI_fOrgin_ScaleX = UI_fScaleX = 2.f;
	UI_fScaleY = 0.2f;
	UI_fScaleZ = 1.f;
	//충돌
	m_stBody.fLen[x] = 1.5f;
	m_stBody.fLen[y] = 0.7f;
	m_stBody.fLen[z] = 2.5f;

	m_stHead.fLen[x] = 1.f;
	m_stHead.fLen[y] = 1.f;
	m_stHead.fLen[z] = 1.9f;
	__super::Ready_Object();

	return S_OK;
}

_int CBottomDirAlly::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);
	m_fReloadTime += fTimeDelta;

	if (ColBuild != true)
	{
		StateCheck();
		Detect(fTimeDelta);
		Basic(fTimeDelta);
	}
	else
	{
		++ColBuildCount;
		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		m_pTransformCom->Move_Pos(&(vLook*fAccel_top_speed*fTimeDelta*0.5f));
		if (ColBuildCount >= 140)
		{
			ColBuildCount = 0;
			ColBuild = false;
		}
	}
	_vec3 vTrans;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vTrans);
	m_pTransformHead->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
	m_pTransformPosin->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
	Add_RenderGroup(RENDER_NONALPHA, this);
	Update_OBB();
	return OBJ_NOEVENT;
}

void CBottomDirAlly::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	//Update_UI();
}

void CBottomDirAlly::Render_Object(void)
{
	if (m_iAction != AIACTION::AIACTION_DEFENSE)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	}
	m_pBody->Render(m_pTransformCom->Get_WorldMatrix());
	if (m_iAction != AIACTION::AIACTION_DEFENSE)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	}
	m_pHead->Render(m_pTransformHead->Get_WorldMatrix());
	if (m_iAction != AIACTION::AIACTION_DEFENSE)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	}
	m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &UI_matViewF);

	m_pTextureF->Set_Texture(0);
	m_pRcTexF->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CBottomDirAlly::StateCheck()
{
	_vec3	vPos;
	srand((unsigned int)time(NULL));
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	//RightLocation

	if (vPos.x >VTXITV* VTXCNTX / 2.f && vPos.z < VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bRightLocationCount == false)
		{
			m_vPatrol.x = (_float)(VTXITV* VTXCNTX*0.5 + rand() % 230 + 60.f);
			m_vPatrol.y = 0.f;
			m_vPatrol.z = (_float)(VTXITV* VTXCNTX*0.5 - rand() % 200 - 10.f);
			m_bRightLocationCount = true;
		}
		m_bRightTopLocationCount = false;
		m_bLeftLocationCount = false;
		m_bLeftTopLocationCount = false;

		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"RightLocation");
		CRightLocation* LTemp = dynamic_cast<CRightLocation*>(Dest);
		m_iLocationState = LTemp->Get_LocationState();


	}

	//LeftLocation
	else if (vPos.x < (VTXITV*VTXCNTX) / 2.f && vPos.z < (VTXITV*VTXCNTZ) / 2.f)
	{

		if (m_bLeftLocationCount == false)
		{
			m_vPatrol.x = (float)((rand() % 220) + 30.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (float)(rand() % 200 + 25.f);
			m_bLeftLocationCount = true;
		}
		m_bRightTopLocationCount = false;
		m_bRightLocationCount = false;
		m_bLeftTopLocationCount = false;
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"LeftLocation");
		CLeftLocation* LTemp = dynamic_cast<CLeftLocation*>(Dest);
		m_iLocationState = LTemp->Get_LocationState();
	}
	//RightTopLocation
	else if (vPos.x >VTXITV*VTXCNTX / 2.f && vPos.z >VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bRightTopLocationCount == false)
		{
			m_vPatrol.x = VTXITV* VTXCNTX - rand() % 50 - 45.f;
			m_vPatrol.y = 0;
			m_vPatrol.z = VTXITV* VTXCNTX - rand() % 50 - 45.f;

			m_bRightTopLocationCount = true;
		}
		m_bRightLocationCount = false;
		m_bLeftLocationCount = false;
		m_bLeftTopLocationCount = false;


		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"RightTopLocation");
		CRightTopLocation* LTemp = dynamic_cast<CRightTopLocation*>(Dest);
		m_iLocationState = LTemp->Get_LocationState();
	}
	//LeftTopLocation
	else if (vPos.x < VTXITV*VTXCNTX / 2.f && vPos.z > VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bLeftTopLocationCount == false)
		{
			m_vPatrol.x = (VTXITV*VTXCNTX / 2.f - rand() % 13 + 5.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 20 + VTXITV*VTXCNTZ / 2.f + 20.f);

			m_bLeftTopLocationCount = true;
		}
		m_bRightTopLocationCount = false;
		m_bLeftLocationCount = false;
		m_bRightLocationCount = false;

		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"LeftTopLocation");
		CLeftTopLocation* LTemp = dynamic_cast<CLeftTopLocation*>(Dest);
		m_iLocationState = LTemp->Get_LocationState();
	}
}

const _vec3 CBottomDirAlly::Get_Info(void)
{
	_vec3 Info;
	m_pTransformCom->Get_Info(INFO_POS, &Info);

	return Info;
}

void CBottomDirAlly::Move_Info(_vec3 _Info)
{
	m_pTransformCom->Move_Pos(&_Info);
	m_pTransformHead->Move_Pos(&_Info);
	m_pTransformPosin->Move_Pos(&_Info);
}

void CBottomDirAlly::OBB_Collision_EX(void)
{
	if (m_bOnce == false)
	{
		m_bOnce = true;
	}
	else
	{
		m_iAction = AIACTION::AIACTION_DEFENSE;
		m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(40.f));
		m_pTransformHead->Rotation(ROTATION::ROT_Y, D3DXToRadian(40.f));
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, D3DXToRadian(40.f));

	}

}

void CBottomDirAlly::Update_OBB(void)
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

OBB * CBottomDirAlly::Get_OBB(void)
{
	return &m_stBody;
}

void CBottomDirAlly::Detect(_float fTimeDelta)
{

	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
	vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
	if (Temps.size() != 0)
	{
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			CTransform*	pEnermyTransform = static_cast<CTransform*> (static_cast<CBottomDirEnermy*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
			_float fEnemyCol = Dist(pEnermyTransform);

			if (fEnemyCol <= Range)
			{
				m_iAction = AIACTION::AIACTION_BATTLE;
				_vec3 Pos, Dir, vLook, vEHPos, TargetPos;
				m_pTransformHead->Get_Info(INFO::INFO_POS, &vEHPos);
				pEnermyTransform->Get_Info(INFO::INFO_POS, &TargetPos);
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
				Pos += Dir*fPosinDist* 3.f;
				if (abs(D3DXToDegree(Angle)) < 4.f)
				{
					if (m_fReloadTime > m_fReload&&TempBullet == 1)
					{
						Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::MASHINE_BULLET);
						m_fReloadTime = 0.f;
					}
					else if (m_fReloadTime > m_fReload&&TempBullet != 1)
					{
						Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
						m_fReloadTime = 0.f;
					}
				}
			}
		}
	}
	if (Sours.size() != 0)
	{
		for (auto& iter = Sours.begin(); iter < Sours.end(); ++iter)
		{
			CTransform*	pEnermyTransform = static_cast<CTransform*> (static_cast<CDefault_Enermy*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
			_float fEnemyCol = Dist(pEnermyTransform);

			if (fEnemyCol <= Range)
			{
				m_iAction = AIACTION::AIACTION_BATTLE;
				_vec3 Pos, Dir, vLook, vEHPos, TargetPos;
				m_pTransformHead->Get_Info(INFO::INFO_POS, &vEHPos);
				pEnermyTransform->Get_Info(INFO::INFO_POS, &TargetPos);
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
				Pos += Dir* fPosinDist*3.f;
				if (abs(D3DXToDegree(Angle)) < 4.f)
				{
					if (m_fReloadTime > m_fReload&&TempBullet == 1)
					{
						Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::MASHINE_BULLET);
						m_fReloadTime = 0.f;
					}
					else if (m_fReloadTime > m_fReload&&TempBullet != 1)
					{
						Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
						m_fReloadTime = 0.f;
					}
				}

			}
		}
	}

}

void CBottomDirAlly::ObjectCol(_bool m_Left)
{
	_bool bLeft = m_Left;
	if (bLeft)
	{
		m_pTransformHead->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90.f));
		m_pTransformCom->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90.f));
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90.f));
		ColBuild = true;
	}
	else
	{
		ColBuild = true;
		m_pTransformHead->Rotation(ROTATION::ROT_Y, D3DXToRadian(90.f));
		m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(90.f));
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, D3DXToRadian(90.f));
	}
}

_float CBottomDirAlly::Dist(CTransform * _Target)
{
	_vec3 vTargetPos, vPos, vCol, _vAPos;
	_Target->Get_Info(INFO::INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	vCol = vPos - vTargetPos;
	_float Dist = D3DXVec3Length(&vCol);
	return Dist;
}

void CBottomDirAlly::Basic(_float fTimeDelta)
{
	switch (m_iAction)
	{
	case AIACTION::AIACTION_WAIT:
		Wait(fTimeDelta);
		break;
	case AIACTION::AIACTION_OCCOPATION:
		Occupation(fTimeDelta);
		break;
	case AIACTION::AIACTION_ENERMY_IN_AREA:
		Enermy_In_Area(fTimeDelta);
		break;
	}
}

_bool CBottomDirAlly::Left_RightCheck(_vec3 _vDir, _vec3 _vLook)
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

void CBottomDirAlly::Occupation(_float fTimeDelta)
{

	_vec3  vPos, vLook, vDir;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	if (m_bPatrol == false)
	{
		vDir = m_vPatrol - vPos;
	}
	else
	{
		vDir = vPatrolRange - vPos;
	}
	if (m_vPatrol.x - 3.f <= vPos.x && (m_vPatrol.z - 3.f <= vPos.z) && m_bPatrol == false)
	{
		vPatrolRange.x = m_vPatrol.x + 40.f + rand() % 70;
		vPatrolRange.y = 0;
		vPatrolRange.z = m_vPatrol.z;
		m_bPatrol = true;
	}
	if (vPatrolRange.x - 3.f <= vPos.x && (vPatrolRange.z >= vPos.z) && m_bPatrol == true)
	{
		m_bPatrol = false;
	}
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vDir, &vDir);

	Left_RightCheck(vDir, vLook);

	_float Dot = D3DXVec3Dot(&vLook, &vDir);
	_float AngleTemp = (float)acosf(Dot);
	if (isnan(AngleTemp))
	{
		AngleTemp = 0;
	}
	if (LeftCheck == false)
	{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta);

	}
	else
	{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta);
	}


	_vec3 vTrans2;
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
	m_pTransformCom->Move_Pos(&(vTrans2*fAccel_top_speed*fTimeDelta));

}

void CBottomDirAlly::Wait(_float fTimeDelta)
{

	_vec3   vTemp, vSour, vDist, vDir, vLook, vTrans2;
	_float Angle = 0.f, Dot = 0.f;
	switch (m_PastLocation)
	{
	case LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP:
	{		vTemp = { 0.f,0.f,m_vPatrol.z };
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vSour);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	vDir = vTemp - vSour;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);

	Left_RightCheck(vDir, vLook);

	Dot = D3DXVec3Dot(&vDir, &vLook);

	Angle = (float)acosf(Dot);
	if (isnan(Angle))
	{
		Angle = 0;
	}
	if (LeftCheck == false)
	{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
	}
	else if (LeftCheck == true)
	{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
	}

	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
	m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*fAccel_top_speed));
	break;
	}
	case LOCATIONCHECK::LOCATIONCHECK_LEFT:
	{	vTemp = { VTXITV*VTXCNTX ,0.f,m_vPatrol.z };
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vSour);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	vDir = vTemp - vSour;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);

	Left_RightCheck(vDir, vLook);
	Dot = D3DXVec3Dot(&vDir, &vLook);
	Angle = (float)acosf(Dot);

	if (isnan(Angle))
	{
		Angle = 0;
	}
	if (LeftCheck == false)
	{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
	}
	else
	{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
	}
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
	m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*fAccel_top_speed));

	break;
	}
	case  LOCATIONCHECK::LOCATIONCHECK_RIGHT:
	{	vTemp = { m_vPatrol.x ,0.f,VTXITV*VTXCNTZ };
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vSour);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	vDir = vTemp - vSour;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);

	Left_RightCheck(vDir, vLook);
	Dot = D3DXVec3Dot(&vDir, &vLook);
	Angle = (float)acosf(Dot);

	if (isnan(Angle))
	{
		Angle = 0;
	}
	if (LeftCheck == false)
	{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
	}
	else
	{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
	}
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
	m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*fAccel_top_speed));

	break;
	}
	default:
		break;
	}
}
void CBottomDirAlly::Enermy_In_Area(_float fTimeDelta)
{
	_bool test = false;
	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
	vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);


	for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
	{
		CTransform*	pDefaultEnermyTransform = static_cast<CTransform*> (static_cast<CBottomDirEnermy*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
		_float fAllyCol = Dist(pDefaultEnermyTransform);

		if (fAllyCol <= 150.f)
		{
			if (dynamic_cast<CBottomDirEnermy*>(*iter)->Get_LocationCheck() == m_iLocationCheck)
			{
				test = true;
				_vec3 vTargetPos, vLook, vPos;
				pDefaultEnermyTransform->Get_Info(INFO::INFO_POS, &vTargetPos);
				m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
				m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
				_vec3 vDir = vTargetPos - vPos;
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vLook, &vLook);
				Left_RightCheck(vDir, vLook);
				_float Dot = D3DXVec3Dot(&vDir, &vLook);
				_float Angle = acosf(Dot);
				if (isnan(Angle))
				{
					Angle = 0;
				}
				if (LeftCheck == false)
				{
					m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
					m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
					m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
				}
				else
				{
					m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
					m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
					m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
				}
			}
		}
	}
	if (test == false)
	{
		Occupation(fTimeDelta);
	}
	else
	{
		_vec3 vTrans2;
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
		m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*fAccel_top_speed));
	}
}

CBottomDirAlly* CBottomDirAlly::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBottomDirAlly*		pInstance = new CBottomDirAlly(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CDefault_Ally Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBottomDirAlly * CBottomDirAlly::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CBottomDirAlly*		pInstance = new CBottomDirAlly(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		MSG_BOX("CDefault_Ally Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CBottomDirAlly::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	//탱크 몸통
	if (m_EData->TankType == TANKTYPE::HUMVEE)
	{
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Humvee_ally_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Humvee_ally_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Humvee_ally_posin");
		NULL_CHECK_RETURN(m_pPosin, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });
	}
	else if (m_EData->TankType == TANKTYPE::SMALL_TANK)
	{
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Small_ally_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Small_ally_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Small_ally_posin");
		NULL_CHECK_RETURN(m_pPosin, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });
	}
	else if (m_EData->TankType == TANKTYPE::MIDDLE_TANK)
	{
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Middle_ally_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Middle_ally_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Middle_ally_posin");
		NULL_CHECK_RETURN(m_pPosin, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });
	}
	else if (m_EData->TankType == TANKTYPE::BIG_TANK)
	{
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Big_ally_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Big_ally_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Big_ally_posin");
		NULL_CHECK_RETURN(m_pPosin, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });
	}
	else if (m_EData->TankType == TANKTYPE::LONG_TANK)
	{
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Long_ally_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Long_ally_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Long_ally_posin");
		NULL_CHECK_RETURN(m_pPosin, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });
	}
	//탱크 움직임

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTransformHead = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_HeadTransform", pComponent });

	pComponent = m_pTransformPosin = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PosinTransform", pComponent });


	pComponent = m_pRcTexF = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTexF, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureF = static_cast<CTexture*>(Clone_Prototype(L"Proto_World_Hp_Tex"));
	NULL_CHECK_RETURN(m_pTextureF, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_World_Hp_Tex", pComponent });

	pComponent = m_pTransformHP_UI = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHP_UI, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_WHP2", pComponent });

	return S_OK;
}

void CBottomDirAlly::Free(void)
{
	__super::Free();
}


void CBottomDirAlly::Update_UI(void)
{
	//CGameObject* pTankView = Engine::Get_Object(L"Environment", L"TankCamera");
	//CGameObject* pStaticView = Engine::Get_Object(L"Environment", L"StaticCamera");
	//CGameObject* pAimView = Engine::Get_Object(L"Environment", L"AimCamera");

	if (UI_fHP >= UI_Orgin_HP)
	{
		UI_fHP = UI_Orgin_HP;
	}
	if (UI_fHP <= 0.f)
	{
		UI_fHP = 0.f;
	}

	_float HP_Percent = (UI_fHP / UI_Orgin_HP);

	if (HP_Percent > 1.f)
	{
		HP_Percent = 1.f;
	}
	if (HP_Percent < 0.f)
	{
		HP_Percent = 0.f;
	}

	UI_fScaleX = (UI_fOrgin_ScaleX * HP_Percent);

	_vec3 vTankPos, vUI_HPF;
	// UI_ 높이 _ 키워드
	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		UI_fScaleX = 2.f;
		UI_fScaleY = 0.2f;

		m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY, UI_fScaleZ);

		m_pTransformCom->Get_Info(INFO_POS, &vTankPos);

		vUI_HPF = { vTankPos.x, vTankPos.y + 3.f, vTankPos.z };

		Engine::Get_Camera()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &UI_matViewF);
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::DRONE_CAMERA)
	{
		UI_fScaleX = 3.f;
		UI_fScaleY = 0.4f;

		m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY, UI_fScaleZ);

		m_pTransformCom->Get_Info(INFO_POS, &vTankPos);

		vUI_HPF = { vTankPos.x, vTankPos.y + 3.5f, vTankPos.z + 3.f };

		Engine::Get_Camera()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &UI_matViewF);
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
	{
		m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY, UI_fScaleZ);

		m_pTransformCom->Get_Info(INFO_POS, &vTankPos);

		vUI_HPF = { vTankPos.x, vTankPos.y + 2.5f, vTankPos.z };

		Engine::Get_Camera()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &UI_matViewF);
	}

	memset(&UI_matViewF._41, 0, sizeof(_vec3));

	D3DXMatrixInverse(&UI_matViewF, 0, &UI_matViewF);

	_vec3 BillPos = vUI_HPF;

	_float fScale[ROT_END];

	fScale[ROT_X] = UI_fScaleX;
	fScale[ROT_Y] = UI_fScaleY;
	fScale[ROT_Z] = UI_fScaleZ;

	memcpy(&UI_matViewF._41, &BillPos, sizeof(_vec3));

	for (int i = 0; i < ROT_END; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			UI_matViewF(i, j) *= fScale[i];
		}
	}
}
