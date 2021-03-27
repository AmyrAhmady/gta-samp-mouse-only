#pragma once
#include <Windows.h>

struct CNetGame;

extern DWORD SAMPHandle;
extern DWORD * pGame;
extern CNetGame * pNetGame;

template <typename ret_type, typename... func_args>
class samp_class_function
{
public:
	typedef ret_type(__thiscall * function_t)(func_args...);

	samp_class_function(DWORD address)
	{
		this->function_address = address;
	}

	ret_type operator () (func_args... args) const
	{
		return reinterpret_cast<function_t>(SAMPHandle + this->function_address)(args...);
	}

	DWORD function_address;
};

template <typename ret_type, typename... func_args>
class samp_function
{
public:
	typedef ret_type(__stdcall * function_t)(func_args...);

	samp_function(DWORD address)
	{
		this->function_address = address;
	}

	ret_type operator () (func_args... args) const
	{
		return reinterpret_cast<function_t>(SAMPHandle + this->function_address)(args...);
	}

	DWORD function_address;
};


#pragma pack(push, 1)
struct Pools {
	char pad_0[24];
	void * m_pPlayer;
	void * m_pVehicle;
	char pad_1[4];
};

struct CNetGame
{
	char pad_0[973];
	Pools * m_pPools;
};

struct CVehicle
{
	char pad_0[68];
	int m_dwHandle;
};
#pragma pack(pop)

samp_class_function<void, void *, int, bool> CGame__SetCursorMode(0x9BD30);
samp_class_function<DWORD *, void *> CGame__GetLocalPed(0x1010);
samp_class_function<uint16_t, void *> CVehiclePool__GetNearestVehicle(0x1B110);
samp_class_function<int, void *, uint16_t> CVehiclePool__GetSlotState(0x1140);
samp_class_function<CVehicle *, void *, uint16_t> CVehiclePool__GetGTAObject(0x1110);
samp_class_function<uint16_t, void *, CVehicle *> CVehiclePool__GetID(0x1110);
samp_class_function<float, void *> CEntity__GetDistanceToLocalPed(0x9A700);
samp_class_function<CVehicle *, void *> CPed_GetVehicle(0xA68C0);
samp_class_function<void, void *, int, BOOL> CPed_EnterVehicle(0xA6CD0);
samp_class_function<void, void *> CPed_ExitVehicle(0xA6DA0);
samp_class_function<BOOL, void *> CPed_IsInVehicle(0xA6730);
samp_class_function<void, void *, int> CLocalPlayer__SendVehicleExitRPC(0x59E0);

samp_function<void, int, BOOL> SendVehicleEnterRPC(0x58C0);
