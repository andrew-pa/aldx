#pragma once
#include "helper.h"
#include <Xinput.h>

namespace aldx
{
	//gamepad
	// XInput Gamepad wrapper
	class gamepad
	{
	public:
		gamepad() { }
		gamepad(DWORD usrIdx);

		inline void update(bool throw_on_not_connected = false)
		{
			lerrCode = XInputGetState(usrIdx, &state);
			if (lerrCode == ERROR_DEVICE_NOT_CONNECTED && throw_on_not_connected)
			{
				char msg[64];
				sprintf_s(msg, "Gamepad %d not connected", usrIdx);
				throw error_code_exception(lerrCode, msg);
			}
			else if (lerrCode != ERROR_SUCCESS && lerrCode != ERROR_DEVICE_NOT_CONNECTED)
			{
				char msg[64];
				sprintf_s(msg, "Gamepad error %d", lerrCode);
				throw error_code_exception(lerrCode, msg);
			}
		}

		inline float dpc(short v, short z, float max = SHRT_MAX) const
		{
			float vn = clamp(static_cast<float>(v) / max);
			if (abs(vn) < (float)z / max)
				vn = 0;
			return vn;
		}

		inline float2 left_thumb() const
		{
			return float2(dpc(state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE), dpc(state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
		}
		inline float2 right_thumb() const
		{
			return float2(dpc(state.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE), dpc(state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
		}
		inline float left_trigger() const
		{
			return dpc(state.Gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, BYTE_MAX);
		}
		inline float right_trigger() const
		{
			return dpc(state.Gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, BYTE_MAX);
		}
		inline bool is_button_down(unsigned short button) const
		{
			return (state.Gamepad.wButtons & button) != 0;
		}
		inline bool connected() const { return (lerrCode != ERROR_DEVICE_NOT_CONNECTED); }

		inline void set_vibration(float2 vibe) const
		{
			XINPUT_VIBRATION vi;
			vi.wLeftMotorSpeed = (WORD)(vibe.x / (float)0xffff);
			vi.wRightMotorSpeed = (WORD)(vibe.y / (float)0xffff);
			XInputSetState(usrIdx, &vi);
		}

		inline XINPUT_STATE get_state() const { return state; }

		inline DWORD& user_index() { return usrIdx; }

	protected:
		DWORD usrIdx;
		XINPUT_STATE state;
		DWORD lerrCode;
	};

}