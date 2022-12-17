#include "stdafx.h"
#include "..\Header\UI_Compass.h"
#include "Stage.h"
#include "Export_Function.h"
#include "AimCamera.h"
#include "UI_FontMgr.h"

CUI_Compass::CUI_Compass(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Compass::CUI_Compass(const CUI_Compass& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Compass::~CUI_Compass()
{
}

HRESULT CUI_Compass::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 70.f;
	m_fScaleY = 70.f;
	m_fScaleZ = 1.f;

	m_fPosX = 730.f;
	m_fPosY = 530.f;
	m_fPosZ = 0.01f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	m_szlastTankName = m_szTankName = CUI_FontMgr::GetInstance()->Get_Tank_Name();

	if (m_szTankName == L"Humvee")
	{
		iRotaion = 40;
	}
	else if (m_szTankName == L"A - 44")
	{
		iRotaion = 40;
	}
	else if (m_szTankName == L"TE6E4")
	{
		iRotaion = 30;
	}
	else if (m_szTankName == L"KING TIGER")
	{
		iRotaion = 20;
	}
	else if (m_szTankName == L"M55")
	{
		iRotaion = 10;
	}


	return S_OK;
}


_int CUI_Compass::Update_Object(const _float & fTimeDelta)
{
#pragma region 
//CComponent* pTank = Engine::Get_Component(L"GameLogic", L"PlayerVehicle" , L"Proto_TransformBody" , ID_DYNAMIC);
	//_float fAngle;
	//m_pTransform->Rotation(ROT_Z, fAngle);
	//_vec3 vRot;
	//static_cast<CTransform*>(pTank)->Get_Info(INFO::INFO_LOOK, &vRot);

	//CTransform* pPlayerTrans = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	//NULL_CHECK_RETURN(pPlayerTrans, E_FAIL);

	//_vec3 PlyerPos;
	//pPlayerTrans->Get_Info(INFO_POS, &PlyerPos);
	////HitPos.y = PlyerPos.y = 2.4f;
	////_vec3 vLook = HitPos - PlyerPos;
	////m_vLook = *D3DXVec3Normalize(&vLook, &vLook);

	//CComponent* pTank = Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC);
	//_float fAngle = static_cast<CTransform*>(pTank)->Get_Angle(ROT_Y);
	
	//CTransform* pTankLook = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));

	//_vec3 vTankLook, vTankPos, vAimPos , vAngle;

	//vAimPos = (Engine::Get_Camera(L"AimCamera")->Get_Look());

	//pTankLook->Get_Info(INFO::INFO_LOOK, &vTankLook);
	//pTankLook->Get_Info(INFO::INFO_POS, &vTankPos);

	//// ��ũ�� ���� ���� ���Ϳ� ����ī�޶� ��ġ
	//vAngle = vTankLook - vTankPos;
	//_float fAngle = acos(D3DXVec3Dot(&vAngle, &vAimPos));

	//fAngle = (fAngle * 3.1415f) / 180.f;

	// ����ī�޶� �������� ��ũ ������ ��� �˰� �ʹ�. ���� ��ũ�� �޸��� ���߿� ���� ī�޶� Ű��, ������ ��븦 ���� ȸ���Ѵٸ�,
	// ������� �������� ������, ��ũ�� ������ ������� �� �� �ֵ��� �ϴ� ���̴�.

	// ���� ��ũ�� Y�� ȸ���� �ݴ�� �������� Z�� ȸ������ ������ �ȴ�.
	
	// ���ؾ� �ϴ� �� = ��ũ�� ���� ���� ����          	m_pTransformBody->Get_Info(INFO_LOOK, &vDir);
	// ���ؾ� �ϴ� �� = ��ũ�� �߽ɿ��� ����ī�޶�� ���ϴ� ���� ����
/*	_vec3 HitPos = static_cast<CTankSet*>(pPlayer)->Get_HitPos();
	_vec3 PlyerPos;
	pPlayerTrans->Get_Info(INFO_POS, &PlyerPos);
	HitPos.y = PlyerPos.y = 2.4f;
	_vec3 vLook = HitPos - PlyerPos;
	m_vLook = *D3DXVec3Normalize(&vLook, &vLook);*/
	//									static_cast<CTankCamera*>(Engine::Get_Camera(L"TankCamera"))

	//// ��ũ�� ���� ���� ���� ���ϱ�
	//_vec3 vTankLook;
	//CTransform* pTank = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	//pTank->Get_Info(INFO::INFO_LOOK, &vTankLook);
	//D3DXVec3Normalize(&vTankLook, &vTankLook);

	//// ��ũ �߽ɿ��� ����ī�޶�� ���ϴ� ���� ����
	//_vec3 vTankPos , vAimCameraPos , vAimLook;
	//pTank->Get_Info(INFO::INFO_POS, &vTankPos);
	//CAimCamera* pAimView = static_cast<CAimCamera*>(Engine::Get_Camera(L"AimCamera"));
	//vAimLook = pAimView->Get_Look();

	//vTankLook.y = vAimLook.y = 0.f; // ���� �ǰ� �ʿ� ���� y�� ���� - ��� ���ص� �ȴ�.(���� ������ ���̴�.)

	//_float vAngle_F = ((vTankLook.x * vAimLook.x) + (vTankLook.z * vAimLook.z));
	//vAngle_F = (vAngle_F * 3.1415f) / 180.f;
#pragma endregion

	m_szTankName = CUI_FontMgr::GetInstance()->Get_Tank_Name();

	if (m_szlastTankName != m_szTankName)
	{
		m_pTransform->Rotation(ROT_Z, 0.f - m_pTransform->Get_Angle(ROT_Z));
		m_szlastTankName = m_szTankName;

	}


	if (m_szTankName == L"Humvee")
	{
		iRotaion = 40;
	}
	else if (m_szTankName == L"A - 44")
	{
		iRotaion = 40;
	}
	else if (m_szTankName == L"TE6E4")
	{
		iRotaion = 30;
	}
	else if (m_szTankName == L"KING TIGER")
	{
		iRotaion = 20;
	}
	else if (m_szTankName == L"M55")
	{
		iRotaion = 10;
	}



	if (Get_DIKeyState_Custom(DIK_A) == KEY_STATE::HOLD)
	{
		m_pTransform->Rotation(ROT_Z, D3DXToRadian((iRotaion)*fTimeDelta));
		//m_pTransform->Rotation(ROT_Z, vAngle_F);

	}
	if (Get_DIKeyState_Custom(DIK_D) == KEY_STATE::HOLD)
	{
		m_pTransform->Rotation(ROT_Z, D3DXToRadian( -(iRotaion) * fTimeDelta));
	}
	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_Compass::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	
	Add_RenderGroup(RENDER_UI, this);
	
}

void CUI_Compass::Render_Object(void)
{
	if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
	{

		//Render_Font()

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();


	}

}

CUI_Compass * CUI_Compass::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Compass*		pInstance = new CUI_Compass(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("UI_Compass Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_Compass::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Compass_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Compass_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Compass", pComponent });


	return S_OK;
}

void CUI_Compass::Free(void)
{
	CGameObject::Free();
}
