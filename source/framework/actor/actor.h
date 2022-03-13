/**
 * @file actor.h
 * @brief ゲームオブジェクトの振る舞いを扱う
 */

#pragma once

#include"../component/component.h"

 /**
 * @brief ゲームオブジェクトの振る舞いをするクラス
 */
class Actor
{
public:
	enum ActorType
	{
		ACTORTYPE_NONE,
		ACTORTYPE_PLAYER,
		ACTORTYPE_ENEMY,
		ACTORTYPE_ALLIES,
		ACTORTYPE_PLAYER_BULLET,
		ACTORTYPE_ENEMY_BULLET,
		ACTORTYPE_BEAMRIFLE,
		ACTORTYPE_CANNON,
		ACTORTYPE_BEAMSABER_NKAKU001,
		ACTORTYPE_BEAMSABER_NKAKU002,
		ACTORTYPE_BEAMSABER_NKAKU003,
		ACTORTYPE_MAX,
	};

protected:
	//! アクターのタイプ
	ActorType m_ActorType;

	//! アクティブかどうか
	bool m_Active;

	// 座標変換
	//! ワールド行列
	Conv_XM::Matrix4x4f m_WorldMatrix;
	//! ポジション
	Conv_XM::Vector3f m_Position;
	//! スケール
	Conv_XM::Vector3f m_Scale;
	//! クォータニオン
	Conv_XM::Vector4f m_Quaternion;
	//! 座標変換が行われるかどうかのフラグ
	bool m_RecomputeTransform;

	//! アクターが持つコンポーネント
	std::vector<class Component*> m_Components;
public:
	Actor();
	virtual ~Actor();

	/**
	* @brief 初期化処理
	* @details オブジェクトマネージャーから呼び出される読み込み処理関数(オーバーライド不可)
	*/
	void LoadData();

	/**
	* @brief 初期化処理
	* @details オブジェクトマネージャーから呼び出される初期化処理関数(オーバーライド不可)
	*/
	void Init();

	/**
	* @brief 終了処理
	* @details オブジェクトマネージャーから呼び出される終了処理関数(オーバーライド不可)
	*/
	void Uninit();

	/**
	* @brief 入力処理
	* @details オブジェクトマネージャーから呼び出される入力処理関数(オーバーライド不可)
	*/
	void ProcessInput();

	/**
	* @brief 更新処理
	* @details オブジェクトマネージャーから呼び出される更新処理関数(オーバーライド不可)
	*/
	void Update();

	/**
	* @brief ワールド行列更新関数
	* @details ポジション、スケール、クォータニオンからワールド行列を作成する
	*/
	void ComputeWorldTransform();

	/**
	* @brief コンポーネント生成関数
	* @return T 作成したコンポーネント
	*/
	template<typename T>
	T* CreateComponent()
	{
		// コンポーネントを生成
		T* component = new T(this);
		// nullチェック
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::CreateComponent componentがnullptr");
			return nullptr;
		}
		// コンポーネントを追加
		AddComponent(component);

		return component;
	}

	/**
	* @brief コンポーネント取得関数
	* @return T テンプレートで指定されたコンポーネントのポインタ
	* @details 指定したコンポーネントを一つだけ持っている場合のみ正常作動
	*/
	template <typename T>
	T*  GetComponent() const
	{
		for (auto comp : m_Components)
		{
			if (typeid(*comp) == typeid(T))
			{
				T* t = dynamic_cast<T*>(comp);
				return  t;
			}
		}
		return nullptr;
	}

	/**
	* @brief 当たり判定処理
	* @details 衝突時に呼び出される関数
	*/
	virtual void CollisionDetected(class Actor* coleObj);

	/**
	* @brief 角度入力
	* @param[in] float(x) 角度X
	* @param[in] float(y) 角度Y
	* @param[in] float(z) 角度Z
	* @details degreeで入力
	*/
	void SetEuler(float x, float y, float z);

	/**
	* @brief 角度入力
	* @param[in] const Conv_XM::Vector3f&(euler) 角度
	* @details degreeで入力
	*/
	void SetEuler(const Conv_XM::Vector3f& euler);

	// セッター,ゲッター

	void SetActorType(ActorType type) { m_ActorType = type; }
	inline ActorType GetActorType() const { return m_ActorType; }

	virtual void SetActive(bool flag) { m_Active = flag; }
	inline bool GetActive() const { return m_Active; }

	inline const Conv_XM::Matrix4x4f& GetWorldMatrix() const { return m_WorldMatrix; }

	inline void SetPosition(const Conv_XM::Vector3f& position)
	{
		m_RecomputeTransform = true;
		m_Position = position; 
	}
	inline const Conv_XM::Vector3f& GetPosition() const { return m_Position; }

	inline void SetScale(const Conv_XM::Vector3f& scale) 
	{
		m_RecomputeTransform = true;
		m_Scale = scale; 
	}
	inline const Conv_XM::Vector3f& GetScale() const { return m_Scale; }

	inline void SetQuaternion(const Conv_XM::Vector4f& quaternion) 
	{
		m_RecomputeTransform = true;
		m_Quaternion = quaternion; 
	}
	inline const Conv_XM::Vector4f& GetQuaternion() const { return m_Quaternion; }

	Conv_XM::Vector3f GetForward() const;
	Conv_XM::Vector3f GetRight() const;
	Conv_XM::Vector3f GetUp() const;
private:

	/**
	* @brief アクター独自の読み込み処理
	*/
	virtual void LoadActor();

	/**
	* @brief アクター独自の初期化処理
	*/
	virtual void InitActor();

	/**
	* @brief アクター独自の終了処理
	*/
	virtual void UninitActor();

	/**
	* @brief アクター独自の入力処理
	*/
	virtual void ProcessInputActor();

	/**
	* @brief アクター独自の更新処理
	*/
	virtual void UpdateActor();

	/**
	* @brief アクターが持つ全コンポーネントを読み込み処理(オーバーライド不可)
	*/
	void LoadComponents();

	/**
	* @brief アクターが持つ全コンポーネントを初期処理(オーバーライド不可)
	*/
	void InitComponents();

	/**
	* @brief アクターが持つ全コンポーネントを終了処理(オーバーライド不可)
	*/
	void UninitComponents();

	/**
	* @brief アクターが持つ全コンポーネントを入力処理(オーバーライド不可)
	*/
	void ProcessInputComponents();

	/**
	* @brief アクターが持つ全コンポーネントを更新処理(オーバーライド不可)
	*/
	void UpdateComponents();


	/**
	* @brief コンポーネントの登録
	* @param[in] class Component*(component) コンポーネント
	*/
	void AddComponent(Component * component);
};