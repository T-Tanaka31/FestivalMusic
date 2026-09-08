#include "InputManager.h"

//	静的メンバ変数の初期化
InputManager* InputManager::pInstance = nullptr;

void InputManager::CreateInstance() {
	pInstance = new InputManager();
}

InputManager* InputManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();
	return pInstance;
}

void InputManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

InputManager::InputManager()
	: keyState("")
	, prevkeyState("")
	, padState()
	, prevPadState()
	, mouseInput(-1)
	, prevMouseInput(-1)
	, sthickState()
	, rangeOfMotion(32767.0f)
{
}

/*
 * @function	Update
 * @brief		更新処理
 */
void InputManager::Update() {
	// １フレーム前のキーの状態を保存
	memcpy_s(prevkeyState, 256, keyState, 256);
	// 今のフレームの状態を保存
	GetHitKeyStateAll(keyState);

	// １フレーム前のキーの状態を保存
	prevPadState = padState;
	// 今のフレームの状態を保存
	DWORD i = XInputGetState(0, &padState);

	// 接続が取れているか確認
	if (i == ERROR_SUCCESS) {
		i = ERROR_SUCCESS;
	}

	//スティックの取得
	//GetJoypadXInputState(DX_INPUT_PAD1, &sthickState);

	prevMouseInput = mouseInput;
	mouseInput = GetMouseInput();

}
