#include "stdafx.h"
#include "..\Header\Default_Ally.h"
#include "Export_Function.h"
#include"Management.h"
#include"Layer.h"
#include"TestPlayer.h"
//Location
#include"RightLocation.h"
#include"LeftTopLocation.h"
#include"LeftLocation.h"
#include"RightTopLocation.h"
CDefault_Ally::CDefault_Ally(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CDefault_Ally::CDefault_Ally(const CDefault_Ally & rhs)
	: CGameObject(rhs)
{
}

CDefault_Ally::~CDefault_Ally()
{
}

HRESULT CDefault_Ally::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);

	m_pTransformCom->Set_Pos(10.f, 0.f, 10.f);
	m_pTransformHead->Set_Pos(10.f, 0.f, 10.f);
	m_pTransformPosin->Set_Pos(10.f, 0.f, 10.f);

	return S_OK;
}

_int CDefault_Ally::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);
	StateCheck();
	m_fReloadTime += fTimeDelta;
	Detect(fTimeDelta);
	Basic(fTimeDelta);

	_vec3 vTrans;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vTrans);
	m_pTransformHead->Set_Pos(vTrans.x, vTrans.y + 1.f, vTrans.z);
	m_pTransformPosin->Set_Pos(vTrans.x, vTrans.y + 1.f, vTrans.z);

	return OBJ_NOEVENT;
}

void CDefault_Ally::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_PRIORITY, this);
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

void CDefault_Ally::StateCheck()
{
	_vec3	vPos;

	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	//RightLocation
	if (vPos.x > VTXCNTX / 2.f && vPos.z < VTXCNTZ / 2.f)
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


	}
	//LeftLocation
	else if (vPos.x < VTXCNTX / 2.f && vPos.z < VTXCNTZ / 2.f)
	{
		if (m_bLeftLocationCount == false)
		{
			m_bLeftLocationCount = true;
			srand((unsigned int)time(NULL));
			m_vPatrol.x = (float)(VTXCNTX / 2.f - rand() % 24 - 30.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (float)(VTXCNTX / 2.f - rand() % 17);

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
			m_vPatrol.x = (rand() % 32 + VTXCNTX / 2.f);
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
			m_vPatrol.x = (VTXCNTX / 2.f - rand() % 13 + 5.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 20 + VTXCNTZ / 2.f + 20.f);

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
	CTransform*		pEnermyCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"Default_Enermy", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pEnermyCom);

	D3DXVECTOR3 vPPos, vEPos, vCol;
	pEnermyCom->Get_Info(INFO::INFO_POS, &vPPos);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vEPos);
	vCol = vEPos - vPPos;
	float Dist = D3DXVec3Length(&vCol);

	//���� �� �� ��
	if (Dist <= 60.f)
	{
		m_iAction = AIACTION::AIACTION_BATTLE;
		_vec3 Pos, Dir, vLook, vEHPos;
		m_pTransformPosin->Get_Info(INFO::INFO_POS, &vEHPos);
		Dir = vPPos - vEHPos;
		D3DXVec3Normalize(&Dir, &Dir);
		m_pTransformPosin->Get_Info(INFO::INFO_LOOK, &vLook);
		_vec3 Up = { 0.f,1.f,0.f }, Temp;
		D3DXVec3Normalize(&Up, &Up);
		D3DXVec3Normalize(&vLook, &vLook);

		_float Dot = D3DXVec3Dot(&vLook, &Dir);
		D3DXVec3Cross(&Temp, &Dir, &vLook);
		_float TempTest = D3DXVec3Dot(&Up, &Temp);
		_float Angle = (float)acosf(Dot);
		if (isnan(Angle))
		{
			Angle = 0;
		}
		if (TempTest > 0)
		{
			m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
		}
		else
		{
			m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
			m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
		}

		m_pTransformPosin->Get_Info(INFO_POS, &Pos);
		m_pTransformPosin->Get_Info(INFO_LOOK, &vLook);
		Dir = vPPos - vEPos;
		D3DXVec3Normalize(&Dir, &Dir);
		D3DXVec3Normalize(&vLook, &vLook);
		Pos += Dir* 3.f;
		if (abs(D3DXToDegree(Angle)) < 1.f)
		{
			if (m_fReload <= m_fReloadTime)
			{
				Engine::Reuse_Object(Pos, Dir, 500.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
				m_fReloadTime = 0.f;
			}
		}
	}

}

void CDefault_Ally::Basic(_float fTimeDelta)
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
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta);
				_vec3 vTrans2;
				m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
				m_pTransformCom->Move_Pos(&(vTrans2*2.f*fTimeDelta));
			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta);
				_vec3 vTrans2;
				m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
				m_pTransformCom->Move_Pos(&(vTrans2*10.f*fTimeDelta));
			}
		}
	}
}

void CDefault_Ally::Wait(_float fTimeDelta)
{
	if (m_iAction == AIACTION::AIACTION_WAIT)
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
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
			break;
		case LOCATIONCHECK::LOCATIONCHECK_LEFTTOP:
			vTemp = { VTXCNTX,0.f,m_vPatrol.z };
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
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
			break;

		case LOCATIONCHECK::LOCATIONCHECK_LEFT:
			vTemp = { m_vPatrol.x,0.f,VTXCNTZ };
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
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));

			break;
		}

	}
}

void CDefault_Ally::Enermy_In_Area(_float fTimeDelta)
{

	if (m_iAction == AIACTION::AIACTION_ENERMY_IN_AREA)
	{

		CTransform*		pEnermy = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"Default_Enermy", L"Proto_Transform", ID_DYNAMIC));
		NULL_CHECK(pEnermy);
		_vec3 vPlayerPos, vEnermyPos, vDir, vLook;

		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPlayerPos);
		pEnermy->Get_Info(INFO::INFO_POS, &vEnermyPos);
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);

		vDir = vEnermyPos - vPlayerPos;

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
			m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
		else
			m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);

		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		m_pTransformCom->Move_Pos(&(vLook*fTimeDelta*10.f));
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

HRESULT CDefault_Ally::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	//��ũ ����
	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Tank_01_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Tank_01_head");
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

	pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Tank_01_posin");
	NULL_CHECK_RETURN(m_pPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });

	//��ũ ������

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