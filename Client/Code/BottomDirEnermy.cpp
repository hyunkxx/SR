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
}

HRESULT CBottomDirEnermy::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(3.f, 3.f, 3.f);
	m_pTransformCom->Set_Pos(210.f, 0.f, 210.f);

	m_pTransformHead->Set_Scale(3.f, 3.f, 3.f);
	m_pTransformHead->Set_Pos(210.f, 0.f, 210.f);

	return S_OK;
}

_int CBottomDirEnermy::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);
	m_fReloadTime += fTimeDelta;
	StateCheck();
	Detect(fTimeDelta);
	Basic(fTimeDelta);
	if (m_iAction == AIACTION::AIACTION_WAIT)
	{

	}
	_vec3 vTrans;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vTrans);
	m_pTransformHead->Set_Pos(vTrans.x, vTrans.y + 3.f, vTrans.z);

	return OBJ_NOEVENT;
}

void CBottomDirEnermy::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_PRIORITY, this);
}

void CBottomDirEnermy::Render_Object(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/*m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/


	m_pBodyBuffer->Render_Buffer();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());

	m_pHeadBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CBottomDirEnermy::StateCheck()
{
	_vec3	vPos;

	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	//RightLocation
	if (vPos.x > VTXCNTX / 2.f && vPos.z < VTXCNTZ / 2.f)
	{

		if (m_bRightLocationCount == false)
		{
			m_bRightLocationCount = true;
			srand((unsigned int)time(NULL));
			m_vPatrol.x = (VTXCNTX / 2.f + rand() % 25);
			m_vPatrol.y = 0;
			m_vPatrol.z = (float)((VTXCNTZ / 2.f) - rand() % 50 - 10.f);
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
	else if (vPos.x < VTXCNTX / 2.f && vPos.z < VTXCNTZ / 2.f)
	{
		if (m_bLeftLocationCount == false)
		{
			m_bLeftLocationCount = true;
			srand((unsigned int)time(NULL));
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
	else if (vPos.x >VTXCNTX / 2.f && vPos.z >VTXCNTZ / 2.f)
	{
		if (m_bRightTopLocationCount == false)
		{
			srand((unsigned int)time(NULL));
			m_vPatrol.x = (rand() % 32 + VTXCNTX / 2.f + 50.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 32 + VTXCNTZ / 2.f);

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
	else if (vPos.x < VTXCNTX / 2.f && vPos.z > VTXCNTZ / 2.f)
	{
		if (m_bLeftTopLocationCount == false)
		{
			srand((unsigned int)time(NULL));
			//	m_vPatrol.x = (VTXCNTX / 2.f - rand() % 13-10.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 25 + VTXCNTZ / 2.f);
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
	NULL_CHECK(pPlayerTransform);

	CTransform*		pAllyTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"Default_Ally", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pAllyTransform);

	_float fPlayerCol = Dist(pPlayerTransform);
	_float fAllyCol = Dist(pAllyTransform);
	if (fPlayerCol<fAllyCol)
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
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
			else
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);

			m_pTransformHead->Get_Info(INFO_POS, &Pos);
			m_pTransformHead->Get_Info(INFO_LOOK, &vLook);
			Dir = TargetPos - Pos;
			D3DXVec3Normalize(&Dir, &Dir);
			D3DXVec3Normalize(&vLook, &vLook);
			Pos += Dir* 3.f;
		
				if (m_fReloadTime > m_fReload)
				{
					Engine::Reuse_Object(Pos, Dir, 500.f, m_pTransformHead->Get_Angle(ROT_X), m_pTransformHead->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
					m_fReloadTime = 0.f;
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
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
			else
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);

			m_pTransformHead->Get_Info(INFO_POS, &Pos);
			m_pTransformHead->Get_Info(INFO_LOOK, &vLook);
			Dir = TargetPos - Pos;
			D3DXVec3Normalize(&Dir, &Dir);
			D3DXVec3Normalize(&vLook, &vLook);
			Pos += Dir* 3.f;
		
				if (m_fReloadTime > m_fReload)
				{
					Engine::Reuse_Object(Pos, Dir, 500.f, m_pTransformHead->Get_Angle(ROT_X), m_pTransformHead->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
					m_fReloadTime = 0.f;
				}
			

		}
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
	if (m_iAction == AIACTION::AIACTION_OCCOPATION)
	{

		_vec3  vPos, vLook;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		if (m_vPatrol.x + 3 <= vPos.x&&m_vPatrol.z + 3 <= vPos.z)
		{
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
			_vec3		vDir = m_vPatrol - vPos;

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
				_vec3 vTrans2;
				m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
				m_pTransformCom->Move_Pos(&(vTrans2*5.f*fTimeDelta));
			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta*3.f);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta*3.f);
				_vec3 vTrans2;
				m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
				m_pTransformCom->Move_Pos(&(vTrans2*5.f*fTimeDelta));
			}
		}
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
			}
			else if (LeftCheck == true)
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
			}

			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*5.f));
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
			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
			}
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*5.f));
			break;

		case LOCATIONCHECK::LOCATIONCHECK_RIGHT:
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
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta*2.f);


			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta*2.f);

			}
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*5.f));
			break;
		}

	}
}

void CBottomDirEnermy::Enermy_In_Area(_float fTimeDelta)
{
	if (m_iAction == AIACTION::AIACTION_ENERMY_IN_AREA)
	{
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"GameLogic");
		CGameObject* Dest = Sour->Get_GameObject(L"Default_Ally");
		CDefault_Ally* pAlly = dynamic_cast<CDefault_Ally*>(Dest);


		if (m_iLocationCheck == pAlly->Get_LocationCheck())
		{
			_vec3 vTargetPos, vEnermyPos, vDir, vLook;
			CTransform*		Target = dynamic_cast<CTransform*>(pAlly->Get_Component(L"Proto_Transform", ID_DYNAMIC));
			Target->Get_Info(INFO::INFO_POS, &vTargetPos);
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vEnermyPos);
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);

			vDir = vTargetPos - vEnermyPos;

			D3DXVec3Normalize(&vDir, &vDir);
			D3DXVec3Normalize(&vLook, &vLook);

			Left_RightCheck(vDir, vLook);

			_float Dot = D3DXVec3Dot(&vDir, &vLook);
			_float Angle = (_float)acosf(Dot);
			if (isnan(Angle))
			{
				Angle = 0;
			}
			if (LeftCheck == false)
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
			else
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);

			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			m_pTransformCom->Move_Pos(&(vLook*fTimeDelta*4.f));
		}
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

HRESULT CBottomDirEnermy::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	//攀农 个烹

	pComponent = m_pHeadBuffer = static_cast<CTank_Head*>(Clone_Prototype(L"Proto_TankHead"));
	NULL_CHECK_RETURN(m_pHeadBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ETankHead", pComponent });

	pComponent = m_pBodyBuffer = static_cast<CTank_Body*>(Clone_Prototype(L"Proto_TankBody"));
	NULL_CHECK_RETURN(m_pBodyBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ETankBody", pComponent });

	//攀农 框流烙

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTransformHead = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_HeadTransform", pComponent });

	return S_OK;
}

void CBottomDirEnermy::Free(void)
{
	__super::Free();
}
