#include "stdafx.h"
#include "..\Header\BottomDirEnermy.h"
#include "Export_Function.h"
#include"Management.h"
#include"Layer.h"
#include"TestPlayer.h"
//Location
#include"RightLocation.h"
#include"LeftTopLocation.h"
#include"LeftLocation.h"
#include"RightTopLocation.h"
#include"Default_Ally.h"
#include "TankCamera.h"
#include "StaticCamera.h"
#include "AimCamera.h"
#include"BottomDirAlly.h"
CBottomDirEnermy::CBottomDirEnermy(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBottomDirEnermy::CBottomDirEnermy(const CBottomDirEnermy & rhs)
	: CGameObject(rhs)
{
}

CBottomDirEnermy::~CBottomDirEnermy()
{
	Free();
}

HRESULT CBottomDirEnermy::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformCom->Set_Pos(530.f, 2.f, 530.f);

	m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformHead->Set_Pos(530.f, 2.f, 530.f);

	m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformPosin->Set_Pos(530.f, 2.f, 530.f);

	//UI_HP
	UI_Orgin_HP = UI_fHP = 300.f;    // tankData.fMaxHP;
	UI_fOrgin_ScaleX = UI_fScaleX = 2.f;
	UI_fScaleY = 0.2f;
	UI_fScaleZ = 1.f;

	return S_OK;
}

HRESULT CBottomDirEnermy::Ready_Object(void * pArg)
{
	m_EData = (EData*)pArg;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

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

	m_stBody.fLen[x] = 1.5f;
	m_stBody.fLen[y] = 0.7f;
	m_stBody.fLen[z] = 2.5f;

	m_stHead.fLen[x] = 1.f;
	m_stHead.fLen[y] = 1.f;
	m_stHead.fLen[z] = 1.9f;

	__super::Ready_Object();
	return S_OK;
}

_int CBottomDirEnermy::Update_Object(const _float& fTimeDelta)
{
	m_fPreHp = UI_fHP;
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
		m_pTransformCom->Move_Pos(&(vLook*10.f*fTimeDelta));
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
	Update_OBB();
	Add_RenderGroup(RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

void CBottomDirEnermy::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	//Update_UI();

}

void CBottomDirEnermy::Render_Object(void)
{


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBody->Render(m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pHead->Render(m_pTransformHead->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &UI_matViewF);

	m_pTextureF->Set_Texture(0);
	m_pRcTexF->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

const _vec3 CBottomDirEnermy::Get_Info(void)
{
	_vec3 Info;
	m_pTransformCom->Get_Info(INFO_POS, &Info);

	return Info;
}

void CBottomDirEnermy::Move_Info(_vec3 _Info)
{
	m_pTransformCom->Move_Pos(&_Info);
	m_pTransformHead->Move_Pos(&_Info);
	m_pTransformPosin->Move_Pos(&_Info);
}

void CBottomDirEnermy::OBB_Collision_EX(void)
{
	//건물 충돌
}

void CBottomDirEnermy::Update_OBB(void)
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

OBB * CBottomDirEnermy::Get_OBB(void)
{
	return &m_stBody;
}

void CBottomDirEnermy::StateCheck()
{
	_vec3	vPos;
	srand((unsigned int)time(NULL));
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	//RightLocation
	if (vPos.x > VTXITV*VTXCNTX / 2.f && vPos.z < VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bRightLocationCount == false)
		{
			m_vPatrol.x = (float)(VTXITV*VTXCNTX *0.5f + rand() % 200 + 30.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (float)(VTXITV*VTXCNTZ*0.5f - rand() % 280 + 30.f);
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
	else if (vPos.x < VTXITV*VTXCNTX / 2.f && vPos.z < VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bLeftLocationCount == false)
		{
			m_bLeftLocationCount = true;
			m_vPatrol.x = (float)(rand() % 25);
			m_vPatrol.y = 0;
			m_vPatrol.z = (float)(rand() % 25);
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

			m_vPatrol.x = (rand() % 180 + VTXITV*VTXCNTX / 2.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 180 + VTXITV*VTXCNTZ / 2.f);

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

			//	m_vPatrol.x = (VTXCNTX / 2.f - rand() % 13-10.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 25 + VTXITV*VTXCNTZ / 2.f);
			m_vPatrol.x = 10.f;
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

void CBottomDirEnermy::Detect(_float fTimeDelta)
{
	CTransform*		pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	if (pPlayerTransform == nullptr)
		return;

	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDALLY);
	if (Temps.size() != 0)
	{
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			CTransform*	pAllyTransform = static_cast<CTransform*> (static_cast<CBottomDirAlly*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
			_float fPlayerCol = Dist(pPlayerTransform);
			_float fAllyCol = Dist(pAllyTransform);
			if (fPlayerCol < fAllyCol)
			{
				if (fPlayerCol <= 60.f)
				{
					m_iAction = AIACTION::AIACTION_BATTLE;
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
					Pos += Dir* 3.f;
					if (abs(D3DXToDegree(Angle)) < 4.f)
					{
						if (m_fReloadTime > m_fReload)
						{
							Engine::Reuse_Object(Pos, Dir, 500.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::MASHINE_BULLET);
							m_fReloadTime = 0.f;
						}
					}
				}
			}
			else
			{
				if (fAllyCol <= 60.f)
				{
					m_iAction = AIACTION::AIACTION_BATTLE;
					_vec3 Pos, Dir, vLook, vEHPos, TargetPos;
					m_pTransformHead->Get_Info(INFO::INFO_POS, &vEHPos);
					pAllyTransform->Get_Info(INFO::INFO_POS, &TargetPos);
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
					Pos += Dir* 3.f;
					if (abs(D3DXToDegree(Angle)) < 4.f)
					{
						if (m_fReloadTime > m_fReload)
						{
							Engine::Reuse_Object(Pos, Dir, 500.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::MASHINE_BULLET);
							m_fReloadTime = 0.f;
						}
					}

				}
			}

		}

	}

	_float fPlayerCol = Dist(pPlayerTransform);
	if (fPlayerCol <= 60.f)
	{
		m_iAction = AIACTION::AIACTION_BATTLE;
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
		Pos += Dir* 3.f;
		if (abs(D3DXToDegree(Angle)) < 4.f)
		{
			if (m_fReloadTime > m_fReload)
			{
				Engine::Reuse_Object(Pos, Dir, 500.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::MASHINE_BULLET);
				m_fReloadTime = 0.f;
			}
		}
	}

}
void CBottomDirEnermy::Set_DisCountLocation()
{
	if (m_iLocationCheck == LOCATIONCHECK::LOCATIONCHECK_LEFTTOP)
	{
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"LeftTopLocation");
		CLeftTopLocation* LTTemp = dynamic_cast<CLeftTopLocation*>(Dest);
		LTTemp->Set_EnermyCount();
	}
	if (m_iLocationCheck == LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP)
	{
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"RightTopLocation");
		CRightTopLocation* RTTemp = dynamic_cast<CRightTopLocation*>(Dest);
		RTTemp->Set_EnermyCount();
	}
	if (m_iLocationCheck == LOCATIONCHECK::LOCATIONCHECK_LEFT)
	{
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"LeftLocation");
		CLeftLocation* LTemp = dynamic_cast<CLeftLocation*>(Dest);
		LTemp->Set_EnermyCount();
	}
	if (m_iLocationCheck == LOCATIONCHECK::LOCATIONCHECK_RIGHT)
	{
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"RightLocation");
		CRightLocation* RTemp = dynamic_cast<CRightLocation*>(Dest);
		RTemp->Set_EnermyCount();
	}
}
void CBottomDirEnermy::Basic(_float fTimeDelta)
{

	if (m_iAction != AIACTION::AIACTION_BATTLE)
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
}

_bool CBottomDirEnermy::Left_RightCheck(_vec3 _vDir, _vec3 _vLook)
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

_float CBottomDirEnermy::Dist(CTransform * _Target)
{
	_vec3 vTargetPos, vPos, vCol, _vAPos;
	_Target->Get_Info(INFO::INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	vCol = vPos - vTargetPos;
	_float Dist = D3DXVec3Length(&vCol);
	return Dist;
}

void CBottomDirEnermy::Occupation(_float fTimeDelta)
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
		vPatrolRange.x = m_vPatrol.x + rand() % 45 + 50.f;
		vPatrolRange.y = 0;
		vPatrolRange.z = m_vPatrol.z - rand() % 30 - 10.f;
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
		m_pTransformCom->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta*3.f);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta*3.f);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta*3.f);
		_vec3 vTrans2;
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
		m_pTransformCom->Move_Pos(&(vTrans2*10.f*fTimeDelta));
	}
	else
	{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta*3.f);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta*3.f);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta*3.f);
		_vec3 vTrans2;
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
		m_pTransformCom->Move_Pos(&(vTrans2*10.f*fTimeDelta));
	}

}

void CBottomDirEnermy::Wait(_float fTimeDelta)
{
	if (m_iAction == AIACTION::AIACTION_WAIT)
	{
		_vec3   vTemp, vSour, vDist, vDir, vLook, vTrans2;
		_float Angle = 0.f, Dot = 0.f;
		switch (m_PastLocation)
		{
		case LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP:

			vTemp = { m_vPatrol.x,0.f,0.f };
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
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
			}
			else if (LeftCheck == true)
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
			}

			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
			break;
		case LOCATIONCHECK::LOCATIONCHECK_LEFTTOP:
			vTemp = { m_vPatrol.x,0.f,0.f };
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
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
			}
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
			break;

		case LOCATIONCHECK::LOCATIONCHECK_RIGHT:
		{
			vTemp = { 0.f,0.f,m_vPatrol.z };
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
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);

			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);

			}
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));

			break;
		}
		case LOCATIONCHECK::LOCATIONCHECK_LEFT:
			vTemp = { 0.f,0.f,m_vPatrol.z };
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
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*4.f);

			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*4.f);

			}
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
			break;
		}


	}
}

void CBottomDirEnermy::Enermy_In_Area(_float fTimeDelta)
{

	_bool test = false;
	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ALLY);
	vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDALLY);
	for (auto& iter = Sours.begin(); iter < Sours.end(); ++iter)
	{
		CTransform*	pAllyTransform = static_cast<CTransform*> (static_cast<CBottomDirAlly*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
		_float fAllyCol = Dist(pAllyTransform);

		if (fAllyCol <= 150.f)
		{
			test = true;
			if (dynamic_cast<CBottomDirAlly*>(*iter)->Get_LocationCheck() == m_iLocationCheck)
			{

				CTransform*	pDefaultEnermyTransform = static_cast<CTransform*> (static_cast<CBottomDirAlly*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
				NULL_CHECK(pDefaultEnermyTransform);

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
		m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
	}
}
CBottomDirEnermy* CBottomDirEnermy::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBottomDirEnermy*		pInstance = new CBottomDirEnermy(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("BackGround Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBottomDirEnermy * CBottomDirEnermy::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CBottomDirEnermy*		pInstance = new CBottomDirEnermy(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		MSG_BOX("BackGround Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CBottomDirEnermy::ObjectCol(_bool m_Left)
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

HRESULT CBottomDirEnermy::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	//탱크 몸통
	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Humvee_enemy_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Humvee_enemy_head");
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

	pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Humvee_enemy_posin");
	NULL_CHECK_RETURN(m_pPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelPosin", pComponent });


	//탱크 움직임

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTransformHead = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_HeadTransform", pComponent });

	pComponent = m_pCalculatorCom = static_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

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

void CBottomDirEnermy::Free(void)
{
	__super::Free();

}

void CBottomDirEnermy::Update_UI(void)
{

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
		UI_fScaleX = 2.f;
		UI_fScaleY = 0.2f;


		m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY, UI_fScaleZ);

		m_pTransformCom->Get_Info(INFO_POS, &vTankPos);

		vUI_HPF = { vTankPos.x, vTankPos.y + 3.f, vTankPos.z };

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
