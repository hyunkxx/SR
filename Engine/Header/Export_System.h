#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "InputDev.h"
#include "CollisionMgr.h"
#include "BulletMgr.h"
#include "SoundMgr.h"
#include"EnermyMgr.h"
#include "CameraMgr.h"
#include "SP_EffectMgr.h"
BEGIN(Engine)

// GraphicDev
inline LPDIRECT3DDEVICE9	Get_GraphicDev(void);
inline HRESULT				Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev);
inline void					Render_Begin(D3DXCOLOR Color);
inline void					Render_End(HWND hWnd = nullptr);

// TimerMgr

inline HRESULT		Ready_Timer(const _tchar* pTimerTag);
inline _float		Get_Timer(const _tchar* pTimerTag);
inline void			Set_Timer(const _tchar* pTimerTag);

// FrameMgr
inline HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fTimeDelta);
inline _bool		IsPermit_Call(const _tchar * pFrameTag, const _float& fTimeDelta);

// FontMgr

inline HRESULT			Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth,	const _uint& iHeight, const _uint& iWeight);
inline void				Render_Font(const _tchar* pFontTag,	const _tchar* pString,	const _vec2* pPos,	D3DXCOLOR Color);


// InputDev
inline _byte		Get_DIKeyState(_ubyte byKeyID);
inline _byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID);
inline KEY_STATE	Get_DIKeyState_Custom(_int _key);
inline KEY_STATE	Get_DIMouseState_Custom(MOUSEKEYSTATE _key);
inline _long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState);
inline	HRESULT		Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline  void		Update_InputDev(void);
inline	void		SetUp_InputDev(void);

//CollisionMgr
inline _bool Sphere_Collision(_vec3 First, _vec3 Second, _float first_range, _float Second_range);
inline _vec3 Sphere_Collision_EX(_vec3 First, _vec3 Second, _float first_range, _float Second_range);
inline _bool OBB_Collision(OBB* First, OBB* Second);
//BulletMgr
inline void Update_BulletMgr(const _float & fTimeDelta);
inline void LateUpdate_BulletMgr(void);
inline void Bullet_Supply(CGameObject* pBullet, BULLET_ID eID);
inline list<CGameObject*>* Get_Bullet_List(BULLET_ID eID);
inline void Collect_Object(void);
inline CGameObject* Reuse_Object(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float fAngleY, BULLET_ID eID);
//EffectMgr
inline void Update_CSP_EffectMgr(const _float & fTimeDelta);
inline void LateUpdate_CSP_EffectMgr(void);
inline void Effect_Supply(CGameObject* pBullet, EFFECT_ID eID);
inline void Collect_Effect(void);
inline CGameObject* Reuse_Effect(EFFECT_ID eID);
// SoundMgr
inline void Ready_Sound(void);
inline void PlaySound_SR(_tchar* pSoundKey, SoundType eID, _float fVolume);
inline void PlayBGM(_tchar* pSoundKey, _float fVolume);
inline void StopSound(SoundType eID);
inline void StopAll();
inline void SetChannelVolume(SoundType eID, _float fVolume);

//EnermyMgr

inline void Enermy_Add(CGameObject* pBullet, OBJID eID);
inline void Update_EnermyMgr(const _float & fTimeDelta);
inline void LateUpdate_EnermyMgr(void);

//CameraMgr
inline HRESULT	Add_Camera(const _tchar* CameraTag, CCamera* pCamera);
inline void	Update_CameraMgr(const _float& fTimeDelta);
inline void	LateUpdate_CameraMgr(void);
inline void	Camera_Change(const _tchar* CameraTag);
inline CAMERA_ID	Get_Camera_ID(void);
inline CCamera*	Get_Camera(void);
inline CCamera*	Get_Camera(const _tchar* CameraTag);

inline void			Release_System(void);

#include "Export_System.inl"

END

#endif // Export_System_h__
