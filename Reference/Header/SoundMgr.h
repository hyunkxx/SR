#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL SoundMgr : public CBase
{
	DECLARE_SINGLETON(SoundMgr)

private:
	explicit SoundMgr();
	virtual ~SoundMgr();

public:
	void Ready_Sound(void);

	void PlaySound_SR(_tchar* pSoundKey, SoundType eID, _float fVolume);
	void PlayBGM(_tchar* pSoundKey, _float fVolume);
	void StopSound(SoundType eID);
	void StopAll();
	void SetChannelVolume(SoundType eID, _float fVolume);

	virtual void Free(void)override;

private:
	void LoadSoundFile();

	// 사운드 리소스 정보를 갖는 객체 
	map<_tchar*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[SOUND_END];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

};

END