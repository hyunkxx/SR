#pragma once

#include "Include.h"
#include "Base.h"

BEGIN(Engine)

class CCube;

class ENGINE_DLL CMeshLoader
{
	DECLARE_SINGLETON(CMeshLoader)
private:
	CMeshLoader();
	~CMeshLoader();
public:
	void Initalize();
	void LoadMesh(wstring strFileName, wstring key);
	vector<CCube*> GetMeshData(wstring key);
	bool HasValue(wstring key);
public:
	bool KeyConflictCheack(wstring key);
public:
	wstring strFilePath;
private:
	map<wstring, vector<CCube*>> mapMeshData;
};

END