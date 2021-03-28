#include <iostream>
#include <Windows.h>
#include "samp.hpp"
#include "utils.hpp"

#define MMB_TIMER_ID 10
#define RMB_TIMER_ID 11
#define COURCH_TIMER_ID 12

int mmb_count = 0;
int rmb_count = 0;

DWORD		SAMPHandle = NULL;
DWORD * pGame = NULL;
CNetGame * pNetGame = NULL;

enum class CROUCH_STATE {
	NONE,
	CROUCHING,
	IDLE,
	GETTING_UP
};

bool			CursorIsToggled = false;
bool			AutoMoveForwardToggled = false;
bool			AutoMoveBackwardToggled = false;
CROUCH_STATE	CrouchingState = CROUCH_STATE::NONE;

void Start();
void Log(const std::string & text);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Start), 0, 0, 0);
	}
	return TRUE;
}

void Start()
{
	HWND SAMPWnd = NULL;
	while (true)
	{
		SAMPWnd = FindWindow(NULL, L"GTA:SA:MP");
		if (SAMPWnd)
		{
			SAMPHandle = reinterpret_cast<DWORD>(GetModuleHandle(L"samp.dll"));
			if (SAMPHandle)
			{
				pGame = *(DWORD **)(SAMPHandle + 0x21A10C);
				pNetGame = *(CNetGame **)(SAMPHandle + 0x21A0F8);
				if (pNetGame == nullptr) continue;
				break;
			}

		};
		Sleep(200);
	}

#ifdef _DEBUG
	AllocConsole();
#endif

	freopen("CONOUT$", "w", stdout);

	while (true)
	{
		// check if focused window is GTA SA window or not, if not, ignore key checks
		HWND focusedwindow = GetForegroundWindow();
		if (focusedwindow != SAMPWnd) continue;

		if (GetAsyncKeyState(VK_MBUTTON) & 0x01)
		{
			if (mmb_count == 0)
			{
				SetTimer(SAMPWnd, MMB_TIMER_ID, 1000, [](HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
					{
						KillTimer(hWnd, MMB_TIMER_ID);
						if (mmb_count == 1)
						{
							if (AutoMoveForwardToggled)
							{
								utils::ReleaseKey('W'); // 'W' key
								AutoMoveForwardToggled = false;
								Log("Auto Move Forward is now disabled");
							}
							else
							{
								if (CursorIsToggled)
								{
									CursorIsToggled = false;
									CGame__SetCursorMode(pGame, CursorIsToggled ? 3 : 0, CursorIsToggled);
									Log(CursorIsToggled ? "Enabled mouse cursor" : "Disabled mouse cursor");
								}
								AutoMoveForwardToggled = true;
								Log("Auto Move Forward is now enabled");
							}
						}
						else if (mmb_count >= 2)
						{
							CursorIsToggled = !CursorIsToggled;

							if (CursorIsToggled)
							{
								utils::ReleaseKey('W'); // 'W' key
								utils::ReleaseKey('S'); // 'S' key
								AutoMoveForwardToggled = false;
								AutoMoveBackwardToggled = false;
								Log("Auto Move Forward is now disabled");
								Log("Auto Move Backward is now disabled");
							}

							CGame__SetCursorMode(pGame, CursorIsToggled ? 3 : 0, CursorIsToggled);
							Log((CursorIsToggled ? "Enabled mouse cursor" : "Disabled mouse cursor"));
						}
						mmb_count = 0;
					}
				);
			}
			mmb_count++;
		}

		if (GetAsyncKeyState(VK_RBUTTON) & 0x01)
		{
			if (rmb_count == 0)
			{
				SetTimer(SAMPWnd, RMB_TIMER_ID, 1000, [](HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
					{
						KillTimer(hWnd, RMB_TIMER_ID);
						if (rmb_count == 2)
						{
							// disable cursor
							if (CursorIsToggled)
							{
								CursorIsToggled = false;
								CGame__SetCursorMode(pGame, CursorIsToggled ? 3 : 0, CursorIsToggled);
								Log((CursorIsToggled ? "Enabled mouse cursor" : "Disabled mouse cursor"));
							}

							// Handle vehicle leave
							void * localPed = CGame__GetLocalPed(pGame);
							if (CPed__IsInVehicle(localPed))
							{
								CVehicle * vehicle = CPed__GetVehicle(localPed);
								if (vehicle)
								{
									if (AutoMoveBackwardToggled)
									{
										utils::ReleaseKey('S'); // 'S' key
										AutoMoveBackwardToggled = false;
									}

									if (AutoMoveForwardToggled)
									{
										utils::ReleaseKey('W'); // 'W' key
										AutoMoveForwardToggled = false;
									}

									uint16_t vehid = CVehiclePool__GetID(pNetGame->m_pPools->m_pVehicle, vehicle);
									CPed__ExitVehicle(localPed);
									CLocalPlayer__SendVehicleExitRPC(localPed, vehid);
									Log("Exiting current vehicle");
								}
							}
							else
							{
								uint16_t closestVehid = CVehiclePool__GetNearestVehicle(pNetGame->m_pPools->m_pVehicle);
								if (closestVehid < 2000 && CVehiclePool__GetSlotState(pNetGame->m_pPools->m_pVehicle, closestVehid))
								{
									CVehicle * vehicle = CVehiclePool__GetGTAObject(pNetGame->m_pPools->m_pVehicle, closestVehid);
									if (CEntity__GetDistanceToLocalPed(vehicle) < 8.0f)
									{
										// Stop auto move forward actions first, then enter vehicle just to avoid interfering each other
										utils::ReleaseKey(0x57); // 'W' key
										AutoMoveForwardToggled = false;
										Log("Auto Move Forward is now disabled");
										CPed__EnterVehicle(localPed, vehicle->m_dwHandle, FALSE);
										SendVehicleEnterRPC(closestVehid, FALSE);
									}
								}
								Log("Entering closest vehicle");
							}
						}
						else if (rmb_count >= 3)
						{
							// disable cursor
							if (CursorIsToggled)
							{
								CursorIsToggled = false;
								CGame__SetCursorMode(pGame, CursorIsToggled ? 3 : 0, CursorIsToggled);
								Log((CursorIsToggled ? "Enabled mouse cursor" : "Disabled mouse cursor"));
							}
							void * localPed = CGame__GetLocalPed(pGame);
							// Perform a vehicle enter as passenger
							if (!CPed__IsInVehicle(localPed))
							{
								uint16_t closestVehid = CVehiclePool__GetNearestVehicle(pNetGame->m_pPools->m_pVehicle);
								if (closestVehid < 2000 && CVehiclePool__GetSlotState(pNetGame->m_pPools->m_pVehicle, closestVehid))
								{
									CVehicle * vehicle = CVehiclePool__GetGTAObject(pNetGame->m_pPools->m_pVehicle, closestVehid);
									if (CEntity__GetDistanceToLocalPed(vehicle) < 8.0f)
									{
										// Stop auto move forward actions first, then enter vehicle just to avoid interfering each other
										utils::ReleaseKey(0x57); // 'W' key
										AutoMoveForwardToggled = false;
										Log("Auto Move Forward is now disabled");
										CPed__EnterVehicle(localPed, vehicle->m_dwHandle, TRUE);
										SendVehicleEnterRPC(closestVehid, TRUE);
									}
								}
								Log("Entering closest vehicle as passenger");
							}
						}
						rmb_count = 0;
					}
				);
			}
			rmb_count++;
		}

		if (GetAsyncKeyState(VK_XBUTTON1) & 0x01)
		{
			void * localPed = CGame__GetLocalPed(pGame);
			if (CPed__IsInVehicle(localPed))
			{
				if (AutoMoveBackwardToggled)
				{
					utils::ReleaseKey('S'); // 'S' key
					AutoMoveBackwardToggled = false;
					Log("Auto Move Backward is now disabled");
				}
				else
				{
					// disable cursor
					if (CursorIsToggled)
					{
						CursorIsToggled = false;
						CGame__SetCursorMode(pGame, CursorIsToggled ? 3 : 0, CursorIsToggled);
						Log((CursorIsToggled ? "Enabled mouse cursor" : "Disabled mouse cursor"));
					}
					AutoMoveBackwardToggled = true;
					Log("Auto Move Backward is now disabled");
				}
			}
			else
			{

				Log("Simulate Space key");
				utils::PressAndReleaseKey(VK_SPACE);
			}

		}

		if (GetAsyncKeyState(VK_XBUTTON2) & 0x01)
		{
			Log("Simulate C key");

			if (CursorIsToggled)
			{
				CursorIsToggled = false;
				CGame__SetCursorMode(pGame, CursorIsToggled ? 3 : 0, CursorIsToggled);
				Log((CursorIsToggled ? "Enabled mouse cursor" : "Disabled mouse cursor"));
			}

			if (CrouchingState == CROUCH_STATE::NONE)
			{
				Log("CROUCH_STATE::CROUCHING");
				CrouchingState = CROUCH_STATE::CROUCHING;

				utils::PressAndReleaseKey('C'); // 'C' key

				SetTimer(SAMPWnd, COURCH_TIMER_ID, 500, [](HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
					{
						Log("CROUCH_STATE::IDLE");
						CrouchingState = CROUCH_STATE::IDLE;

						KillTimer(hWnd, COURCH_TIMER_ID);
					}
				);

			}
			else if (CrouchingState == CROUCH_STATE::IDLE)
			{
				Log("CROUCH_STATE::GETTING_UP");
				CrouchingState = CROUCH_STATE::GETTING_UP;

				utils::PressAndReleaseKey('C'); // 'C' key

				SetTimer(SAMPWnd, COURCH_TIMER_ID, 500, [](HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
					{
						Log("CROUCH_STATE::NONE");
						CrouchingState = CROUCH_STATE::NONE;

						KillTimer(hWnd, COURCH_TIMER_ID);
					}
				);
			}
		}

		if (AutoMoveForwardToggled || AutoMoveBackwardToggled)
		{
			if (CursorIsToggled)
			{
				utils::ReleaseKey('W'); // 'W' key
				AutoMoveForwardToggled = false;
				Log("Auto Move Forward is now disabled");
				continue;
			}

			if (AutoMoveForwardToggled)
			{
				utils::PressKey('W'); // 'W' key
			}

			if (AutoMoveBackwardToggled)
			{
				utils::PressKey('S'); // 'S' key
			}
		}

		Sleep(50);
	}
}

void Log(const std::string & text)
{
#ifdef _DEBUG
	std::cout << text;
#endif
}
