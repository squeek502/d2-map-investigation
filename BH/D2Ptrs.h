#pragma once
// Macros adapted from lord2800's macros.
#include "D2Helpers.h"
#include "D2Structs.h"

#ifdef _DEFINE_PTRS
#define FUNCPTR(dll, name, callingret, args, address) \
	__declspec(naked) callingret dll##_##name##args \
	{ \
		static DWORD f##dll##_##name = NULL; \
		if(f##dll##_##name == NULL) \
		{ \
		__asm { pushad } \
		f##dll##_##name = GetDllOffset(dll, address); \
		__asm { popad } \
		} \
		__asm jmp [f##dll##_##name] \
	}

#define ASMPTR(dll, name, address) \
	DWORD* Asm_##dll##_##name##(VOID) \
	{ \
		static DWORD f##Asm_##dll##_##name = NULL; \
		if(f##Asm_##dll##_##name## == NULL) \
		{ \
		f##Asm_##dll##_##name## = GetDllOffset(dll, address); \
		} \
		return &##f##Asm_##dll##_##name; \
	} 

#define VARPTR(dll, name, type, address) \
	type** Var_##dll##_##name##(VOID) \
	{ \
		static DWORD f##Var_##dll##_##name = NULL; \
		if(f##Var_##dll##_##name## == NULL) \
		{ \
		f##Var_##dll##_##name## = GetDllOffset(dll, address); \
		} \
		return (type**)&##f##Var_##dll##_##name; \
	} 

#else
#define FUNCPTR(dll, name, callingret, args, address) extern callingret dll##_##name##args;
#define ASMPTR(dll, name, address) extern DWORD* Asm_##dll##_##name##(VOID); static DWORD dll##_##name = *Asm_##dll##_##name##();
#define VARPTR(dll, name, type, address) extern type** Var_##dll##_##name##(VOID); static type* p##_##dll##_##name = (type*)*Var_##dll##_##name##();
#endif

#ifdef D2_VERSION_113D
#include "D2Ptrs_113d.h"
#else
#include "D2Ptrs_113c.h"
#endif

#undef FUNCPTR
#undef ASMPTR
#undef VARPTR

#define D2CLIENT_TestPvpFlag(dwId1, dwId2, dwFlag)	(TestPvpFlag_STUB(dwId1, dwId2, dwFlag))
#define D2CLIENT_GetUnitName(x)				(wchar_t*)D2CLIENT_GetUnitName_STUB((DWORD)x)
#define GetUnitFromId(unitid, unittype)		((UnitAny *)D2CLIENT_GetUnitFromId_STUB(unitid, unittype))
#define D2CLIENT_GetLevelName(LevelId) (wchar_t*)D2CLIENT_GetLevelName_STUB(LevelId)
#define D2CLIENT_GetUIState(dwVarNo)					(D2CLIENT_GetUIVar_STUB(dwVarNo))
#define D2CLIENT_ClickParty(RosterUnit, Mode)		(D2CLIENT_ClickParty_ASM(RosterUnit, Mode))
#define D2CLIENT_xShake (*p_D2CLIENT_xShake)	//GameShake
#define D2CLIENT_yShake (*p_D2CLIENT_yShake)	//GameShake