/**
* @file input_manager.h
* @brief 入力制御
*/

#pragma once

#include"../function/singleton.h"

namespace InputManagerData
{
	//! 入力を記憶するフレーム
	static constexpr int k_InputKeepFrame = 14;
}

/**
* @namespace InputManager
* @brief 入力情報クラス
* @details ゲームパッドが接続されていたらキー入力は無視される
*/
class InputManager final : public Singleton<InputManager>
{
public:
	enum InputName
	{
		//! 決定
		IN_DECIDE,
		//! 戻る
		IN_RETURN,
		//! 前入力
		IN_FRONT,
		//! 後ろ入力
		IN_BACK,
		//! 左入力
		IN_LEFT,
		//! 右入力
		IN_RIGHT,
		//! 上入力
		IN_UP,
		//! 下入力
		IN_DOWN,
		//! ジャンプ入力
		IN_JUMP,
		//! メイン入力
		IN_MAIN,
		//! 特殊射撃入力
		IN_SPECIALSHOT,
		//! 格闘入力
		IN_KAKUTOU,
		IN_TURN_LEFT,
		IN_TURN_RIGHT,
		IN_TURN_UP,
		IN_TURN_DOWN,
		//! 左ステックを入力しているかどうか
		IN_LEFT_STICK,
		//! 最大数
		IN_MAX
	};
private:

	//unsigned char  //8以下
	//unsigned short //16以下
	//unsigned long  //32以下
	//! ゲームパッドが繋がれているかどうか
	typedef unsigned long KeyStorageType;


	//! 入力保存変数
	KeyStorageType m_KeepState[InputManagerData::k_InputKeepFrame] = {};
	//! 入力保存用インクリメント変数
	int m_KeepIncrement = 0;

	//! 現在の入力状態
	KeyStorageType m_CurrentMobileSuitState = 0;
	//! 前回の入力状態
	KeyStorageType m_PrevState = 0;
	//! トリガー
	KeyStorageType m_Trigger = 0;
	//! リリース
	KeyStorageType m_Release = 0;

	//! ゲームパッドが繋がれているかどうか
	DWORD m_Is_GamePad_Use = 0;

	//! ゲームパッド入力情報
	XINPUT_STATE m_State = {};

	//! キーボードの入力格納変数1
	BYTE m_InputByteBuffer[256];
private:
	friend class Singleton<InputManager>;

	InputManager() = default;
	~InputManager() = default;

public:
	/**
	* @brief 初期化処理
	*/
	void Init();

	/**
	* @brief 終了処理処理
	*/
	void Uninit();

	/**
	* @brief 更新処理
	*/
	void Update();

	/**
	* @brief ゲームパッドを使っているかどうか
	* @return bool ゲームパッドを使っていたらtrue
	*/
	bool GetIsUseGamePad();

	/**
	* @brief トリガー取得
	* @param[in] InputName(inputName) 入力名
	* @return bool トリガー状態
	*/
	bool GetIsTrigger(InputName inputName);

	/**
	* @brief 入力状態取得
	* @param[in] InputName(inputName) 入力名
	* @return bool 入力されているかどうか
	*/
	bool GetIsPress(InputName inputName);

	/**
	* @brief リリース取得
	* @param[in] InputName(inputName) 入力名
	* @return bool 離されたかどうか
	*/
	bool GetIsRelease(InputName inputName);

	/**
	* @brief 保存キーの取得
	* @param[in] InputName(inputName) 入力名
	* @param[in] int(prevFrame) 入力名
	* @return bool 入力されているかどうか
	*/
	bool GetIsKeepState(InputName inputName, int prevFrame);


	/**
	* @brief 左ステックのX座標を取得
	* @return SHORT 左ステックのX座標の値
	*/
	SHORT GetLeftStickX();

	/**
	* @brief 左ステックのY座標を取得
	* @return SHORT 左ステックのY座標の値
	*/
	SHORT GetLeftStickY();

	/**
	* @brief 右ステックのX座標を取得
	* @return SHORT 右ステックのX座標の値
	*/
	SHORT GetRightStickX();

	/**
	* @brief 右ステックのY座標を取得
	* @return SHORT 右ステックのY座標の値
	*/
	SHORT GetRightStickY();

	/**
	* @brief 左ステックのベクトルを取得
	* @return Conv_XM::Vector2 ベクトル
	*/
	Conv_XM::Vector2f GetLeftStickeVector();

	/**
	* @brief 右ステックのベクトルを取得
	* @return Conv_XM::Vector2 ベクトル
	*/
	Conv_XM::Vector2f GetRightStickeVector();

	/**
	* @brief 左ステックに入力があるかどうか
	* @return bool 入力されているかどうか
	*/
	bool GetLeftStickIsInput();

	/**
	* @brief 右ステックに入力があるかどうか
	* @return bool 入力されているかどうか
	*/
	bool GetRightStickIsInput();
};