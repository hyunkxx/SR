#include "stdafx.h"
#include "..\Header\Default_Ally.h"
#include "Export_Function.h"

//Location
#include"RightLocation.h"
#include"LeftTopLocation.h"
#include"LeftLocation.h"
#include"RightTopLocation.h"
#include"BottomDirEnermy.h"
#include"Default_Enermy.h"	
CDefault_Ally::CDefault_Ally(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_bRightLocationCount(false)
{
}

CDefault_Ally::CDefault_Ally(const CDefault_Ally & rhs)
	: CGameObject(rhs)
{
}

CDefault_Ally::~CDefault_Ally()
{
	Free();
}

HRESULT CDefault_Ally::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);

	m_pTransformCom->Set_Pos(10.f, 1.f, 10.f);
	m_pTransformHead->Set_Pos(10.f, 1.f, 10.f);
	m_pTransformPosin->Set_Pos(10.f, 1.f, 10.f);

	return S_OK;
}

HRESULT CDefault_Ally::Ready_Object(void * pArg)
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

	m_stBody.fLen[x] = 1.5f;
	m_stBody.fLen[y] = 0.7f;
	m_stBody.fLen[z] = 2.5f;

	m_stHead.fLen[x] = 1.f;
	m_stHead.fLen[y] = 1.f;
	m_stHead.fLen[z] = 1.9f;

	__super::Ready_Object();
	return S_OK;
}

_int CDefault_Ally::Update_Object(const _float& fTimeDelta)
{

	__super::Update_Object(fTimeDelta);

	StateCheck();
	Detect(fTimeDelta);
	Basic(fTimeDelta);

	m_fReloadTime += fTimeDelta;

	_vec3 vTrans;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vTrans);
	m_pTransformHead->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
	m_pTransformPosin->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
	Update_OBB();
	CGameObject::Ready_Object();
	return OBJ_NOEVENT;
}

void CDefault_Ally::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_NONALPHA, this);
}

void CDefault_Ally::Render_Object(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBody->Render(m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pHead->Render(m_pTransformHead->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());
}

const _vec3 CDefault_Ally::Get_Info(void)
{
	_vec3 Info;
	m_pTransformCom->Get_Info(INFO_POS, &Info);

	return Info;
}

void CDefault_Ally::Move_Info(_vec3 _Info)
{
	m_pTransformCom->Move_Pos(&_Info);
	m_pTransformHead->Move_Pos(&_Info);
	m_pTransformPosin->Move_Pos(&_Info);
}

void CDefault_Ally::OBB_Collision_EX(void)
{
	//扒拱 面倒 贸府
}

void CDefault_Ally::Update_OBB(void)
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

OBB * CDefault_Ally::Get_OBB(void)
{
	return &m_stBody;
}

void CDefault_Ally::StateCheck()
{
	_vec3	vPos;
	srand((unsigned int)time(NULL));
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	//RightLocation

	if (vPos.x >VTXITV* VTXCNTX / 2.f && vPos.z < VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bRightLocationCount == false)
		{
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
			m_vPatrol.z = (float)(rand() % 180 + 55.f);
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
			m_vPatrol.x = (_float)(VTXITV* VTXCNTX - rand() % 40 - 45.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (_float)(VTXITV* VTXCNTX - rand() % 40 - 45.f);

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
	else if (vPos.x < VTXITV*VTXCNTX *0.5 && vPos.z > VTXITV*VTXCNTZ *0.5)
	{
		if (m_bLeftTopLocationCount == false)
		{
			m_vPatrol.x = (_float)(VTXITV*VTXCNTX *0.5 - rand() % 200 - 90.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (_float)(VTXITV*VTXCNTZ *0.5 + rand() % 200 + 40.f);

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

void CDefault_Ally::Detect(_float fTimeDelta)
{

	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
	vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
	if (Temps.size() != 0)
	{
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			CTransform*	pEnermyTransform = static_cast<CTransform*> (static_cast<CDefault_Enermy* > (*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
			_float fEnemyCol = Dist(pEnermyTransform);

			if (fEnemyCol <= 60.f)
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
				Pos += Dir* 3.f*fPosinDist;
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
			CTransform*	pEnermyTransform = static_cast<CTransform*> (static_cast<CBottomDirEnermy*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
			_float fEnemyCol = Dist(pEnermyTransform);

			if (fEnemyCol <= 60.f)
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

_float CDefault_Ally::Dist(CTransform * _Target)
{
	_vec3 vTargetPos, vPos, vCol, _vAPos;
	_Target->Get_Info(INFO::INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	vCol = vPos - vTargetPos;
	_float Dist = D3DXVec3Length(&vCol);
	return Dist;
}

void CDefault_Ally::Basic(_float fTimeDelta)
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
	default:
		break;
	}
}

_bool CDefault_Ally::Left_RightCheck(_vec3 _vDir, _vec3 _vLook)
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

void CDefault_Ally::Occupation(_float fTimeDelta)
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
		vPatrolRange.x = m_vPatrol.x + rand() % 35 + 65.f;
		vPatrolRange.y = 0;
		vPatrolRange.z = m_vPatrol.z + rand() % 30 + 40.f;
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

void CDefault_Ally::Wait(_float fTimeDelta)
{
	if (m_iAction == AIACTION::AIACTION_WAIT)
	{
		_vec3   vTemp, vSour, vDist, vDir, vLook, vTrans2;
		_float Angle = 0.f, Dot = 0.f;
		switch (m_PastLocation)
		{
		case LOCATIONCHECK::LOCATIONCHECK_LEFTTOP:
			vTemp = { VTXITV*VTXCNTX,0.f,m_vPatrol.z };
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

		case LOCATIONCHECK::LOCATIONCHECK_LEFT:
			vTemp = { m_vPatrol.x,0.f,VTXITV*VTXCNTZ };
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
	}
}

void CDefault_Ally::Enermy_In_Area(_float fTimeDelta)
{
	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
	vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
	_bool test = false;
	for (auto& iter = Sours.begin(); iter < Sours.end(); ++iter)
	{

		CTransform*	pDefaultEnermyTransform = static_cast<CTransform*> (static_cast<CDefault_Enermy*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
		_float fAllyCol = Dist(pDefaultEnermyTransform);

		if (fAllyCol <= 150.f)
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

CDefault_Ally* CDefault_Ally::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDefault_Ally*		pInstance = new CDefault_Ally(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CDefault_Ally Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDefault_Ally * CDefault_Ally::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CDefault_Ally*		pInstance = new CDefault_Ally(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		MSG_BOX("CDefault_Ally Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CDefault_Ally::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	//攀农 个烹
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

	//攀农 框流烙

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTransformHead = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_HeadTransform", pComponent });

	pComponent = m_pTransformPosin = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PosinTransform", pComponent });

	return S_OK;
}

void CDefault_Ally::Free(void)
{
	__super::Free();
}