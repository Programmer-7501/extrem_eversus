/**
 * @file sideseven_object.h
 * @brief サイドセブンのオブジェクト
 */

#pragma once

#include"../actor.h"

 /**
 * @brief サイドセブンのオブジェクト配置クラス
 */
class SideSevenObject : public Actor
{
public:
	/**
	* @brief モデルの読み込み
	*/
	static void LoadModel();

	/**
	* @brief モデルの破棄
	*/
	static void UnloadModel();
private:
	//! 道オブジェクト
	class InstancingModelComponent* m_RoadObjectInstancingModelComponent;

	//! コンクリートオブジェクト
	class InstancingModelComponent* m_ConcreteObjectInstancingModelComponent;

	//! 建物オブジェクト
	class InstancingModelComponent* m_BuildingObjectInstancingModelComponent;

	//! 建物オブジェクトのコリジョンコンポーネント
	std::vector<class AABBCollisionComponent*> m_BuildingObjectCollisionComponents;
public:
	SideSevenObject();
	~SideSevenObject() override;

private:

	/**
	* @brief 読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief 初期化処理
	*/
	void InitActor() override;
};
