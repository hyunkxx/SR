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
#include "GameMode.h"

CBottomDirAlly::CBottomDirAlly(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	CGameMode::GetInstance()->m_nAllyCount++;
}

CBottomDirAlly::CBottomDirAlly(const CBottomDirAlly & rhs)
	: CGameObject(rhs)
{
}

CBottomDirAlly::~CBottomDirAlly()
{
	CGameMode::GetInstance()->m_nAllyCount--;
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

	UI_fOrgin_ScaleX = UI_fScaleX = 2.f;
	UI_fScaleY = 0.2f;
	UI_fScaleZ = 1.f;

	// UI_Minimap
	D3DXMatrixOrthoLH(&UI_Minimap_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fMinimap[SCALEX] = m_fMinimap[SCALEY] = 4.f;
	m_fMinimap[SCALEZ] = 1.f;
	m_pMinimap_Transform->Set_Scale(m_fMinimap[SCALEX], m_fMinimap[SCALEY], m_fMinimap[SCALEZ]);

	return S_OK;
}

HRESULT CBottomDirAlly::Ready_Object(void * pArg)
{
	m_EData = (EData*)pArg;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	TTYPE = m_EData->TankType;
	switch (m_EData->TankType)
	{
	case TANKTYPE::HUMVEE:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::HUMVEE);
		Temp = BULLET_ID::MASHINE_BULLET;
		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fAccel_top_speed = data.fAccel_top_speed*0.5f;
		RotSpped = data.RotSpeed;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		Range = 50.f;
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformCom->Set_Pos(m_EData->vPos.x, 2.f*1.f, m_EData->vPos.z);
		m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformHead->Set_Pos(m_EData->vPos.x, 2.f*1.f, m_EData->vPos.z);
		m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformPosin->Set_Pos(m_EData->vPos.x, 2.f*1.f, m_EData->vPos.z);
		m_stBody.fLen[x] = 2.5f;
		m_stBody.fLen[y] = 4.f;
		m_stBody.fLen[z] = 4.5f;
		break;
	}
	case TANKTYPE::SMALL_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::SMALL_TANK);
		Temp = BULLET_ID::SMALL_CANNONBALL;
		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fAccel_top_speed = data.fAccel_top_speed*0.8f;
		RotSpped = data.RotSpeed;
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
		m_stBody.fLen[x] = 2.5f;
		m_stBody.fLen[y] = 4.f;
		m_stBody.fLen[z] = 4.5f;
		break;
	}
	case TANKTYPE::MIDDLE_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::MIDDLE_TANK);
		Temp = BULLET_ID::MIDDLE_CANNONBALL;
		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fAccel_top_speed = data.fAccel_top_speed*0.8f;
		RotSpped = data.RotSpeed;
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
		m_stBody.fLen[x] = 3.f;
		m_stBody.fLen[y] = 4.f;
		m_stBody.fLen[z] = 5.f;
		break;
	}
	case TANKTYPE::BIG_TANK:
	{
		const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::BIG_TANK);
		Temp = BULLET_ID::BIG_CANNONBALL;
		m_fMaxHp = data.fMaxHP;
		fCurHp = data.fCurHP;
		fAccel_top_speed = data.fAccel_top_speed*0.8f;
		RotSpped = data.RotSpeed;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		Range = 85.f;
		m_pTransformCom->Set_Scale(1.2f, 1.2f, 1.2f);
		m_pTransformCom->Set_Pos(m_EData->vPos.x, 2.f*1.2f, m_EData->vPos.z);
		m_pTransformHead->Set_Scale(1.2f, 1.2f, 1.2f);
		m_pTransformHead->Set_Pos(m_EData->vPos.x, 2.f*1.2f, m_EData->vPos.z);
		m_pTransformPosin->Set_Scale(1.2f, 1.2f, 1.2f);
		m_pTransformPosin->Set_Pos(m_EData->vPos.x, 2.f*1.2f, m_EData->vPos.z);
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
		fAccel_top_speed = data.fAccel_top_speed*0.8f;
		RotSpped = data.RotSpeed;
		fPosinDist = data.fPosinDist;
		m_fReloadTime = data.fReloadTime;
		m_fReload = data.fReload;
		m_iCannonSpeed = data.iCannonSpeed;
		m_pTransformCom->Set_Scale(1.2f, 1.2f, 1.2f);
		m_pTransformCom->Set_Pos(m_EData->vPos.x, 2.f*1.2f, m_EData->vPos.z);
		m_pTransformHead->Set_Scale(1.2f, 1.2f, 1.2f);
		m_pTransformHead->Set_Pos(m_EData->vPos.x, 2.f*1.2f, m_EData->vPos.z);
		m_pTransformPosin->Set_Scale(1.2f, 1.2f, 1.2f);
		m_pTransformPosin->Set_Pos(m_EData->vPos.x, 2.f*1.2f, m_EData->vPos.z);
		break;
	}
	default:
		break;

	}

	//UI_HP

	UI_fOrgin_ScaleX = UI_fScaleX = 3.f;
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

_int CBottomDirAlly::Update_Object(const _float& fTimeDelta)
{
	if (CGameMode::GetInstance()->m_bGameEnd)
		return 0;

	__super::Update_Object(fTimeDelta);
	Update_Shot_Sound();
	_vec3 vRulePos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vRulePos);
	if (vRulePos.x > VTXCNTX*VTXCNTZ*VTXITV
		|| 0 > vRulePos.x
		|| 0 > vRulePos.z
		|| vRulePos.z > VTXCNTX*VTXCNTZ*VTXITV
		)
	{
		RuleTime += fTimeDelta;
	}
	if (RuleTime >= 8.f)
	{
		DeadMotionCheck = true;
	}
	if (!m_bDead&&DeadMotionCheck)
	{
		Dead_Motion(fTimeDelta);
	}
	else
	{
		m_fReloadTime += fTimeDelta;

		StateCheck();
		/*if (m_iLocationState == LOCATIONSTATE::STATE_ALLY || m_iLocationState == LOCATIONSTATE::STATE_ALLYHQ)
		{
		vector<CGameObject*>Defualt = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
		vector<CGameObject*>Bottom = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
		if (Bottom.size() != 0)
		{
		CTransform* EnemyTransform = nullptr;
		_vec3 vEnemyPos, vPos, vLook, vDir;
		for (auto& iter = Bottom.begin(); iter < Bottom.end(); ++iter)
		{
		CBottomDirEnermy* Temp = dynamic_cast<CBottomDirEnermy*>(*iter);
		NULL_CHECK_RETURN(Temp, OBJ_NOEVENT);
		if (Temp->Get_LocationCheck() == m_iLocationCheck)
		{
		EnemyTransform = static_cast<CTransform*>(Temp->Get_Component(L"Proto_Transform", ID_DYNAMIC));
		}
		}
		if (EnemyTransform != nullptr)
		{
		EnemyTransform->Get_Info(INFO::INFO_POS, &vEnemyPos);
		m_pTransformHead->Get_Info(INFO::INFO_POS, &vPos);
		m_pTransformHead->Get_Info(INFO::INFO_LOOK, &vLook);
		vDir = vEnemyPos - vPos;
		_float Dist = D3DXVec3Length(&vDir);

		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Normalize(&vDir, &vDir);

		Left_RightCheck(vDir, vLook);

		_float Dot = D3DXVec3Dot(&vLook, &vDir);
		_float Angle = (float)acosf(Dot);
		if (isnan(Angle))
		{
		Angle = 0;
		}

		if (LeftCheck == false)
		{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 2.f);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
		}
		else
		{
		m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 2.f);
		m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
		m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
		}
		if (Dist > Range)
		{
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		m_pTransformCom->Move_Pos(&(vLook*fTimeDelta*fAccel_top_speed));
		}
		}
		}
		}*/
		if (m_iAction != AIACTION::AIACTION_RUN&&m_iAction != AIACTION::AIACTION_OBJECTCOL)
		{
			Detect(fTimeDelta);
		}
		Basic(fTimeDelta);


		_vec3 vBasePos = { 596.f + rand() % 15, 0.f, 596.f + rand() % 15 };
		_vec3  vPos, vCol;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		vCol = vPos - vBasePos;
		_float Dist = D3DXVec3Length(&vCol);
		if (Dist < Range)
		{
			m_iAction = AIACTION::AIACTION_BATTLE;
			_vec3 Pos, Dir, vLook, vEHPos, TargetPos;
			m_pTransformHead->Get_Info(INFO::INFO_POS, &vEHPos);
			Dir = vBasePos - vEHPos;

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
			Dir = vBasePos - Pos;
			D3DXVec3Normalize(&Dir, &Dir);
			D3DXVec3Normalize(&vLook, &vLook);
			Pos += Dir* 3.f*fPosinDist;

			if (m_fReloadTime > m_fReload)
			{
				if (abs(D3DXToDegree(Angle)) < 4.f)
				{
					Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), Temp);
					if (m_bSoundOn)
					{
						if (m_EData->TankType == TANKTYPE::HUMVEE)
						{
							PlaySound_SR(L"MACHINEGUN_FIRE.wav", (SoundType)(m_EData->SoundType), m_fSoundSize);
						}
						else
						{
							PlaySound_SR(L"Shoot_Fire.wav", (SoundType)(m_EData->SoundType), m_fSoundSize);
						}
					}
					m_fReloadTime = 0.f;
				}
			}
		}

		_vec3 vTrans;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vTrans);
		m_pTransformHead->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
		m_pTransformPosin->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
	}
	if (m_bDeadTime >= 3.f)
	{
		DeadMotionCheck = false;
		m_bDead = true;
	}
	Add_RenderGroup(RENDER_NONALPHA, this);
	Update_OBB();

	return OBJ_NOEVENT;
}

void CBottomDirAlly::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	if (fCurHp / m_fMaxHp<0.2f)
	{
		m_iAction = AIACTION::AIACTION_RUN;
	}
	if (m_bDead && !Deadtest)
	{
		Deadtest = true;
		m_bDead = false;
		DeadMotionCheck = true;
		m_stBody.fLen[x] = 0.f;
		m_stBody.fLen[y] = 0.f;
		m_stBody.fLen[z] = 0.f;

		m_stHead.fLen[x] = 0.f;
		m_stHead.fLen[y] = 0.f;
		m_stHead.fLen[z] = 0.f;
	}
	Update_UI();
	Update_Minimap();
}

void CBottomDirAlly::Render_Object(void)
{
	if (DeadMotionCheck)
	{
		m_pHead->Change_Color_Dead();
		m_pBody->Change_Color_Dead();
		m_pPosin->Change_Color_Dead();
	}
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
	if ((Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA) || (Engine::Get_Camera_ID() == CAMERA_ID::AH_64A_CAMERA))
	{
		if (CGameMode::GetInstance()->m_bGameEnd)
			return;

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
	if (DeadMotionCheck)
	{
		m_pHead->Return_Color();
		m_pBody->Return_Color();
		m_pPosin->Return_Color();
	}
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
			m_vPatrol.x = (_float)(VTXITV* VTXCNTX*0.5 + rand() % 200 + 30.f);
			m_vPatrol.y = 0.f;
			m_vPatrol.z = (_float)(VTXITV* VTXCNTX*0.5 - rand() % 90 - 110.f);
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
		if (m_iAction != AIACTION::AIACTION_RUN&&m_iAction != AIACTION::AIACTION_OBJECTCOL)
		{
			if (LTemp->Get_AllyCount() >= 1 && LTemp->Get_EnemyCount() >= 1)
				m_iAction = AIACTION::AIACTION_ENERMY_IN_AREA;
			else if (LTemp->Get_AllyOccupation() >= 100.f)
				m_iAction = AIACTION::AIACTION_WAIT;
			else if (LTemp->Get_AllyCount() >= 1 && LTemp->Get_EnemyCount() <= 0)
				m_iAction = AIACTION::AIACTION_OCCOPATION;
		}
	}

	//LeftLocation
	else if (vPos.x <= (VTXITV*VTXCNTX) / 2.f && vPos.z <= (VTXITV*VTXCNTZ) / 2.f)
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
		if (m_iAction != AIACTION::AIACTION_RUN&&m_iAction != AIACTION::AIACTION_OBJECTCOL)
		{
			if (LTemp->Get_AllyCount() >= 1 && LTemp->Get_EnemyCount() >= 1)
				m_iAction = AIACTION::AIACTION_ENERMY_IN_AREA;
			else if (LTemp->Get_AllyOccupation() >= 100.f)
				m_iAction = AIACTION::AIACTION_WAIT;
			else if (LTemp->Get_AllyCount() >= 1 && LTemp->Get_EnemyCount() <= 0)
				m_iAction = AIACTION::AIACTION_OCCOPATION;
		}
	}
	//RightTopLocation
	else if (vPos.x >= VTXITV*VTXCNTX / 2.f && vPos.z >= VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bRightTopLocationCount == false)
		{
			m_vPatrol.x = VTXITV* VTXCNTX - rand() % 25 - 35.f;
			m_vPatrol.y = 0;
			m_vPatrol.z = VTXITV* VTXCNTX - rand() % 25 - 35.f;

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
			if (LTemp->Get_AllyCount() >= 1 && LTemp->Get_EnemyCount() >= 1)
				m_iAction = AIACTION::AIACTION_ENERMY_IN_AREA;
			else if (LTemp->Get_AllyOccupation() >= 100.f)
				m_iAction = AIACTION::AIACTION_WAIT;
			else if (LTemp->Get_AllyCount() >= 1 && LTemp->Get_EnemyCount() <= 0)
				m_iAction = AIACTION::AIACTION_OCCOPATION;
		}
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
		if (m_iAction != AIACTION::AIACTION_RUN&&m_iAction != AIACTION::AIACTION_OBJECTCOL)
		{
			if (LTemp->Get_AllyCount() >= 1 && LTemp->Get_EnemyCount() >= 1)
				m_iAction = AIACTION::AIACTION_ENERMY_IN_AREA;
			else if (LTemp->Get_AllyOccupation() >= 100.f)
				m_iAction = AIACTION::AIACTION_WAIT;
			else if (LTemp->Get_AllyCount() >= 1 && LTemp->Get_EnemyCount() <= 0)
				m_iAction = AIACTION::AIACTION_OCCOPATION;
		}
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

void CBottomDirAlly::Set_DisCountLocation()
{
	if (m_iLocationCheck == LOCATIONCHECK::LOCATIONCHECK_LEFTTOP)
	{
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"LeftTopLocation");
		CLeftTopLocation* LTTemp = dynamic_cast<CLeftTopLocation*>(Dest);
		LTTemp->Set_AllyCount();
	}
	if (m_iLocationCheck == LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP)
	{
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"RightTopLocation");
		CRightTopLocation* RTTemp = dynamic_cast<CRightTopLocation*>(Dest);
		RTTemp->Set_AllyCount();
	}
	if (m_iLocationCheck == LOCATIONCHECK::LOCATIONCHECK_LEFT)
	{
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"LeftLocation");
		CLeftLocation* LTemp = dynamic_cast<CLeftLocation*>(Dest);
		LTemp->Set_AllyCount();
	}
	if (m_iLocationCheck == LOCATIONCHECK::LOCATIONCHECK_RIGHT)
	{
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"RightLocation");
		CRightLocation* RTemp = dynamic_cast<CRightLocation*>(Dest);
		RTemp->Set_AllyCount();
	}
}

void CBottomDirAlly::Detect(_float fTimeDelta)
{

	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
	vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
	_float fPreAllyCol = 3000.f;
	_float fSourCol = 3000.f;
	CTransform* pCompareTransform = nullptr;
	CTransform* pSourCompareTransform = nullptr;

	if (Temps.size() != 0)
	{
		_bool bReady = false;
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			CTransform*	pAllyTransform = static_cast<CTransform*> (static_cast<CDefault_Enermy*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
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
			CTransform*	pAllyTransform = static_cast<CTransform*> (static_cast<CBottomDirEnermy*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
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

				if (m_fReloadTime > m_fReload )
				{
					if (abs(D3DXToDegree(Angle)) < 4.f)
					{
						Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), Temp);
						if (m_bSoundOn)
						{
							if (m_EData->TankType == TANKTYPE::HUMVEE)
							{
								PlaySound_SR(L"MACHINEGUN_FIRE.wav", (SoundType)(m_EData->SoundType), m_fSoundSize);
							}
							else
							{
								PlaySound_SR(L"Shoot_Fire.wav", (SoundType)(m_EData->SoundType), m_fSoundSize);
							}
						}
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

				if (m_fReloadTime > m_fReload )
				{
					if (abs(D3DXToDegree(Angle)) < 4.f)
					{
						Engine::Reuse_Object(Pos, Dir, (_float)m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), Temp);
						if (m_bSoundOn)
						{
							if (m_EData->TankType == TANKTYPE::HUMVEE)
							{
								PlaySound_SR(L"MACHINEGUN_FIRE.wav", (SoundType)(m_EData->SoundType), m_fSoundSize);
							}
							else
							{
								PlaySound_SR(L"Shoot_Fire.wav", (SoundType)(m_EData->SoundType), m_fSoundSize);
							}
						}
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

}

void CBottomDirAlly::ObjectCol(_bool m_Left)
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
	case AIACTION::AIACTION_RUN:
		Run(fTimeDelta);
		break;
	case  AIACTION::AIACTION_OBJECTCOL:
		ColObject(fTimeDelta);
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

void CBottomDirAlly::Run(_float fTimeDelta)
{
	_vec3 vTemp, vSour, vLook, vDir;
	_float Dot, Angle;

	vTemp = { (rand() % 80 + 30.f),0.f,(rand() % 80 + 30.f) };
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vSour);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	vDir = vTemp - vSour;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);

	Left_RightCheck(vDir, vLook);

	Dot = D3DXVec3Dot(&vDir, &vLook);

	Angle = (float)acosf(Dot);
	if (vSour.x > vTemp.x + 3.f&&vSour.z > vTemp.z + 3)
	{
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
	else
	{
		{
			fCurHp += 40.f*fTimeDelta;
			if (fCurHp >= m_fMaxHp)
			{
				fCurHp = m_fMaxHp;
			}
		}
	}
	if (fCurHp / m_fMaxHp >= 0.7f)
	{
		m_iAction = AIACTION::AIACTION_END;
	}
}

void CBottomDirAlly::ColObject(_float fTimeDelta)
{
	re += fTimeDelta;

	if (re <= 1.f)
	{
		if (bLeft)
		{
			m_pTransformHead->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90.f*fTimeDelta));
			m_pTransformCom->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, -D3DXToRadian(90.f*fTimeDelta));
		}
		else
		{

			m_pTransformHead->Rotation(ROTATION::ROT_Y, D3DXToRadian(90.f*fTimeDelta));
			m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(90.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, D3DXToRadian(90.f*fTimeDelta));
		}
	}
	else if (re <= 2.f)
	{
		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		m_pTransformCom->Move_Pos(&(vLook*fAccel_top_speed*fTimeDelta));
	}
	if (re >= 2.f)
	{
		re = 0.f;
		m_iAction = AIACTION::AIACTION_END;
	}
}
void CBottomDirAlly::Dead_Motion(const _float & fTimeDelta)
{
	_vec3 Pos;
	m_pTransformHead->Get_Info(INFO_POS, &Pos);
	m_bDeadTime += fTimeDelta;
	fAccum += fTimeDelta;
	if (0.4f < Pos.y && 0 == m_iMotionCount)
	{

		_float x = 5.f * fTimeDelta;
		_float y = (100.f * fTimeDelta) - (0.5f * 9.8f * fAccum*fAccum);
		_vec3 Move = { x , y ,0.f };
		m_pTransformHead->Move_Pos(&Move);
		m_pTransformPosin->Move_Pos(&Move);
		m_pTransformHead->Rotation(ROT_X, 500 * fTimeDelta);
		m_pTransformPosin->Rotation(ROT_X, 500 * fTimeDelta);
	}

	else
	{
		m_iMotionCount++;
		Pos.y = 0.8f;
		m_pTransformHead->Set_Pos(Pos.x, Pos.y, Pos.z);
		m_pTransformPosin->Set_Pos(Pos.x, Pos.y, Pos.z);
	}
}
void CBottomDirAlly::Occupation(_float fTimeDelta)
{

	_vec3  vPos, vLook, vDir;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	if (m_iLocationCheck != LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP)
	{
		if ((m_vPatrol.x - 3.f <= vPos.x && m_vPatrol.z - 3.f <= vPos.z) && (m_vPatrol.x + 3.f >= vPos.x&&m_vPatrol.z + 3.f >= vPos.z))
		{
			vPatrolRange.x = m_vPatrol.x + 40.f + rand() % 40;
			vPatrolRange.y = 0;
			vPatrolRange.z = m_vPatrol.z + rand() % 40 + 50.f;
			m_bPatrol = true;
		}
		if ((vPatrolRange.x - 3.f <= vPos.x && vPatrolRange.z - 3.f <= vPos.z) && (vPatrolRange.x + 3.f >= vPos.x&&vPatrolRange.z + 3.f >= vPos.z))
		{
			m_bPatrol = false;
		}
	}
	else if (m_iLocationCheck == LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP)
	{
		m_bPatrol = false;
		/*if ((m_vPatrol.x - 3.f <= vPos.x && m_vPatrol.z - 3.f <= vPos.z) && (m_vPatrol.x + 3.f >= vPos.x&&m_vPatrol.z + 3.f >= vPos.z))
		{
		vPatrolRange.x = m_vPatrol.x - 40.f - rand() % 40;
		vPatrolRange.y = 0;
		vPatrolRange.z = m_vPatrol.z - rand() % 40 - 50.f;
		m_bPatrol = true;
		}
		if ((vPatrolRange.x - 3.f <= vPos.x && vPatrolRange.z - 3.f <= vPos.z) && (vPatrolRange.x + 3.f >= vPos.x&&vPatrolRange.z + 3.f >= vPos.z))
		{
		m_bPatrol = false;
		}*/
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
	break;
	}
	default:
		break;
	}
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
	m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*fAccel_top_speed));
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

	//≈ ≈© ∏ˆ≈Î
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
	//≈ ≈© øÚ¡˜¿”

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

	pComponent = m_pMinimap_Texure = static_cast<CTexture*>(Clone_Prototype(L"Proto_Minimap_A_Tex"));
	NULL_CHECK_RETURN(m_pMinimap_Texure, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Minimap_A_Tex", pComponent });

	pComponent = m_pMinimap_Transform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pMinimap_Transform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Minimap_A", pComponent });

	return S_OK;
}

void CBottomDirAlly::Free(void)
{
	CGameObject::Free();
}

void CBottomDirAlly::Update_Shot_Sound(void)
{
	CTransform* pPlayerpos = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	_vec3 vPlayerPos, vThisTank;
	pPlayerpos->Get_Info(INFO::INFO_POS, &vPlayerPos);

	m_pTransformCom->Get_Info(INFO::INFO_POS, &vThisTank);


	_float fPlayer_This_Dist = sqrtf(((vThisTank.x - vPlayerPos.x) * (vThisTank.x - vPlayerPos.x)) + ((vThisTank.z - vPlayerPos.z) * (vThisTank.z - vPlayerPos.z)));

	if (fPlayer_This_Dist <= 100.f)
	{
		m_bSoundOn = true;
	}
	else if (fPlayer_This_Dist > 100.f)
	{
		m_bSoundOn = false;
	}

	m_fSoundSize = 1.f - (fPlayer_This_Dist / 100.f);
}

void CBottomDirAlly::Update_UI(void)
{
	//CGameObject* pTankView = Engine::Get_Object(L"Environment", L"TankCamera");
	//CGameObject* pStaticView = Engine::Get_Object(L"Environment", L"StaticCamera");
	//CGameObject* pAimView = Engine::Get_Object(L"Environment", L"AimCamera");

	if (fCurHp >= m_fMaxHp)
	{
		fCurHp = m_fMaxHp;
	}
	if (fCurHp <= 0.f)
	{
		fCurHp = 0.f;
	}

	_float HP_Percent = (fCurHp / m_fMaxHp);

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
	// UI_ ≥Ù¿Ã _ ≈∞øˆµÂ
	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{

		UI_fScaleY = 0.2f;

		m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY, UI_fScaleZ);

		m_pTransformCom->Get_Info(INFO_POS, &vTankPos);

		vUI_HPF = { vTankPos.x, vTankPos.y + 6.f, vTankPos.z };

		Engine::Get_Camera()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &UI_matViewF);
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::DRONE_CAMERA)
	{

		UI_fScaleY = 0.6f;

		m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY, UI_fScaleZ);

		m_pTransformCom->Get_Info(INFO_POS, &vTankPos);

		vUI_HPF = { vTankPos.x, vTankPos.y + 6.f, vTankPos.z + 3.f };

		Engine::Get_Camera()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &UI_matViewF);
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
	{

		UI_fScaleY = 0.2f;

		m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY, UI_fScaleZ);

		m_pTransformCom->Get_Info(INFO_POS, &vTankPos);

		vUI_HPF = { vTankPos.x, vTankPos.y + 6.f, vTankPos.z };

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

void CBottomDirAlly::Update_Minimap(void)
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

