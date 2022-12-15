#include "stdafx.h"
#include "..\Header\Default_Enermy.h"

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
#include"BottomDirAlly.h"
#include "TankCamera.h"
#include "StaticCamera.h"
#include "AimCamera.h"
#include "Collisionable.h"
#include "GameMode.h"

CDefault_Enermy::CDefault_Enermy(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	CGameMode::GetInstance()->m_nEnemyCount++;
}

CDefault_Enermy::CDefault_Enermy(const CDefault_Enermy & rhs)
	: CGameObject(rhs)
{
}

CDefault_Enermy::~CDefault_Enermy()
{
	Free();
	CGameMode::GetInstance()->m_nEnemyCount--;
}

HRESULT CDefault_Enermy::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformCom->Set_Pos(550.f, 2.f, 550.f);

	m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformHead->Set_Pos(550.f, 1.f, 550.f);

	m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformPosin->Set_Pos(550.f, 1.f, 550.f);

	//UI_HP
	UI_Orgin_HP = UI_fHP = 300.f;    // tankData.fMaxHP;
	UI_fOrgin_ScaleX = UI_fScaleX = 2.f;
	UI_fScaleY = 0.2f;
	UI_fScaleZ = 1.f;

	// UI_Minimap
	D3DXMatrixOrthoLH(&UI_Minimap_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fMinimap[SCALEX] = m_fMinimap[SCALEY] = 3.f;
	m_fMinimap[SCALEZ] = 1.f;
	m_pMinimap_Transform->Set_Scale(m_fMinimap[SCALEX], m_fMinimap[SCALEY], m_fMinimap[SCALEZ]);

	return S_OK;
}

HRESULT CDefault_Enermy::Ready_Object(void * pArg)
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
	m_stBody.fLen[x] = 1.5f;
	m_stBody.fLen[y] = 0.7f;
	m_stBody.fLen[z] = 2.5f;

	m_stHead.fLen[x] = 1.f;
	m_stHead.fLen[y] = 1.f;
	m_stHead.fLen[z] = 1.9f;
	//UI_HP
	UI_Orgin_HP = UI_fHP = 300.f;    // tankData.fMaxHP;
	PreHp = UI_fHP;
	UI_fOrgin_ScaleX = UI_fScaleX = 2.f;
	UI_fScaleY = 0.2f;
	UI_fScaleZ = 1.f;


	// UI_Minimap
	D3DXMatrixOrthoLH(&UI_Minimap_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fMinimap[SCALEX] = m_fMinimap[SCALEY] = 3.f;
	m_fMinimap[SCALEZ] = 1.f;
	m_pMinimap_Transform->Set_Scale(m_fMinimap[SCALEX], m_fMinimap[SCALEY], m_fMinimap[SCALEZ]);


	__super::Ready_Object();
	return S_OK;
}

_int CDefault_Enermy::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);
	__super::Update_Object(fTimeDelta);

	m_fReloadTime += fTimeDelta;

	if (m_iLocationState == LOCATIONSTATE::STATE_ENERMYHQ)
	{
		UI_fHP += 0.0001f*fTimeDelta;
		if (UI_fHP >= UI_Orgin_HP)
		{
			UI_fHP = UI_Orgin_HP;
		}
	}

	PreHp = UI_fHP;//체력 순서 중요

	StateCheck();
	if (m_iAction != AIACTION::AIACTION_RUN&&m_iAction != AIACTION::AIACTION_OBJECTCOL)
	{
		Detect(fTimeDelta);
	}
	Basic(fTimeDelta);
	_vec3 vTrans;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vTrans);
	m_pTransformHead->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
	m_pTransformPosin->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
	Add_RenderGroup(RENDER_NONALPHA, this);
	Update_OBB();

	return OBJ_NOEVENT;
}

void CDefault_Enermy::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
	if (PreHp != UI_fHP)
	{
		m_iAction = AIACTION::AIACTION_RUN;
	}

	Update_UI();
	Update_Minimap();

}

void CDefault_Enermy::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBody->Render(m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pHead->Render(m_pTransformHead->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());


	// HP UI
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &UI_matViewF);
	m_pTextureF->Set_Texture(0);
	m_pRcTexF->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// Minimap UI
	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		_matrix OldViewMatrix, OldProjMatrix, Minimap_ViewMatrix;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pMinimap_Transform->Get_WorldMatrix());
		D3DXMatrixIdentity(&Minimap_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &Minimap_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &UI_Minimap_matProj);
		m_pMinimap_Texure->Set_Texture(0);
		m_pMinimap_RcTex->Render_Buffer();
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
	}
}

const _vec3 CDefault_Enermy::Get_Info(void)
{
	_vec3 Info;
	m_pTransformCom->Get_Info(INFO_POS, &Info);

	return Info;
}

void CDefault_Enermy::Move_Info(_vec3 _Info)
{
	m_pTransformCom->Move_Pos(&_Info);
	m_pTransformHead->Move_Pos(&_Info);
	m_pTransformPosin->Move_Pos(&_Info);
}

void CDefault_Enermy::OBB_Collision_EX(void)
{
	//건물 충돌
}

void CDefault_Enermy::Update_OBB(void)
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

void CDefault_Enermy::StateCheck()
{
	_vec3	vPos;

	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	//RightLocation
	if (vPos.x > VTXITV*VTXCNTX / 2.f && vPos.z < VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bRightLocationCount == false)
			m_bRightLocationCount = true;
		m_bRightTopLocationCount = false;
		m_bLeftLocationCount = false;
		m_bLeftTopLocationCount = false;

		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"RightLocation");
		CRightLocation* LTemp = dynamic_cast<CRightLocation*>(Dest);
		m_iLocationState = LTemp->Get_LocationState();
		LTemp->Get_AllyCount();
		LTemp->Get_EnemyCount();
		LTemp->Get_AllyOccupation();
		LTemp->Get_EnermyOccupation();
		if (m_iAction != AIACTION::AIACTION_RUN&&m_iAction != AIACTION::AIACTION_OBJECTCOL)
		{
			if (LTemp->Get_EnemyCount() >= 1 && LTemp->Get_AllyCount() >= 1)
				m_iAction = AIACTION::AIACTION_ENERMY_IN_AREA;
			else if (LTemp->Get_EnermyOccupation() >= 100.f)
				m_iAction = AIACTION::AIACTION_WAIT;
			else if (LTemp->Get_EnemyCount() >= 1 && LTemp->Get_AllyCount() <= 0)
				m_iAction = AIACTION::AIACTION_OCCOPATION;
		}
	}
	//LeftLocation
	else if (vPos.x < VTXITV*VTXCNTX / 2.f && vPos.z < VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bLeftLocationCount == false)
		{
			m_bLeftLocationCount = true;

			m_vPatrol.x = (float)(rand() % 150 + 1);
			m_vPatrol.y = 0;
			m_vPatrol.z = (float)(rand() % 100 + 1);
		}
		m_bRightTopLocationCount = false;
		m_bRightLocationCount = false;
		m_bLeftTopLocationCount = false;
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"LeftLocation");
		CLeftLocation* LTemp = dynamic_cast<CLeftLocation*>(Dest);
		m_iLocationState = LTemp->Get_LocationState();
		if (m_iAction != AIACTION::AIACTION_RUN&&m_iAction != AIACTION::AIACTION_OBJECTCOL)
		{
			if (LTemp->Get_EnemyCount() >= 1 && LTemp->Get_AllyCount() >= 1)
				m_iAction = AIACTION::AIACTION_ENERMY_IN_AREA;
			else if (LTemp->Get_EnermyOccupation() >= 100.f)
				m_iAction = AIACTION::AIACTION_WAIT;
			else if (LTemp->Get_EnemyCount() >= 1 && LTemp->Get_AllyCount() <= 0)
				m_iAction = AIACTION::AIACTION_OCCOPATION;
		}
	}
	//RightTopLocation
	else if (vPos.x >VTXITV*VTXCNTX / 2.f && vPos.z >VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bRightTopLocationCount == false)
		{
			m_vPatrol.x = (rand() % 120 + VTXITV*VTXCNTX / 2.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 120 + VTXITV*VTXCNTZ / 2.f);

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
		if (m_iAction != AIACTION::AIACTION_RUN&&m_iAction != AIACTION::AIACTION_OBJECTCOL)
		{
			if (LTemp->Get_EnemyCount() >= 1 && LTemp->Get_AllyCount() >= 1)
				m_iAction = AIACTION::AIACTION_ENERMY_IN_AREA;
			else if (LTemp->Get_EnermyOccupation() >= 100.f)
				m_iAction = AIACTION::AIACTION_WAIT;
			else if (LTemp->Get_EnemyCount() >= 1 && LTemp->Get_AllyCount() <= 0)
				m_iAction = AIACTION::AIACTION_OCCOPATION;
		}
	}
	//LeftTopLocation
	else if (vPos.x < VTXITV*VTXCNTX*0.5f && vPos.z > VTXITV*VTXCNTZ *0.5f)
	{
		if (m_bLeftTopLocationCount == false)
		{
			m_vPatrol.x = (VTXITV*VTXCNTX / 2.f - rand() % 220 - 30.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 170 + VTXITV*VTXCNTZ / 2.f + 70.f);

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
		if (m_iAction != AIACTION::AIACTION_RUN&&m_iAction != AIACTION::AIACTION_OBJECTCOL)
		{
			if (LTemp->Get_EnemyCount() >= 1 && LTemp->Get_AllyCount() >= 1)
				m_iAction = AIACTION::AIACTION_ENERMY_IN_AREA;
			else if (LTemp->Get_EnermyOccupation() >= 100.f)
				m_iAction = AIACTION::AIACTION_WAIT;
			else if (LTemp->Get_EnemyCount() >= 1 && LTemp->Get_AllyCount() <= 0)
				m_iAction = AIACTION::AIACTION_OCCOPATION;
		}
	}
}

OBB * CDefault_Enermy::Get_OBB(void)
{
	return &m_stBody;
}

void CDefault_Enermy::Detect(_float fTimeDelta)
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
					m_iAction = AIACTION::AIACTION_BATTLE;
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
						m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
						m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
					}
					else
					{
						m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
						m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
					}
					m_pTransformHead->Get_Info(INFO_POS, &Pos);
					m_pTransformHead->Get_Info(INFO_LOOK, &vLook);
					Dir = TargetPos - Pos;
					D3DXVec3Normalize(&Dir, &Dir);
					D3DXVec3Normalize(&vLook, &vLook);
					Pos += Dir* 3.f*fPosinDist;

					if (m_fReloadTime > m_fReload + 5.f&&TempBullet == 1)
					{
						if (abs(D3DXToDegree(Angle)) < 4.f)
						{
							Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::MASHINE_BULLET);
							m_fReloadTime = 0.f;
						}
					}
					else if (m_fReloadTime > m_fReload + 5.f&&TempBullet != 1)
					{
						if (abs(D3DXToDegree(Angle)) < 4.f)
						{
							Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
							m_fReloadTime = 0.f;
						}
					}
					else
					{
						m_iAction = AIACTION::AIACTION_OCCOPATION;
					}
				}
			}
			else if (fPreAllyCol > fSourCol)
			{
				if (fSourCol <= Range)
				{
					m_iAction = AIACTION::AIACTION_BATTLE;
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
						m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
						m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
					}
					else
					{
						m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
						m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
					}
					m_pTransformHead->Get_Info(INFO_POS, &Pos);
					m_pTransformHead->Get_Info(INFO_LOOK, &vLook);
					Dir = TargetPos - Pos;
					D3DXVec3Normalize(&Dir, &Dir);
					D3DXVec3Normalize(&vLook, &vLook);
					Pos += Dir* 3.f*fPosinDist;

					if (m_fReloadTime > m_fReload + 5.f&&TempBullet == 1)
					{
						if (abs(D3DXToDegree(Angle)) < 4.f)
						{
							Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::MASHINE_BULLET);
							m_fReloadTime = 0.f;
						}
					}
					else if (m_fReloadTime > m_fReload + 5.f&&TempBullet != 1)
					{
						if (abs(D3DXToDegree(Angle)) < 4.f)
						{
							Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
							m_fReloadTime = 0.f;
						}
					}
					else
					{
						m_iAction = AIACTION::AIACTION_OCCOPATION;
					}
				}
			}
		}
		else if (fPlayerCol <= Range)
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
			Pos += Dir* 3.f*fPosinDist;
			if (m_fReloadTime > m_fReload + 5.f)
			{
				if (abs(D3DXToDegree(Angle)) < 4.f)
				{
					Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::MASHINE_BULLET);
					m_fReloadTime = 0.f;
				}
			}
			else
			{
				m_iAction = AIACTION::AIACTION_OCCOPATION;
			}
		}
	}
	else
	{
		if (fPlayerCol <= Range)
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
			Pos += Dir* 3.f*fPosinDist;
			if (m_fReloadTime > m_fReload + 5.f)
			{
				if (abs(D3DXToDegree(Angle)) < 4.f)
				{
					Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::MASHINE_BULLET);
					m_fReloadTime = 0.f;
				}
			}
			else
			{
				m_iAction = AIACTION::AIACTION_OCCOPATION;
			}
		}
	}
}


void CDefault_Enermy::Set_DisCountLocation()
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




void CDefault_Enermy::Basic(_float fTimeDelta)
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
	case AIACTION::AIACTION_RUN:
		Run(fTimeDelta);
		break;
	case AIACTION::AIACTION_OBJECTCOL:
		ColObject(fTimeDelta);
		break;
	}
}

_bool CDefault_Enermy::Left_RightCheck(_vec3 _vDir, _vec3 _vLook)
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

_float CDefault_Enermy::Dist(CTransform * _Target)
{
	_vec3 vTargetPos, vPos, vCol, _vAPos;
	_Target->Get_Info(INFO::INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	vCol = vPos - vTargetPos;
	_float Dist = D3DXVec3Length(&vCol);
	return Dist;
}

void CDefault_Enermy::ObjectCol(_bool m_Left)
{
	if (!m_bTest)
	{
		m_bTest = true;
	}
	else
	{
		m_iAction = AIACTION::AIACTION_OBJECTCOL;
		bLeft = m_Left;
	}
}

void CDefault_Enermy::Run(_float fTimeDelta)
{

	CTransform*		pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	if (pPlayerTransform == nullptr)
		return;

	_float fPlayerCol = Dist(pPlayerTransform);
	if (fPlayerCol <= 60.f)
	{
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
		if (m_fReloadTime > m_fReload + 3.f)
		{

			Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::MASHINE_BULLET);
			m_fReloadTime = 0.f;

		}
	}

	if (UI_fHP < 290.f)
	{
		_vec3 vTemp, vSour, vLook, vDir;
		_float Dot, Angle;

		vTemp = { (_float)((VTXCNTX*VTXITV) - rand() % 80 - 60),0.f,(_float)((VTXCNTX*VTXITV) - rand() % 100 - 40) };
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vSour);
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		vDir = vTemp - vSour;

		D3DXVec3Normalize(&vDir, &vDir);
		D3DXVec3Normalize(&vLook, &vLook);

		Left_RightCheck(vDir, vLook);

		Dot = D3DXVec3Dot(&vDir, &vLook);

		Angle = (float)acosf(Dot);
		if (isnan(Angle))
			Angle = 0;
		if (LeftCheck == false)
		{
			m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
		}
		else if (LeftCheck == true)
		{
			m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
		}

		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		m_pTransformCom->Move_Pos(&(vLook*fTimeDelta*fAccel_top_speed));

	}
}

void CDefault_Enermy::Occupation(_float fTimeDelta)//이거 케이스별로나구기
{
	_vec3  vPos, vLook, vDir;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);

	if ((m_vPatrol.x - 3.f <= vPos.x && m_vPatrol.z - 3.f <= vPos.z) && (m_vPatrol.x + 3.f >= vPos.x&&m_vPatrol.z + 3.f >= vPos.z))
	{
		vPatrolRange.x = m_vPatrol.x - 40.f - rand() % 25;
		vPatrolRange.y = 0;
		vPatrolRange.z = m_vPatrol.z - rand() % 45 - 20.f;
		m_bPatrol = true;
	}
	if ((vPatrolRange.x - 3.f <= vPos.x && vPatrolRange.z - 3.f <= vPos.z) && (vPatrolRange.x + 3.f >= vPos.x&&vPatrolRange.z + 3.f >= vPos.z))
	{
		m_bPatrol = false;
	}
	if (!m_bPatrol)
	{
		vDir = m_vPatrol - vPos;
	}
	else
	{
		vDir = vPatrolRange - vPos;
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
	}
	else
	{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta*3.f);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta*3.f);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta*3.f);

	}
	_vec3 vTrans2;
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
	m_pTransformCom->Move_Pos(&(vTrans2*fAccel_top_speed*fTimeDelta));



}

void CDefault_Enermy::Wait(_float fTimeDelta)
{
	_vec3   vTemp, vSour, vDist, vDir, vLook, vTrans2;
	_float Angle = 0.f, Dot = 0.f;
	switch (m_PastLocation)
	{
	case LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP:

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
		m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*fAccel_top_speed));
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
			m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
			m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
		}
		else
		{
			m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
			m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
		}
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
		m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*fAccel_top_speed));
		break;
	case LOCATIONCHECK::LOCATIONCHECK_LEFT:
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
			m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
			m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
		}
		else
		{
			m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
			m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
		}
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
		m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*fAccel_top_speed));
		break;
	}
}

void CDefault_Enermy::Enermy_In_Area(_float fTimeDelta)
{
	_bool test = false;
	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ALLY);
	vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDALLY);
	for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
	{
		CTransform*	pAllyTransform = static_cast<CTransform*> (static_cast<CDefault_Ally*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
		_float fAllyCol = Dist(pAllyTransform);

		if (fAllyCol <= 150.f)
		{
			test = true;
			if (dynamic_cast<CDefault_Ally*>(*iter)->Get_LocationCheck() == m_iLocationCheck)
			{

				CTransform*	pDefaultEnermyTransform = static_cast<CTransform*> (static_cast<CDefault_Ally*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
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
		m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*fAccel_top_speed));

	}

}

CDefault_Enermy* CDefault_Enermy::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDefault_Enermy*		pInstance = new CDefault_Enermy(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("BackGround Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDefault_Enermy * CDefault_Enermy::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CDefault_Enermy*		pInstance = new CDefault_Enermy(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		MSG_BOX("CDefault_Enermy Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDefault_Enermy::ColObject(_float fTimeDelta)
{
	re += fTimeDelta;

	if (re <= 1)
	{
		if (bLeft)
		{
			m_pTransformHead->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90.f*fTimeDelta));
			m_pTransformCom->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90.f*fTimeDelta));
			ColBuild = true;
		}
		else
		{
			ColBuild = true;
			m_pTransformHead->Rotation(ROTATION::ROT_Y, D3DXToRadian(90.f*fTimeDelta));
			m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(90.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, D3DXToRadian(90.f*fTimeDelta));
		}

		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		m_pTransformCom->Move_Pos(&(-vLook*fAccel_top_speed*fTimeDelta*1.1));
	}
	else if (re <= 2)
	{
		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		m_pTransformCom->Move_Pos(&(vLook*fAccel_top_speed*fTimeDelta*1.1));
	}
	if (re >= 2)
	{
		re = 0.f;
		m_iAction = AIACTION::AIACTION_END;
	}
}

HRESULT CDefault_Enermy::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	//탱크 몸통
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
		pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Big_enemy_body");
		NULL_CHECK_RETURN(m_pBody, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

		pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Big_enemy_head");
		NULL_CHECK_RETURN(m_pHead, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

		pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Big_enemy_posin");
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

	// UI
	pComponent = m_pRcTexF = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTexF, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureF = static_cast<CTexture*>(Clone_Prototype(L"Proto_World_Hp_Tex"));
	NULL_CHECK_RETURN(m_pTextureF, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_World_Hp_Tex", pComponent });

	pComponent = m_pTransformHP_UI = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHP_UI, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_WHP2", pComponent });

	//MiniMap UI
	pComponent = m_pMinimap_RcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pMinimap_RcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pMinimap_Texure = static_cast<CTexture*>(Clone_Prototype(L"Proto_Minimap_E_Tex"));
	NULL_CHECK_RETURN(m_pMinimap_Texure, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Minimap_E_Tex", pComponent });

	pComponent = m_pMinimap_Transform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pMinimap_Transform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Minimap_E", pComponent });

	return S_OK;
}

void CDefault_Enermy::Free(void)
{
	Safe_Release(m_pMinimap_RcTex);
	Safe_Release(m_pMinimap_Texure);
	Safe_Release(m_pMinimap_Transform);
	__super::Free();
}
void CDefault_Enermy::Update_UI(void)
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
		UI_fScaleY = 0.6f;

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

void CDefault_Enermy::Update_Minimap(void)
{
	_vec3 vTankPos;

	m_pTransformCom->Get_Info(INFO::INFO_POS, &vTankPos);

	_float fX_Percent = (roundf(vTankPos.x) / 635.f);
	_float fZ_Percent = (roundf(vTankPos.z) / 635.f);

	if (fX_Percent <= 0.f)
	{
		fX_Percent = 0.f;
	}
	else if (fX_Percent >= 1.f)
	{
		fX_Percent = 1.f;
	}

	if (fZ_Percent <= 0.f)
	{
		fZ_Percent = 0.f;
	}
	else if (fZ_Percent >= 1.f)
	{
		fZ_Percent = 1.f;
	}

	m_fMinimap[POSX] = 640.f + roundf(160.f * fX_Percent);
	m_fMinimap[POSY] = 600.f - roundf(115.f * fZ_Percent);
	m_fMinimap[POSZ] = 0.03f;

	// Minimap _ Pos
	m_pMinimap_Transform->Set_Pos(m_fMinimap[POSX] - (WINCX * 0.5f), (WINCY * 0.5f) - m_fMinimap[POSY], m_fMinimap[POSZ]);


}

