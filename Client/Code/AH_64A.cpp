#include "stdafx.h"
#include "..\Header\AH_64A.h"

#include "Export_Function.h"
#include "AH_64A_Camera.h"
#include "AH_64A_AimCamera.h"
#include "TankSet.h"
#include "AH_64A_Shoot_Effect.h"
#include "AH_64A_EndCamera.h"

CAH_64A::CAH_64A(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CAH_64A::CAH_64A(const CAH_64A & rhs)
	: CGameObject(rhs)
{
}

CAH_64A::~CAH_64A()
{
}

_int CAH_64A::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return 0;

	m_fReloadTime += fTimeDelta;
	m_fPlayCount += fTimeDelta;

	play_Voice();


	if(3.f <m_fDeadCount)
		Engine::StopSound(AH_64A_SOUND);

	if (8.f < m_fDeadCount)
	{
		m_bDead = true;
		for (_int i = 0; ROT_END > (ROTATION)i; i++)
		{
			m_fRot[(ROTATION)i] = 0.f;
		}
		m_fDeadCount = 0.f;
		m_vInfo[INFO_POS] = { 100.f, 50.f , -300.f };
		m_vInfo[INFO_RIGHT] = { 1.f, 0.f , 0.f };
		m_vInfo[INFO_UP] = { 0.f, 1.f , 0.f };
		m_vInfo[INFO_LOOK] = { 0.f, 0.f , 1.f };

	}

	if (!m_In_Sound && !m_bDeadCounting)
	{
		m_fSoundCount += fTimeDelta;

		if (0.5f < m_fSoundCount)
		{
			// 사운드 다시 재생

			m_fSoundCount = 0.f;
			Engine::PlaySound_SR(L"AH_64A_SOUND2.mp3", AH_64A_SOUND, 1.f);
		}
	}
	else
	{
		m_fInSoundCount += fTimeDelta;
		m_fRot[ROT_X] = 30.f;
		if (10.f < m_fInSoundCount)
		{
			// 사운드 다시 재생
			m_fInSoundCount = 0.f;
			Engine::PlaySound_SR(L"AH_64A_IN_SOUND2.mp3", AH_64A_SOUND, 1.f);
		}
	}

	if (m_fPlayCount > 60.f && !m_bDeadCounting)
	{
		Engine::Camera_Change(L"AH_64A_EndCamera");
		Engine::StopSound(AH_64A_VOICE2);
		Engine::PlaySound_SR(L"AH_64A_End.mp3", AH_64A_VOICE2, 0.5f);
		static_cast<CAH_64A_EndCamera*>(Engine::Get_Camera())->Camera_Setting(m_vInfo[INFO_POS]);
		m_bDeadCounting = true;
		m_bLock = true;
		Engine::StopSound(AH_64A_SOUND);
	}
	else if (Get_DIKeyState_Custom(DIK_ESCAPE) == KEY_STATE::TAP)
	{
		Engine::Camera_Change(L"AH_64A_EndCamera");
		Engine::StopSound(AH_64A_VOICE2);
		Engine::PlaySound_SR(L"AH_64A_End.mp3", AH_64A_VOICE2, 0.5f);
		static_cast<CAH_64A_EndCamera*>(Engine::Get_Camera())->Camera_Setting(m_vInfo[INFO_POS]);
		m_bDeadCounting = true;
		m_bLock = true;
		Engine::StopSound(AH_64A_SOUND);
	}

	m_fAccum += 1500.f * fTimeDelta;
	if (720.f <= m_fAccum)
		m_fAccum = 0.f;

	_int iExit = CGameObject::Update_Object(fTimeDelta);

	if (m_bAppear)
	{
		_vec3 vLook = m_vInfo[INFO_LOOK];
		vLook.y = 0.f;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vInfo[INFO_POS] += vLook * 50.f * fTimeDelta;
		if (-10.f < m_vInfo[INFO_POS].z)
		{
			m_bAppear = false;
			m_fAdAccum = 50.f / 30.f;
		}
	}
	else
	{
		Key_Input(fTimeDelta);
	}

	if (m_bDeadCounting)
	{
		m_fDeadCount += fTimeDelta;
		_vec3 Dir = m_vInfo[INFO_LOOK];
		Dir.y = 0.f;
		D3DXVec3Normalize(&Dir, &Dir);
		m_pTransformBody->Move_Pos(&(Dir * 100.f *fTimeDelta));
	}


	Update_Minimap();
	Set_Transform(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);
	return iExit;
}

void CAH_64A::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	CGameObject::LateUpdate_Object();
}

void CAH_64A::Render_Object(void)
{
	if (m_bDead)
		return;

	if (Engine::Get_Camera()->Get_ID() == CAMERA_ID::AH_64A_AIMCAMERA)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBody->Get_WorldMatrix());
	m_pBody->Render(m_pTransformBody->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pHead->Render(m_pTransformHead->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformTail->Get_WorldMatrix());
	m_pTail->Render(m_pTransformTail->Get_WorldMatrix());

	if (Engine::Get_Camera_ID() == CAMERA_ID::AH_64A_CAMERA)
	{
		// Minimap UI
		_matrix OldViewMatrix, OldProjMatrix;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pMinimap_Transform->Get_WorldMatrix());
		_matrix	Minimap_ViewMatrix;
		D3DXMatrixIdentity(&Minimap_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &Minimap_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &UI_Minimap_matProj);
		m_pMinimap_Texure->Set_Texture(0);
		m_pMinimap_RcTex->Render_Buffer();
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
	}
}

void CAH_64A::RenderGUI(void)
{
}

void CAH_64A::Key_Input(const _float & fTimeDleta)
{
	if (m_bLock)
		return;

	if (Engine::Get_DIKeyState_Custom(DIK_W) == KEY_STATE::HOLD)
	{
		m_fRot[ROT_X] += (30.f * fTimeDleta);
		if (30.f < m_fRot[ROT_X])
			m_fRot[ROT_X] = 30.f;

		if (0.f < m_fRot[ROT_X] && Engine::Get_DIKeyState_Custom(DIK_LCONTROL) == KEY_STATE::NONE)
			Plus_Advance_AccelSpeed(fTimeDleta);
	}
	else if (Engine::Get_DIKeyState_Custom(DIK_S) == KEY_STATE::HOLD)
	{
		m_fRot[ROT_X] -= (30.f * fTimeDleta);
		if (-30.f > m_fRot[ROT_X])
			m_fRot[ROT_X] = -30.f;

		if (0.f > m_fRot[ROT_X] && Engine::Get_DIKeyState_Custom(DIK_LCONTROL) == KEY_STATE::NONE)
			Minus_Advance_AccelSpeed(fTimeDleta);
	}
	else
	{

		if (0.f < m_fRot[ROT_X])
		{
			m_fRot[ROT_X] -= (30.f * fTimeDleta);
			if (0.f > m_fRot[ROT_X])
				m_fRot[ROT_X] = 0.f;
		}
		else
		{
			m_fRot[ROT_X] += (30.f * fTimeDleta);
			if (0.f < m_fRot[ROT_X])
				m_fRot[ROT_X] = 0.f;
		}

		if (m_fAdSpeed > 0.f)
			Minus_Advance_AccelSpeed(fTimeDleta);
		else
			Plus_Advance_AccelSpeed(fTimeDleta);
	}

	if (Engine::Get_DIKeyState_Custom(DIK_A) == KEY_STATE::HOLD)
	{
		if (Engine::Get_DIKeyState_Custom(DIK_W) == KEY_STATE::HOLD
			|| Engine::Get_DIKeyState_Custom(DIK_S) == KEY_STATE::HOLD
			|| Engine::Get_DIKeyState_Custom(DIK_LCONTROL) == KEY_STATE::HOLD)
		{
			m_fRot[ROT_Y] -= 60.f * fTimeDleta;

			m_fRot[ROT_Z] += (20.f * fTimeDleta);
			if (20.f < m_fRot[ROT_Z])
				m_fRot[ROT_Z] = 20.f;

			m_fRightAccum = 0.f;
		}
		else if (Engine::Get_DIKeyState_Custom(DIK_W) == KEY_STATE::NONE &&Engine::Get_DIKeyState_Custom(DIK_S) == KEY_STATE::NONE)
		{
			m_fRot[ROT_Z] += (20.f * fTimeDleta);
			if (20.f < m_fRot[ROT_Z])
				m_fRot[ROT_Z] = 20.f;

			if (0.f < m_fRot[ROT_Z] && Engine::Get_DIKeyState_Custom(DIK_LCONTROL) == KEY_STATE::NONE)
				Minus_Right_AccelSpeed(fTimeDleta);
		}
	}
	else if (Engine::Get_DIKeyState_Custom(DIK_D) == KEY_STATE::HOLD)
	{
		if (Engine::Get_DIKeyState_Custom(DIK_W) == KEY_STATE::HOLD
			|| Engine::Get_DIKeyState_Custom(DIK_S) == KEY_STATE::HOLD
			|| Engine::Get_DIKeyState_Custom(DIK_LCONTROL) == KEY_STATE::HOLD)
		{
			m_fRot[ROT_Y] += 60.f * fTimeDleta;

			m_fRot[ROT_Z] -= (20.f * fTimeDleta);
			if (-20.f > m_fRot[ROT_Z])
				m_fRot[ROT_Z] = -20.f;

			m_fRightAccum = 0.f;
		}
		else if (Engine::Get_DIKeyState_Custom(DIK_W) == KEY_STATE::NONE &&Engine::Get_DIKeyState_Custom(DIK_S) == KEY_STATE::NONE)
		{
			m_fRot[ROT_Z] -= (20.f * fTimeDleta);
			if (-20.f > m_fRot[ROT_Z])
				m_fRot[ROT_Z] = -20.f;

			if (0.f > m_fRot[ROT_Z] &&
				Engine::Get_DIKeyState_Custom(DIK_LCONTROL) == KEY_STATE::NONE)
				Plus_Right_AccelSpeed(fTimeDleta);
		}
	}
	else
	{
		if (0.f < m_fRot[ROT_Z])
		{
			m_fRot[ROT_Z] -= (20.f * fTimeDleta);
			if (0.f > m_fRot[ROT_Z])
				m_fRot[ROT_Z] = 0.f;
		}
		else
		{
			m_fRot[ROT_Z] += (20.f * fTimeDleta);
			if (0.f < m_fRot[ROT_Z])
				m_fRot[ROT_Z] = 0.f;
		}

		if (m_fRightSpeed > 0.f)
			Minus_Right_AccelSpeed(fTimeDleta);
		else
			Plus_Right_AccelSpeed(fTimeDleta);
	}
	if (Engine::Get_DIKeyState_Custom(DIK_C) == KEY_STATE::HOLD)
	{

		for (int i = 0; 3 > i; i++)
		{
			if (m_fAdSpeed > 0.f)
				Minus_Advance_AccelSpeed(fTimeDleta);
			else
				Plus_Advance_AccelSpeed(fTimeDleta);
		}

	}

	if (Engine::Get_DIMouseState_Custom(DIM_LB) == KEY_STATE::HOLD && 0.08f < m_fReloadTime)
	{
		if (m_bRight_Shoot)
		{
			_vec3 Pos, Dir, Right, Up;
			Pos = m_vInfo[INFO_POS];
			Dir = m_vInfo[INFO_LOOK];
			Right = m_vInfo[INFO_RIGHT];
			Up = m_vInfo[INFO_UP];
			Pos.y += 2.f * m_fScale;
			D3DXVec3Normalize(&Dir, &Dir);
			Pos += Dir * 5 * m_fScale;

			Engine::Reuse_Object((Pos + (4 * Right) - (3 * Up)), Dir, 800.f, m_fRot[ROT_X], D3DXToRadian(m_fRot[ROT_Y]), BULLET_ID::AH_64A_BULLET);
			m_fReloadTime = 0.f;

			Engine::StopSound(PLAYER_SHOT_SOUND1);

			if (m_In_Sound)
				Engine::PlaySound_SR(L"SE_TN81_PURGE_00.wav", PLAYER_SHOT_SOUND1, 0.8f / 3.f);
			else
				Engine::PlaySound_SR(L"SE_TN81_PURGE_00.wav", PLAYER_SHOT_SOUND1, 0.8f);

			CGameObject* pObject = Engine::Reuse_Effect(EFFECT_ID::AH_64A_EFFECT);
			pObject->Set_Dead(false);
			static_cast<CAH_64A_Shoot_Effect*>(pObject)->Set_Target(m_pTransformBody);
			static_cast<CAH_64A_Shoot_Effect*>(pObject)->Set_Left_Effect(true);
			m_bRight_Shoot = false;
		}
		else
		{
			_vec3 Pos, Dir, Right, Up;
			Pos = m_vInfo[INFO_POS];
			Dir = m_vInfo[INFO_LOOK];
			Up = m_vInfo[INFO_UP];
			Right = m_vInfo[INFO_RIGHT];
			Pos.y += 2.f * m_fScale;
			D3DXVec3Normalize(&Dir, &Dir);
			Pos += Dir * 5 * m_fScale;
			Engine::Reuse_Object((Pos - (4 * Right) - (3 * Up)), Dir, 800.f, m_fRot[ROT_X], D3DXToRadian(m_fRot[ROT_Y]), BULLET_ID::AH_64A_BULLET);
			m_fReloadTime = 0.f;

			Engine::StopSound(PLAYER_SHOT_SOUND2);

			if (m_In_Sound)
				Engine::PlaySound_SR(L"SE_TN81_PURGE_00.wav", PLAYER_SHOT_SOUND2, CUI_Volume::s_fShotSound / 3.f);
			else
				Engine::PlaySound_SR(L"SE_TN81_PURGE_00.wav", PLAYER_SHOT_SOUND2, CUI_Volume::s_fShotSound);

			CGameObject* pObject = Engine::Reuse_Effect(EFFECT_ID::AH_64A_EFFECT);
			pObject->Set_Dead(false);
			static_cast<CAH_64A_Shoot_Effect*>(pObject)->Set_Target(m_pTransformBody);
			static_cast<CAH_64A_Shoot_Effect*>(pObject)->Set_Left_Effect(false);
			m_bRight_Shoot = true;
		}

	}

	if (Engine::Get_DIMouseState_Custom(DIM_RB) == KEY_STATE::TAP)
	{
		if (dynamic_cast<CAH_64A_AimCamera*>(Engine::Get_Camera()))
		{
			Engine::Camera_Change(L"AH_64A_Camera");
			dynamic_cast<CAH_64A_Camera*>(Engine::Get_Camera())->Set_Pos(m_vInfo[INFO_POS]);
			m_In_Sound = false;
			Engine::StopSound(AH_64A_SOUND);
			m_fInSoundCount = 0.f;
		}
		else if (dynamic_cast<CAH_64A_Camera*>(Engine::Get_Camera()))
		{
			Engine::Camera_Change(L"AH_64A_AimCamera");
			m_In_Sound = true;
			Engine::StopSound(AH_64A_SOUND);
			Engine::PlaySound_SR(L"AH_64A_IN_SOUND2.mp3", AH_64A_SOUND, 1.f);
			m_fInSoundCount = 0.f;
		}
	}
	_vec3 vLook = m_vInfo[INFO_LOOK];
	_vec3 vRight = m_vInfo[INFO_RIGHT];
	vLook.y = 0.f;
	vRight.y = 0.f;

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	m_vInfo[INFO_POS] += vLook * m_fAdSpeed * fTimeDleta;
	m_vInfo[INFO_POS] += vRight * m_fRightSpeed * fTimeDleta;

}

void CAH_64A::Start_AH_64A(void)
{
	m_fScale = 1.f;
	m_fDeadCount = 0.f;
	m_bDead = false;
	m_bLock = false;
	m_bAppear = true;
	m_bDeadCounting = false;
	m_vInfo[INFO_POS] = { 100.f, 50.f , -400.f };
	m_vInfo[INFO_RIGHT] = { 10.f, 0.f , 0.f };
	m_vInfo[INFO_UP] = { 0.f, 1.f , 0.f };
	m_vInfo[INFO_LOOK] = { 0.f, 0.f , 1.f };

	m_fAdAccum = 0.f;
	m_fRightAccum = 0.f;
	m_fPlayCount = 0.f;

	m_fSoundCount = 30.f;
	m_fInSoundCount = 0.f;
	m_In_Sound = false;

	m_fSound = 0.f;
	m_fRot[ROT_X] = 30.f;
	Engine::StopSound(AH_64A_VOICE);
	Engine::StopSound(AH_64A_VOICE2);
	Engine::StopSound(AH_64A_VOICE3);



	Engine::PlaySound_SR(L"AH_64A_START_2.mp3", AH_64A_VOICE2, 1.f);
	Engine::Camera_Change(L"AH_64A_Camera");
	if (dynamic_cast<CAH_64A_Camera*>(Engine::Get_Camera(L"AH_64A_Camera")))
	{
		m_pTransformBody->Reset_Trans();
		dynamic_cast<CAH_64A_Camera*>(Engine::Get_Camera(L"AH_64A_Camera"))->Set_Target(m_pTransformBody);
		dynamic_cast<CAH_64A_Camera*>(Engine::Get_Camera(L"AH_64A_Camera"))->Camera_Setting(_vec3{ 400.f, 100.f , 0.f });
		Engine::Get_Camera()->Shake_On();
	}
}

void CAH_64A::Set_Transform(const _float& fTimeDelta)
{
	_vec3 Right, Up, Look;
	Right = { 1.f,0.f,0.f };
	Up = { 0.f,1.f,0.f };
	Look = { 0.f,0.f,1.f };


	_matrix matWorld, matScale, matRot[ROT_END], matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale)
		;
	D3DXVec3Cross(&m_vInfo[INFO_RIGHT], &Up, &m_vInfo[INFO_LOOK]);
	D3DXVec3Normalize(&m_vInfo[INFO_RIGHT], &m_vInfo[INFO_RIGHT]);

	D3DXVec3Cross(&m_vInfo[INFO_UP], &m_vInfo[INFO_LOOK], &m_vInfo[INFO_RIGHT]);
	D3DXVec3Normalize(&m_vInfo[INFO_UP], &m_vInfo[INFO_UP]);


	//	_float RadianX =  acosf(D3DXVec3Dot(&m_vInfo[INFO_UP], &Up));
	_vec3 NewLook;
	NewLook = m_vInfo[INFO_LOOK];
	NewLook.y = 0.f;

	_float RadianX = acosf(D3DXVec3Dot(&m_vInfo[INFO_LOOK], &NewLook));
	_float RadianY = acosf(D3DXVec3Dot(&m_vInfo[INFO_LOOK], &Look));

	if (m_vInfo[INFO_LOOK].y <  Look.y)
		RadianX *= -1;

	if (m_vInfo[INFO_LOOK].x < Look.x)
		RadianY *= -1;

	D3DXMatrixRotationY(&matRot[ROT_Y], D3DXToRadian(m_fRot[ROT_Y]));
	D3DXMatrixRotationX(&matRot[ROT_X], D3DXToRadian(m_fRot[ROT_X]));
	D3DXMatrixRotationZ(&matRot[ROT_Z], D3DXToRadian(m_fRot[ROT_Z]));
	//D3DXMatrixTranslation(&matTrans,0, 0, 0);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	matWorld = matScale  * matRot[ROT_Z] * matRot[ROT_X] * matRot[ROT_Y] * matTrans;
	m_pTransformBody->m_matWorld = matWorld;

	////////////////////////////////////////////////////////////////////////////////////////

	_matrix matWorld2, matScale2, matRot2[ROT_END], matTrans2;

	D3DXMatrixIdentity(&matWorld2);
	D3DXMatrixScaling(&matScale2, m_fScale * 1.5f, m_fScale* 1.5f, m_fScale* 1.5f);

	D3DXMatrixRotationY(&matRot2[ROT_Y], D3DXToRadian(m_fAccum));
	D3DXMatrixTranslation(&matTrans2, 0, -(3 * m_fScale), -(2 * m_fScale));

	matWorld2 = matScale2 * matRot2[ROT_Y] * matTrans2  * matWorld;

	m_pTransformHead->m_matWorld = matWorld2;

	////////////////////////////////////////////////////////////////////////////////////////


	_matrix matWorld3, matScale3, matRot3[ROT_END], matTrans3;

	D3DXMatrixIdentity(&matWorld3);
	D3DXMatrixScaling(&matScale3, m_fScale * 0.8f, m_fScale* 0.8f, m_fScale* 0.8f);

	D3DXMatrixRotationX(&matRot3[ROT_X], D3DXToRadian(m_fAccum));
	D3DXMatrixTranslation(&matTrans3, -1.f * m_fScale, 1.f * m_fScale, -17.f* m_fScale);

	matWorld3 = matScale3 * matRot3[ROT_X] * matTrans3  * matWorld;

	m_pTransformTail->m_matWorld = matWorld3;

	memcpy(m_vInfo[INFO_LOOK], &(*m_pTransformBody->Get_WorldMatrix()).m[INFO_LOOK][0], sizeof(_vec3));
	memcpy(m_vInfo[INFO_UP], &(*m_pTransformBody->Get_WorldMatrix()).m[INFO_UP][0], sizeof(_vec3));
	memcpy(m_vInfo[INFO_RIGHT], &(*m_pTransformBody->Get_WorldMatrix()).m[INFO_RIGHT][0], sizeof(_vec3));

}

HRESULT CAH_64A::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransformBody = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Body_Transform", pComponent });

	pComponent = m_pTransformHead = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Head_Transform", pComponent });

	pComponent = m_pTransformTail = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformTail, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Tail_Transform", pComponent });

	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"AH_64A_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"AH_64A_head");
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelHead", pComponent });

	pComponent = m_pTail = CVoxel::Create(m_pGraphicDev, L"AH_64A_object");
	NULL_CHECK_RETURN(m_pTail, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelTail", pComponent });
	
	//MiniMap UI
	pComponent = m_pMinimap_RcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pMinimap_RcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pMinimap_Texure = static_cast<CTexture*>(Clone_Prototype(L"Proto_Minimap_P_AH_Tex"));
	NULL_CHECK_RETURN(m_pMinimap_Texure, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Minimap_P_AH_Tex", pComponent });

	pComponent = m_pMinimap_Transform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pMinimap_Transform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Minimap_P_AH", pComponent });

	return S_OK;
}

HRESULT CAH_64A::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fScale = 1.f;
	m_bDead = false;

	m_vInfo[INFO_LOOK] = { 0.f,0.f,1.f };
	m_vInfo[INFO_UP] = { 0.f,1.f,0.f };
	m_vInfo[INFO_RIGHT] = { 1.f,0.f,0.f };
	m_vInfo[INFO_POS] = { 40.f, 20.f * m_fScale, 0.f };

	m_fAdSpeed = 0.f;
	m_fRightSpeed = 0.f;
	for (int i = 0; i < ROT_END; i++)
	{
		m_fRot[i] = 0.f;
	}


	// UI_Minimap
	D3DXMatrixOrthoLH(&UI_Minimap_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fMinimap[SCALEX] = m_fMinimap[SCALEY] = 4.f;
	m_fMinimap[SCALEZ] = 1.f;
	m_pMinimap_Transform->Set_Scale(m_fMinimap[SCALEX], m_fMinimap[SCALEY], m_fMinimap[SCALEZ]);

	return S_OK;
}

CAH_64A * CAH_64A::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAH_64A* pInstance = new CAH_64A(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CAH_64A::Free(void)
{
	CGameObject::Free();
}

_float CAH_64A::Plus_Advance_AccelSpeed(const _float & fTimeDelta)
{
	_bool Ad;

	if (m_fAdSpeed > 0.f)
		Ad = true;
	else
		Ad = false;

	m_fAdAccum += fTimeDelta;

	m_fAdSpeed = (30.f) * m_fAdAccum;

	if (60.f < m_fAdSpeed)
		m_fAdSpeed = 50.f;
	else if (-50.f * m_fScale > m_fAdSpeed)
		m_fAdSpeed = -50.f;


	if (m_fAdSpeed < 0.f && Ad)
	{
		m_fAdSpeed = 0.f;
		m_fAdAccum = 0.f;
	}


	return m_fAdSpeed;
}

_float CAH_64A::Minus_Advance_AccelSpeed(const _float & fTimeDelta)
{
	_bool back;

	if (m_fAdSpeed < 0.f)
		back = true;
	else
		back = false;

	m_fAdAccum -= fTimeDelta;

	m_fAdSpeed = (30.f) * m_fAdAccum;

	if (-50.f > m_fAdSpeed)
		m_fAdSpeed = -50.f;
	else if (50.f < m_fAdSpeed)
		m_fAdSpeed = 50.f;

	if (m_fAdSpeed > 0.f && back)
	{
		m_fAdAccum = 0.f;
		m_fAdSpeed = 0.f;
	}

	return m_fAdSpeed;
}

_float CAH_64A::Plus_Right_AccelSpeed(const _float & fTimeDelta)
{
	_bool Ad;

	if (m_fRightSpeed > 0.f)
		Ad = true;
	else
		Ad = false;

	m_fRightAccum += fTimeDelta;

	m_fRightSpeed = (30.f) * m_fRightAccum;

	if (40.f < m_fRightSpeed)
		m_fRightSpeed = 40.f * m_fScale;
	else if (-40.f > m_fRightSpeed)
		m_fRightSpeed = -40.f;

	if (m_fRightSpeed <  0.f && Ad)
	{
		m_fRightAccum = 0.f;
		m_fRightSpeed = 0.f;
	}

	return m_fRightSpeed;

}

_float CAH_64A::Minus_Right_AccelSpeed(const _float & fTimeDelta)
{
	_bool back;

	if (m_fRightSpeed < 0.f)
		back = true;
	else
		back = false;

	m_fRightAccum -= fTimeDelta;

	m_fRightSpeed = (30.f) * m_fRightAccum;

	if (-40.f * m_fScale > m_fRightSpeed)
		m_fRightSpeed = -40.f;
	else if (40.f < m_fRightSpeed)
		m_fRightSpeed = 40.f;

	if (m_fRightSpeed > 0.f && back)
	{
		m_fRightSpeed = 0.f;
		m_fRightAccum = 0.f;
	}

	return m_fRightSpeed;
}

void CAH_64A::play_Voice(void)
{
	if (m_fPlayCount > 1.5f && 2.f > m_fPlayCount)
	{
		Engine::PlaySound_SR(L"AH_64A_START.mp3", AH_64A_VOICE, 0.5f);
	}

	if (m_fPlayCount > 15.f && 16.f > m_fPlayCount)
	{
		Engine::PlaySound_SR(L"AH_64A_FUN.mp3", AH_64A_VOICE2, 0.5f);
	}

	if (m_fPlayCount > 25.f && 26.f > m_fPlayCount)
	{
		Engine::PlaySound_SR(L"AH_64A_VOICE2.mp3", AH_64A_VOICE2, 0.5f);
	}

	if (m_fPlayCount > 35.f && 35.5f > m_fPlayCount)
	{
		Engine::PlaySound_SR(L"AH_64A_GO.mp3", AH_64A_VOICE2, 0.5f);
	}

	if (m_fPlayCount > 45.f && 46.f > m_fPlayCount)
	{
		Engine::PlaySound_SR(L"AH_64A_OK.mp3", AH_64A_VOICE2, 0.5f);
	}
}

void CAH_64A::Update_Minimap(void)
{

	//_vec3 vTankPos;

	//m_pTransformBody->Get_Info(INFO::INFO_POS, &vTankPos);

	_float fX_Percent = (roundf(m_vInfo[INFO_POS].x) / 635.f);
	_float fZ_Percent = (roundf(m_vInfo[INFO_POS].z) / 635.f);

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
