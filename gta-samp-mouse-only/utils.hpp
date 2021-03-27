#pragma once
#include <Windows.h>

namespace utils
{
	inline void PressAndReleaseKey(BYTE key)
	{
		keybd_event(key,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
		Sleep(300);
		keybd_event(key,
			0x45,
			KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
			0);
	}

	inline void ReleaseKey(BYTE key)
	{
		keybd_event(key,
			0x45,
			KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
			0);
	}

	inline void PressKey(BYTE key)
	{
		keybd_event(key,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
}
