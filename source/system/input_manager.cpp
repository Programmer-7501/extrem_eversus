
#include"main.h"
#include "input_manager.h"

#pragma warning(disable : 4995)

namespace InputData
{
	static constexpr int GamePadInputArray[] =
	{
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_DPAD_UP,
		XINPUT_GAMEPAD_DPAD_DOWN,
		XINPUT_GAMEPAD_DPAD_LEFT,
		XINPUT_GAMEPAD_DPAD_RIGHT,
		XINPUT_GAMEPAD_RIGHT_SHOULDER,
		XINPUT_GAMEPAD_LEFT_SHOULDER,
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_Y,
		XINPUT_GAMEPAD_RIGHT_SHOULDER,
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_Y,
		XINPUT_GAMEPAD_A
	};

	static constexpr int KeyBoardInputArray[] =
	{
		VK_RETURN,
		VK_BACK,
		'W',
		'S',
		'A',
		'D',
		'E',
		'Q',
		VK_SPACE,
		'R',
		'T',
		'F',
		VK_LEFT,
		VK_RIGHT,
		VK_UP,
		VK_DOWN,
	};

	static const SHORT k_StickDeadZone = 1500;
}

void InputManager::Init()
{
	for (int i = 0; i < InputManagerData::k_InputKeepFrame; i++)
	{
		m_KeepState[i] = 0;
	}
}

void InputManager::Uninit()
{
}

void InputManager::Update()
{
	// 前回の入力状態を保存
	m_PrevState = m_CurrentMobileSuitState;

	m_KeepState[m_KeepIncrement] = m_CurrentMobileSuitState;
	m_KeepIncrement++;
	m_KeepIncrement %= InputManagerData::k_InputKeepFrame;

	// ゲームパッドの入力情報取得
	m_Is_GamePad_Use = XInputGetState(0, &m_State);

	// 変数を初期化
	m_CurrentMobileSuitState = 0;
	// ゲームパッドが接続されている
	if (m_Is_GamePad_Use == ERROR_SUCCESS)
	{
		for (int i = 0; i < IN_MAX; i++)
		{
			// ステックが入力されているかどうかを図る
			if (i == IN_LEFT_STICK)
			{
				if (GetLeftStickIsInput() == true)
				{
					m_CurrentMobileSuitState |= (1 << i);
				}
				continue;
			}
			if (m_State.Gamepad.wButtons & InputData::GamePadInputArray[i])
			{
				m_CurrentMobileSuitState |= (1 << i);
			}
		}
	}
	else if (m_Is_GamePad_Use == ERROR_DEVICE_NOT_CONNECTED)// ゲームパッドが接続されていない
	{
		// キー入力取得
		GetKeyboardState(m_InputByteBuffer);

		for (int i = 0; i < IN_MAX; i++)
		{
			// ステックが入力されているかどうかを図る
			if (i == IN_LEFT_STICK)
			{
				if (m_InputByteBuffer['W'] & 0x80)
				{
					m_CurrentMobileSuitState |= (1 << i);
					continue;
				}
				if (m_InputByteBuffer['S'] & 0x80)
				{
					m_CurrentMobileSuitState |= (1 << i);
					continue;
				}
				if (m_InputByteBuffer['A'] & 0x80)
				{
					m_CurrentMobileSuitState |= (1 << i);
					continue;
				}
				if (m_InputByteBuffer['D'] & 0x80)
				{
					m_CurrentMobileSuitState |= (1 << i);
					continue;
				}
				continue;
			}

			if (m_InputByteBuffer[InputData::KeyBoardInputArray[i]] & 0x80)
			{
				m_CurrentMobileSuitState |= (1 << i);
			}
		}
	}

	// 押した瞬間と離した瞬間のキーを取得	
	m_Trigger = (m_PrevState ^ m_CurrentMobileSuitState) & m_CurrentMobileSuitState;
	m_Release = (m_PrevState ^ m_CurrentMobileSuitState) & m_PrevState;
}

bool InputManager::GetIsUseGamePad()
{
	// ゲームパッドが接続されている
	if (m_Is_GamePad_Use == ERROR_SUCCESS)
	{
		return true;
	}

	return false;
}

bool InputManager::GetIsTrigger(InputName inputName)
{
	return m_Trigger & (1u << inputName);
}

bool InputManager::GetIsPress(InputName inputName)
{
	return m_CurrentMobileSuitState & (1u << inputName);
}

bool InputManager::GetIsRelease(InputName inputName)
{
	return m_Release & (1u << inputName);
}

bool InputManager::GetIsKeepState(InputName inputName, int prevFrame)
{
	int index = 0;
	if (m_KeepIncrement >= prevFrame)
	{
		index = m_KeepIncrement - prevFrame;
	}
	else
	{
		int dif = m_KeepIncrement - prevFrame;
		index = InputManagerData::k_InputKeepFrame - dif;
	}

	return m_KeepState[index] & (1u << inputName);
}

SHORT InputManager::GetLeftStickX()
{
	SHORT res;
	// ゲームパッドが接続されている
	if (m_Is_GamePad_Use == ERROR_SUCCESS)
	{
		// デッドゾーンより入力されていたら
		if (abs(m_State.Gamepad.sThumbLX) > InputData::k_StickDeadZone)
		{
			res = m_State.Gamepad.sThumbLX;
		}
		else
		{
			res = 0;
		}
	}
	else
	{
		res = 0;
	}
	return res;
}

SHORT InputManager::GetLeftStickY()
{
	SHORT res;
	// ゲームパッドが接続されている
	if (m_Is_GamePad_Use == ERROR_SUCCESS)
	{
		// デッドゾーンより入力されていたら
		if (abs(m_State.Gamepad.sThumbLY) > InputData::k_StickDeadZone)
		{
			res = m_State.Gamepad.sThumbLY;
		}
		else
		{
			res = 0;
		}
	}
	else
	{
		res = 0;
	}
	return res;
}

SHORT InputManager::GetRightStickX()
{
	SHORT res;
	// ゲームパッドが接続されている
	if (m_Is_GamePad_Use == ERROR_SUCCESS)
	{
		// デッドゾーンより入力されていたら
		if (abs(m_State.Gamepad.sThumbRX) > InputData::k_StickDeadZone)
		{
			res = m_State.Gamepad.sThumbRX;
		}
		else
		{
			res = 0;
		}
	}
	else
	{
		res = 0;
	}
	return res;
}

SHORT InputManager::GetRightStickY()
{
	SHORT res;
	// ゲームパッドが接続されている
	if (m_Is_GamePad_Use == ERROR_SUCCESS)
	{
		// デッドゾーンより入力されていたら
		if (abs(m_State.Gamepad.sThumbRY) > InputData::k_StickDeadZone)
		{
			res = m_State.Gamepad.sThumbRY;
		}
		else
		{
			res = 0;
		}
	}
	else
	{
		res = 0;
	}

	return res;
}

Conv_XM::Vector2f InputManager::GetLeftStickeVector()
{
	SHORT x, y;
	Conv_XM::Vector2f res;
	x = GetLeftStickX();
	y = GetLeftStickY();
	res = Conv_XM::Vector2f(static_cast<float>(x), static_cast<float>(y));
	res = DirectX::XMVector2Normalize(res);
	return res;
}

Conv_XM::Vector2f InputManager::GetRightStickeVector()
{
	SHORT x, y;
	Conv_XM::Vector2f res;
	x = GetRightStickX();
	y = GetRightStickY();
	res = Conv_XM::Vector2f(static_cast<float>(x), static_cast<float>(y));
	res = DirectX::XMVector2Normalize(res);
	return res;
}

bool InputManager::GetLeftStickIsInput()
{
	SHORT x, y;
	x = GetLeftStickX();
	y = GetLeftStickY();

	if (abs(x) > 0 || abs(y) > 0)
	{
		return true;
	}

	return false;
}

bool InputManager::GetRightStickIsInput()
{
	SHORT x, y;
	x = GetRightStickX();
	y = GetRightStickY();

	if (abs(x) > 0 || abs(y) > 0)
	{
		return true;
	}

	return false;
}
