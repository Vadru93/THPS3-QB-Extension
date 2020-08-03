#pragma once
#ifndef SKATER_H
#define SKATER_H
#include "_Math.h"
#include "KeyState.h"
#include "Script.h"


//Skater is not yet looked into that much.... 
struct Settings
{
	BYTE unk[256];
	typedef bool(__thiscall* const pIsTrue)(Settings* pThis, const char* name);
	bool IsTrue(const char* name)
	{
		return pIsTrue(0x004342E0)(this, name);
	}
};

struct SkaterProfile
{
	BYTE unk[256];

	typedef Settings* (__thiscall* const pGetSettings)(SkaterProfile* pThis);

	Settings* GetSettings()
	{
		return pGetSettings(0x00438B00)(this);
	}
};

struct SkaterProfileContainer
{
	SkaterProfile* profile;

	typedef SkaterProfile* (__cdecl* const GetSkaterProfile)(bool allocate);

	SkaterProfileContainer()
	{
		profile = GetSkaterProfile(0x004367E0)(false);//increas vp count
	}

	typedef void(__cdecl* const ReleaseSkaterProfile)();

	~SkaterProfileContainer()
	{
		ReleaseSkaterProfile(0x00436830)();//decrease vp count
	}
};

//#pragma pack(1)
//Skater: contains information about camera, position, collision etc
EXTERN struct Skater//GetSkaterInfo 004769D0
{
private://0575a190
	DWORD** memberFunctions;
	BYTE unk1[0x14];
	D3DXVECTOR3 position;
	float positionW;//1.0f
	D3DXVECTOR3 oldpos;
	float oldposW;//1.0f
	BYTE unk4[0x2FC];
	D3DXVECTOR3 velocity;
	float random;//1.0 when standing still, else a high number
	BYTE unk5[0x10];
	D3DXMATRIX matrix;
	BYTE unknown5[0x3E];
	bool bailOn;
	bool landedfromvert;
	bool truelandedfromvert;
	BYTE unknown4[0x7F4B];
	bool inVert;
	BYTE unknown6[0xF];
	bool tracking;
	BYTE unknown7[0x1F];
	bool canbreakvert;
	BYTE unknown8[0x4F];//1 more
	bool autoturn;
	BYTE unknown[0x57];//1 more
	D3DXMATRIX  old;
	D3DXMATRIX  lerp;//1 more
	BYTE unknown2[0x58];
	KeyState keystates[12];
	BYTE unknown3[0x3C];
	D3DXVECTOR3 startcol;
	float startcolW;//1.0f
	D3DXVECTOR3 endcol;
	float endcolW;//1.0f
	void* scripts;
	DWORD flag;//0 1 idk?
	float unk;//some normal??
	D3DXVECTOR3 normal;
	D3DXVECTOR3 hitpoint;
	BYTE unk3[0x134];
	D3DXVECTOR4 displaynormal;
	D3DXVECTOR4 currentnormal;
	D3DXVECTOR4 lastdisplaynormal;
	float wtf;
	float framelength;
	float un1;
	float un2;
	float normallerp;

//#pragma pop(pack)
public:

	bool IsHost()
	{
		return *(bool*)(this + 0x84CC);
	}
	DWORD GetCurrentTime()
	{
		static const DWORD timer = 0x00409AE0;
		_asm call timer
		static DWORD temp = 0;
		_asm mov temp, eax
		return temp;
	}

	//the press is between 0x0-0xFF, press below or equal to 0x40 is deadzone
	void UpdateKeyState(BYTE key, DWORD press)
	{

		GetKeyState(key)->Update(this->GetCurrentTime(), press);

		
		//_printf("Updating press %d\nKeyState %p chc %X\n", press, this, this->checksum);
		/*typedef void(__thiscall* const pUpdate)(KeyState* pThis, DWORD press);
		pUpdate(0x0049BAA0)(this, press);*/
	}

	static DWORD GetCamModeAddress()
	{
		DWORD ptr = *(DWORD*)0x00930BB0;
		ptr = *(DWORD*)ptr;
		return ptr + 0x882C;
	}

	D3DXVECTOR4* GetCurrentNormal()
	{
		return &currentnormal;
	}

	typedef void(__thiscall* const pSetNormal)(Skater* pThis, D3DXVECTOR4& normal);
	void SetNormal(D3DXVECTOR4& normal)
	{

		D3DXMATRIX* pMatrix = GetMatrix();
		displaynormal = *(D3DXVECTOR4*)pMatrix->m[Y];
		currentnormal = normal;
		lastdisplaynormal = displaynormal;
		normallerp = 0.1f;
		*(D3DXVECTOR4*)pMatrix->m[Y] = normal;
		OrthoNormalizeAbout(GetMatrix(), Y);
	}
	void SetNormalFast(D3DXVECTOR4& normal)
	{

		D3DXMATRIX* pMatrix = GetMatrix();
		displaynormal = *(D3DXVECTOR4*)pMatrix->m[Y];
		currentnormal = normal;
		lastdisplaynormal = displaynormal;
		normallerp = 1.0f;
		*(D3DXVECTOR4*)pMatrix->m[Y] = normal;
		OrthoNormalizeAbout(GetMatrix(), Y);
	}


	typedef float(__thiscall* const pGetScriptedStat)(Skater* pThis, char* name);
	float GetScriptedStat(char* name)
	{
		return pGetScriptedStat(0x0049F670)(this, name);
	}


	typedef bool(__thiscall* const pCallMemberFunction)(Skater* pThis, DWORD checksum, CStruct* pStruct, CScript* pScript);
	bool CallMemberFunction(DWORD checksum, CStruct* pStruct, CScript* pScript)
	{
		DWORD* pMemberFunction = memberFunctions[1];
		_printf("Skater -> MemberFunction %p\nName %s pStruct %p pScript %p\n", pMemberFunction, FindChecksumName(checksum), pStruct, pScript);
		return pCallMemberFunction((DWORD)pMemberFunction)(this, checksum, pStruct, pScript);
	}


	bool IsTracking()
	{
		return tracking;
	}

	bool InVert()
	{
		return inVert;
	}
	void SetLanded(bool value)
	{
		landedfromvert = value;
		truelandedfromvert = value;
	}


	KeyState* GetKeyState(BYTE idx)
	{
		if (idx > 11)
			idx = 11;
		return &keystates[idx];
	}

	float GetFrameLength()
	{
		return framelength;
	}

	static Skater* GetSkater()
	{
		static const DWORD ptr = 0x005D06C0;
		VALIDATE_PTR((void*)ptr);
		DWORD pSkater = *(DWORD*)ptr + 0x580;
		VALIDATE_PTR((void*)pSkater);
		pSkater = *(DWORD*)pSkater + 0x4;
		VALIDATE_PTR((void*)pSkater);
		pSkater = *(DWORD*)pSkater + 0x2C;
		VALIDATE_PTR((void*)pSkater);
		pSkater = *(DWORD*)pSkater + 0x48;
		VALIDATE_PTR((void*)pSkater);
		pSkater = *(DWORD*)pSkater;
		VALIDATE_DATA((Skater*)pSkater, sizeof(Skater));
		return (Skater*)pSkater;
	}

	D3DXMATRIX* GetMatrix()
	{
		return &matrix;
	}


	//used for spine transfer, don't call this
	EXTERN void Store();

	//used for spine transfer, don't call this
	EXTERN void Restore();

	//used for spine transfer, don't call this
	EXTERN void Slerping();

	//used for spine transfer, don't call this
	EXTERN void MultiplyMatrix(const D3DXMATRIX& delta);

	//used for spine transfer, don't call this
	EXTERN bool CheckForSpine();

	//used for spine transfer, don't call this
	EXTERN void ResetLerpingMatrix();


	D3DXVECTOR3* GetCollisionNormal()
	{
		return &normal;
	}



	//Tell Physcs we are inside vert air
	EXTERN void SetVertAir(bool value);

	EXTERN void SetCanBreakVert(bool value);

	typedef void(__thiscall* const pTriggerScript)(Skater* pThis, DWORD triggerType, void*);
	void TriggerScript(DWORD triggerType)
	{
		pTriggerScript(0x0049D070)(this, triggerType, scripts);
	}


	//call this before CheckCollision to set the ray for raytracing.
	EXTERN void SetRay(D3DXVECTOR3 start, D3DXVECTOR3 end);

	void SetNormals()
	{
		*(D3DXVECTOR3*)&currentnormal = normal;
		currentnormal.w = 1.0f;
	}

	//clear 
	typedef void(__thiscall* const pFlagException)(Skater* pThis, const char* name, DWORD unk2);
	void FlagException(const char* name, DWORD unk2 = 0)
	{
		pFlagException(0x0048F990)(this, name, unk2);
	}

	EXTERN bool CollisionCheck(BYTE flag, bool ignore = true);

	D3DXVECTOR3* GetNormal()
	{
		return &*(D3DXVECTOR3*)&currentnormal;
	}


	D3DXVECTOR3* GetHitPoint()
	{
		return &hitpoint;
	}


	D3DXVECTOR3* GetOldPosition()
	{
		return &oldpos;
	}

	D3DXVECTOR3* GetPosition()
	{
		return &position;
	}


	void SetPosition(D3DXVECTOR3 pos)
	{
		oldpos = position;
		position = pos;

	}


	D3DXVECTOR3* GetVelocity()
	{
		/*DWORD pSpeed = (DWORD)this + 0x334;
		VALIDATE_DATA((D3DXVECTOR3*)pSpeed, sizeof(D3DXVECTOR3));

		return (D3DXVECTOR3*)pSpeed;*/
		return &velocity;
	}
};
#endif