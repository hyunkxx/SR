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
CDefault_Enermy::CDefault_Enermy(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CDefault_Enermy::CDefault_Enermy(const CDefault_Enermy & rhs)
	: CGameObject(rhs)
{

}

CDefault_Enermy::~CDefault_Enermy()
{
	Free();
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
	return S_OK;
}

HRESULT CDefault_Enermy::Ready_Object(void * pArg)
{

	m_EData = (EData*)pArg;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformCom->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);

	m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformHead->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);

	m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformPosin->Set_Pos(m_EData->vPos.x, 2.f, m_EData->vPos.z);
	return S_OK;
}

_int CDefault_Enermy::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);
	m_fReloadTime += fTimeDelta;
	StateCheck();
	Detect(fTimeDelta);
	Basic(fTimeDelta);
	_vec3 vTrans;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vTrans);
	m_pTransformHead->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
	m_pTransformPosin->Set_Pos(vTrans.x, vTrans.y, vTrans.z);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CDefault_Enermy::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();


}

void CDefault_Enermy::Render_Object(void)
{


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBody->Render(m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pHead->Render(m_pTransformHead->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());

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
	}
	//RightTopLocation
	else if (vPos.x >VTXITV*VTXCNTX / 2.f && vPos.z >VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bRightTopLocationCount == false)
		{
			m_vPatrol.x = (rand() % 32 + VTXITV*VTXCNTX / 2.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 32 + VTXITV*VTXCNTZ / 2.f);

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

			m_vPatrol.x = (VTXITV*VTXCNTX / 2.f - rand() % 250);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 200 + VTXITV*VTXCNTZ / 2.f);

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

void CDefault_Enermy::Detect(_float fTimeDelta)
{
	CTransform*		pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ALLY);
	if (Temps.size() != 0)
	{
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			CTransform*	pAllyTransform = static_cast<CTransform*> (static_cast<CDefault_Ally*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
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
					Pos += Dir* 3.f;
					if (abs(D3DXToDegree(Angle)) < 5.f)
					{
						if (m_fReloadTime > m_fReload)
						{
							Engine::Reuse_Object(Pos, Dir, 500.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
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
				Engine::Reuse_Object(Pos, Dir, 500.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
				m_fReloadTime = 0.f;
			}
		}
	}
}

void CDefault_Enermy::Basic(_float fTimeDelta)
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

void CDefault_Enermy::Occupation(_float fTimeDelta)//이거 케이스별로나구기
{
	if (m_iAction == AIACTION::AIACTION_OCCOPATION)
	{

		_vec3  vPos, vLook;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		if (m_vPatrol.x + 3 <= vPos.x)
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
			m_pTransformCom->Move_Pos(&(vTrans2*10.f*fTimeDelta));
		}
	}
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
		m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
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
		m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
		break;
	}
}

void CDefault_Enermy::Enermy_In_Area(_float fTimeDelta)
{
	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ALLY);
	vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDALLY);
	for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
	{
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
	_vec3 vTrans2;
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
	m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
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

HRESULT CDefault_Enermy::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

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

	pComponent = m_pTransformPosin = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PosinTransform", pComponent });

	return S_OK;
}

void CDefault_Enermy::Free(void)
{
	__super::Free();
}