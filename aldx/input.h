#pragma once
#include "helper.h"
#include "gamepad.h"

namespace aldx
{
	//mouse_button
	// Flags Enum that represents the buttons on a mouse
	enum class mouse_button
	{
		none = 0,
		left = 0x00000001,
		middle = 0x00000002,
		right = 0x00000004,
		x1 = 0x00000008,
		x2 = 0x00000010,
	};

	inline mouse_button operator |(mouse_button a, mouse_button b)
	{
		return (mouse_button)(((uint32)a) | (uint32)b);
	}
	inline mouse_button operator &(mouse_button a, mouse_button b)
	{
		return (mouse_button)(((uint32)a)&(uint32)b);
	}

	//mouse
	// Class that represents a Mouse
	class mouse
	{
	public:
		static float2 position();

		static float2 deltaPosition();

		static bool button_down(mouse_button button);

		static bool button_up(mouse_button button);

#ifdef WIN32
		static void ___set_mouse_state(float2 pos, mouse_button mb);
#endif
	};

	inline bool __IsKeyDown(int vkey)
	{
		return (GetAsyncKeyState(vkey) & 0x8000) > 0;
	}

	//Keyboard
	// Static Class that represents a Keyboard
	class keyboard
	{
	public:
		inline static bool key_down(uint32 key)
		{
			return ::__IsKeyDown(key);
		}

		inline static bool key_up(uint32 key)
		{
			return !::__IsKeyDown(key);
		}
	};

}