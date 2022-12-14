LPDIRECT3DDEVICE9	Get_GraphicDev(void)
{
	return CGraphicDev::GetInstance()->Get_GraphicDev();
}
HRESULT				Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void					Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void					Render_End(HWND hWnd)
{
	CGraphicDev::GetInstance()->Render_End(hWnd);
}

// TimerMgr

HRESULT		Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}
_float		Get_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_Timer(pTimerTag);
}
void			Set_Timer(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Set_Timer(pTimerTag);
}

HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fTimeDelta);
}
_bool		IsPermit_Call(const _tchar * pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}

HRESULT			Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}
void				Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
}

// InputDev
_byte		Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID)
{
	return CInputDev::GetInstance()->Get_DIMouseState(byMouseID);
}
KEY_STATE	Get_DIKeyState_Custom(_int _key)
{
	return  CInputDev::GetInstance()->Get_DIKeyState_Custom(_key);
}
KEY_STATE	Get_DIMouseState_Custom(MOUSEKEYSTATE _key)
{
	return  CInputDev::GetInstance()->Get_DIMouseState_Custom(_key);
}
_long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(eMoveState);
}
HRESULT		Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}
void		Update_InputDev(void)
{
	CInputDev::GetInstance()->Update_InputDev();
}
void		SetUp_InputDev(void)
{
	CInputDev::GetInstance()->SetUp_InputDev();
}

//CollisionMgr

_bool Sphere_Collision(_vec3 First, _vec3 Second, _float first_range, _float Second_range)
{
	return CCollisionMgr::GetInstance()->Sphere_Collision(First, Second, first_range, Second_range);
}
_vec3 Sphere_Collision_EX(_vec3 First, _vec3 Second, _float first_range, _float Second_range)
{
	return CCollisionMgr::GetInstance()->Sphere_Collision_EX(First, Second, first_range, Second_range);
}
_bool OBB_Collision(OBB* First, OBB* Second)
{
	return CCollisionMgr::GetInstance()->OBB_Collision(First, Second);
}

//BulletMgr
inline void Update_BulletMgr(const _float & fTimeDelta)
{
	CBulletMgr::GetInstance()->Update_BulletMgr(fTimeDelta);
}
inline void LateUpdate_BulletMgr(void)
{
	CBulletMgr::GetInstance()->LateUpdate_BulletMgr();
}
inline void Bullet_Supply(CGameObject* pBullet, BULLET_ID eID)
{
	CBulletMgr::GetInstance()->Bullet_Supply(pBullet, eID);
}
inline void Collect_Object(void)
{
	CBulletMgr::GetInstance()->Collect_Object();
}
inline void Reuse_Object(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float fAngleY, BULLET_ID eID)
{
	CBulletMgr::GetInstance()->Reuse_Object(vPos, vDir, fSpeed, fAngleX, fAngleY, eID);
}
inline list<CGameObject*>* Get_Bullet_List(BULLET_ID eID)
{
	return CBulletMgr::GetInstance()->Get_Bullet_List(eID);
}
// SoundMgr
inline void Ready_Sound(void)
{
	return SoundMgr::GetInstance()->Ready_Sound();
}

void PlaySound_SR(_tchar* pSoundKey, SoundType eID, _float fVolume)
{
	return SoundMgr::GetInstance()->PlaySound_SR(pSoundKey, eID, fVolume);
}

void PlayBGM(_tchar* pSoundKey, _float fVolume)
{
	return SoundMgr::GetInstance()->PlayBGM(pSoundKey, fVolume);
}
void StopSound(SoundType eID)
{
	return SoundMgr::GetInstance()->StopSound(eID);
}
void StopAll()
{
	return SoundMgr::GetInstance()->StopAll();
}
void SetChannelVolume(SoundType eID, _float fVolume)
{
	return SoundMgr::GetInstance()->SetChannelVolume(eID, fVolume);
}

//Enermy
inline void Enermy_Add(CGameObject * pBullet, OBJID eID)
{
	CEnermyMgr::GetInstance()->Enermy_Add(pBullet, eID);
}

inline void Update_EnermyMgr(const _float & fTimeDelta)
{
	CEnermyMgr::GetInstance()->Update_EnermyMgr(fTimeDelta);
}
inline void LateUpdate_EnermyMgr(void)
{
	CEnermyMgr::GetInstance()->LateUpdate_EnermyMgr();
}

//CameraMgr
HRESULT	Add_Camera(const _tchar* CameraTag, CCamera* pCamera)
{
	return CCameraMgr::GetInstance()->Add_Camera(CameraTag, pCamera);
}
void	Update_CameraMgr(const _float& fTimeDelta)
{
	CCameraMgr::GetInstance()->Update_CameraMgr(fTimeDelta);
}
void	LateUpdate_CameraMgr(void)
{
	CCameraMgr::GetInstance()->LateUpdate_CameraMgr();
}
void	Camera_Change(const _tchar* CameraTag)
{
	CCameraMgr::GetInstance()->Camera_Change(CameraTag);
}
CAMERA_ID	Get_Camera_ID(void)
{
	return CCameraMgr::GetInstance()->Get_Camera_ID();
}
CCamera*	Get_Camera(void)
{
	return CCameraMgr::GetInstance()->Get_Camera();
}
CCamera*	Get_Camera(const _tchar* CameraTag)
{
	return CCameraMgr::GetInstance()->Get_Camera(CameraTag);
}

inline void			Release_System(void)
{
	CCameraMgr::GetInstance()->DestroyInstance();
	SoundMgr::GetInstance()->DestroyInstance();
	CBulletMgr::GetInstance()->DestroyInstance();
	CEnermyMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	CInputDev::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}