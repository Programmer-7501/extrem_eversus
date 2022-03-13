
#include"../../../system/main.h"
#include "sideseven_object.h"
#include"../../component/mesh_component/instancing_model_component.h"
#include"../../../directx/resource/model_manager.h"
#include"../../component/collision_component/aabb_collision_component.h"
#include"../../component/mesh_component/debug/aabb_debug_view_component.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_collision_manager.h"

namespace SideSevenObjectData
{
	//! 道路
	static const char* k_RoadModelFileName = "asset/model/road/road.bin";

	//! コンクリート
	static const char* k_ConcreteModelFileName = "asset/model/concrete/concrete.bin";

	//! 建物
	static const char* k_BuildingModelFileName = "asset/model/building/building.bin";

	//! 道オブジェクト数
	static constexpr int k_RoadNum = 102 + 43 + 28 + 25 + 25 + 29 + 29;

	//! コンクリートオブジェクト数
	static constexpr int k_ConcreteNum = 204 + 22;

	//! 建物オブジェクト数
	static constexpr int k_BuildingNum = 5;

	//! 道オブジェクトスケール
	static Conv_XM::Vector3f k_RoadScale = Conv_XM::Vector3f(1.0f, 1.0f, 0.06f);

	//! 道オブジェクト横姿勢
	static Conv_XM::Vector4f k_RoadBesideQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	//! 道オブジェクト横姿勢
	static Conv_XM::Vector4f k_RoadVerticalQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(90.0f), 0.0f);

	//! 道オブジェクト姿勢
	static Conv_XM::Vector4f k_RoadQuaternion[k_RoadNum] = {
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,

		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,

		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,

		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,

		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,
		k_RoadBesideQuaternion,

		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,

		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
		k_RoadVerticalQuaternion,
	};


	//! 道オブジェクト数
	static Conv_XM::Vector3f k_RoadTransform[k_RoadNum] = {
		{-12.0f,0.0f,17.0f},
		{-10.0f,0.0f,17.0f},
		{-8.0f,0.0f,17.0f},
		{-6.0f,0.0f,17.0f},
		{-4.0f,0.0f,17.0f},
		{-2.0f,0.0f,17.0f},
		{ 0.0f,0.0f,17.0f},
		{2.0f,0.0f,17.0f},
		{4.0f,0.0f,17.0f},
		{6.0f,0.0f,17.0f},
		{8.0f,0.0f,17.0f},
		{10.0f,0.0f,17.0f},
		{12.0f,0.0f,17.0f},
		{14.0f,0.0f,17.0f},
		{16.0f,0.0f,17.0f},
		{18.0f,0.0f,17.0f},
		{20.0f,0.0f,17.0f},
		{22.0f,0.0f,17.0f},
		{24.0f,0.0f,17.0f},
		{26.0f,0.0f,17.0f},
		{28.0f,0.0f,17.0f},
		{30.0f,0.0f,17.0f},
		{32.0f,0.0f,17.0f},
		{34.0f,0.0f,17.0f},
		{36.0f,0.0f,17.0f},
		{38.0f,0.0f,17.0f},
		{40.0f,0.0f,17.0f},
		{42.0f,0.0f,17.0f},
		{44.0f,0.0f,17.0f},
		{46.0f,0.0f,17.0f},
		{48.0f,0.0f,17.0f},
		{50.0f,0.0f,17.0f},
		{52.0f,0.0f,17.0f},
		{54.0f,0.0f,17.0f},
		{56.0f,0.0f,17.0f},
		{58.0f,0.0f,17.0f},
		{60.0f,0.0f,17.0f},
		{62.0f,0.0f,17.0f},
		{64.0f,0.0f,17.0f},
		{66.0f,0.0f,17.0f},
		{68.0f,0.0f,17.0f},
		{70.0f,0.0f,17.0f},
		{72.0f,0.0f,17.0f},
		{74.0f,0.0f,17.0f},
		{76.0f,0.0f,17.0f},
		{78.0f,0.0f,17.0f},
		{80.0f,0.0f,17.0f},
		{82.0f,0.0f,17.0f},
		{84.0f,0.0f,17.0f},
		{86.0f,0.0f,17.0f},
		{84.0f,0.0f,17.0f},

		{-12.0f,0.0f,11.0f},
		{-10.0f,0.0f,11.0f},
		{-8.0f,0.0f,11.0f},
		{-6.0f,0.0f,11.0f},
		{-4.0f,0.0f,11.0f},
		{-2.0f,0.0f,11.0f},
		{ 0.0f,0.0f,11.0f},
		{2.0f,0.0f,11.0f},
		{4.0f,0.0f,11.0f},
		{6.0f,0.0f,11.0f},
		{8.0f,0.0f,11.0f},
		{10.0f,0.0f,11.0f},
		{12.0f,0.0f,11.0f},
		{14.0f,0.0f,11.0f},
		{16.0f,0.0f,11.0f},
		{18.0f,0.0f,11.0f},
		{20.0f,0.0f,11.0f},
		{22.0f,0.0f,11.0f},
		{24.0f,0.0f,11.0f},
		{26.0f,0.0f,11.0f},
		{28.0f,0.0f,11.0f},
		{30.0f,0.0f,11.0f},
		{32.0f,0.0f,11.0f},
		{34.0f,0.0f,11.0f},
		{36.0f,0.0f,11.0f},
		{38.0f,0.0f,11.0f},
		{40.0f,0.0f,11.0f},
		{42.0f,0.0f,11.0f},
		{44.0f,0.0f,11.0f},
		{46.0f,0.0f,11.0f},
		{48.0f,0.0f,11.0f},
		{50.0f,0.0f,11.0f},
		{52.0f,0.0f,11.0f},
		{54.0f,0.0f,11.0f},
		{56.0f,0.0f,11.0f},
		{58.0f,0.0f,11.0f},
		{60.0f,0.0f,11.0f},
		{62.0f,0.0f,11.0f},
		{64.0f,0.0f,11.0f},
		{66.0f,0.0f,11.0f},
		{68.0f,0.0f,11.0f},
		{70.0f,0.0f,11.0f},
		{72.0f,0.0f,11.0f},
		{74.0f,0.0f,11.0f},
		{76.0f,0.0f,11.0f},
		{78.0f,0.0f,11.0f},
		{80.0f,0.0f,11.0f},
		{82.0f,0.0f,11.0f},
		{84.0f,0.0f,11.0f},
		{86.0f,0.0f,11.0f},
		{84.0f,0.0f,11.0f},

		{ 54.0f,0.0f,15.0f },
		{ 54.0f,0.0f,13.0f },
		{ 54.0f,0.0f,9.0f},
		{ 54.0f,0.0f,7.0f},
		{ 54.0f,0.0f,5.0f },
		{ 54.0f,0.0f,3.0f },
		{ 54.0f,0.0f,1.0f },
		{ 54.0f,0.0f,-1.0f },
		{ 54.0f,0.0f,-3.0f },
		{ 54.0f,0.0f,-5.0f },
		{ 54.0f,0.0f,-7.0f },
		{ 54.0f,0.0f,-9.0f },
		{ 54.0f,0.0f,-11.0f },
		{ 54.0f,0.0f,-13.0f },
		{ 54.0f,0.0f,-15.0f },
		{ 54.0f,0.0f,-17.0f },
		{ 54.0f,0.0f,-19.0f },
		{ 54.0f,0.0f,-21.0f },
		{ 54.0f,0.0f,-23.0f },
		{ 54.0f,0.0f,-25.0f },
		{ 54.0f,0.0f,-27.0f },
		{ 54.0f,0.0f,-29.0f },
		{ 54.0f,0.0f,-31.0f },
		{ 54.0f,0.0f,-33.0f },
		{ 54.0f,0.0f,-35.0f },
		{ 54.0f,0.0f,-37.0f },
		{ 54.0f,0.0f,-39.0f },
		{ 54.0f,0.0f,-41.0f },
		{ 54.0f,0.0f,-43.0f },
		{ 54.0f,0.0f,-45.0f },
		{ 54.0f,0.0f,-47.0f },
		{ 54.0f,0.0f,-49.0f },
		{ 54.0f,0.0f,-51.0f },
		{ 54.0f,0.0f,-53.0f },
		{ 54.0f,0.0f,-55.0f },
		{ 54.0f,0.0f,-57.0f },
		{ 54.0f,0.0f,-59.0f },
		{ 54.0f,0.0f,-61.0f },
		{ 54.0f,0.0f,-63.0f },
		{ 54.0f,0.0f,-65.0f },
		{ 54.0f,0.0f,-67.0f },
		{ 54.0f,0.0f,-69.0f },
		{ 54.0f,0.0f,-71.0f },

		{ 54.0f,0.0f,-73.0f },
		{ 52.0f,0.0f,-73.0f },
		{ 50.0f,0.0f,-73.0f },
		{ 48.0f,0.0f,-73.0f },
		{ 46.0f,0.0f,-73.0f },
		{ 44.0f,0.0f,-73.0f },
		{ 42.0f,0.0f,-73.0f },
		{ 40.0f,0.0f,-73.0f },
		{ 38.0f,0.0f,-73.0f },
		{ 36.0f,0.0f,-73.0f },
		{ 34.0f,0.0f,-73.0f },
		{ 32.0f,0.0f,-73.0f },
		{ 30.0f,0.0f,-73.0f },
		{ 28.0f,0.0f,-73.0f },
		{ 56.0f,0.0f,-73.0f },
		{ 58.0f,0.0f,-73.0f },
		{ 60.0f,0.0f,-73.0f },
		{ 62.0f,0.0f,-73.0f },
		{ 64.0f,0.0f,-73.0f },
		{ 66.0f,0.0f,-73.0f },
		{ 68.0f,0.0f,-73.0f },
		{ 70.0f,0.0f,-73.0f },
		{ 72.0f,0.0f,-73.0f },
		{ 74.0f,0.0f,-73.0f },
		{ 76.0f,0.0f,-73.0f },
		{ 78.0f,0.0f,-73.0f },
		{ 80.0f,0.0f,-73.0f },
		{ 82.0f,0.0f,-73.0f },

		{ 52.0f,0.0f,-44.0f },
		{ 50.0f,0.0f,-44.0f },
		{ 48.0f,0.0f,-44.0f },
		{ 46.0f,0.0f,-44.0f },
		{ 44.0f,0.0f,-44.0f },
		{ 42.0f,0.0f,-44.0f },
		{ 40.0f,0.0f,-44.0f },
		{ 38.0f,0.0f,-44.0f },
		{ 36.0f,0.0f,-44.0f },
		{ 34.0f,0.0f,-44.0f },
		{ 32.0f,0.0f,-44.0f },
		{ 30.0f,0.0f,-44.0f },
		{ 56.0f,0.0f,-44.0f },
		{ 58.0f,0.0f,-44.0f },
		{ 60.0f,0.0f,-44.0f },
		{ 62.0f,0.0f,-44.0f },
		{ 64.0f,0.0f,-44.0f },
		{ 66.0f,0.0f,-44.0f },
		{ 68.0f,0.0f,-44.0f },
		{ 70.0f,0.0f,-44.0f },
		{ 72.0f,0.0f,-44.0f },
		{ 74.0f,0.0f,-44.0f },
		{ 76.0f,0.0f,-44.0f },
		{ 78.0f,0.0f,-44.0f },
		{ 80.0f,0.0f,-44.0f },

		{ 52.0f,0.0f,-15.0f },
		{ 50.0f,0.0f,-15.0f },
		{ 48.0f,0.0f,-15.0f },
		{ 46.0f,0.0f,-15.0f },
		{ 44.0f,0.0f,-15.0f },
		{ 42.0f,0.0f,-15.0f },
		{ 40.0f,0.0f,-15.0f },
		{ 38.0f,0.0f,-15.0f },
		{ 36.0f,0.0f,-15.0f },
		{ 34.0f,0.0f,-15.0f },
		{ 32.0f,0.0f,-15.0f },
		{ 30.0f,0.0f,-15.0f },
		{ 56.0f,0.0f,-15.0f },
		{ 58.0f,0.0f,-15.0f },
		{ 60.0f,0.0f,-15.0f },
		{ 62.0f,0.0f,-15.0f },
		{ 64.0f,0.0f,-15.0f },
		{ 66.0f,0.0f,-15.0f },
		{ 68.0f,0.0f,-15.0f },
		{ 70.0f,0.0f,-15.0f },
		{ 72.0f,0.0f,-15.0f },
		{ 74.0f,0.0f,-15.0f },
		{ 76.0f,0.0f,-15.0f },
		{ 78.0f,0.0f,-15.0f },
		{ 80.0f,0.0f,-15.0f },

		{ 82.0f,0.0f,-15.0f },
		{ 82.0f,0.0f,-17.0f },
		{ 82.0f,0.0f,-19.0f },
		{ 82.0f,0.0f,-21.0f },
		{ 82.0f,0.0f,-23.0f },
		{ 82.0f,0.0f,-25.0f },
		{ 82.0f,0.0f,-27.0f },
		{ 82.0f,0.0f,-29.0f },
		{ 82.0f,0.0f,-31.0f },
		{ 82.0f,0.0f,-33.0f },
		{ 82.0f,0.0f,-35.0f },
		{ 82.0f,0.0f,-37.0f },
		{ 82.0f,0.0f,-39.0f },
		{ 82.0f,0.0f,-41.0f },
		{ 82.0f,0.0f,-43.0f },
		{ 82.0f,0.0f,-45.0f },
		{ 82.0f,0.0f,-47.0f },
		{ 82.0f,0.0f,-49.0f },
		{ 82.0f,0.0f,-51.0f },
		{ 82.0f,0.0f,-53.0f },
		{ 82.0f,0.0f,-55.0f },
		{ 82.0f,0.0f,-57.0f },
		{ 82.0f,0.0f,-59.0f },
		{ 82.0f,0.0f,-61.0f },
		{ 82.0f,0.0f,-63.0f },
		{ 82.0f,0.0f,-65.0f },
		{ 82.0f,0.0f,-67.0f },
		{ 82.0f,0.0f,-69.0f },
		{ 82.0f,0.0f,-71.0f },

		{ 28.0f,0.0f,-15.0f },
		{ 28.0f,0.0f,-17.0f },
		{ 28.0f,0.0f,-19.0f },
		{ 28.0f,0.0f,-21.0f },
		{ 28.0f,0.0f,-23.0f },
		{ 28.0f,0.0f,-25.0f },
		{ 28.0f,0.0f,-27.0f },
		{ 28.0f,0.0f,-29.0f },
		{ 28.0f,0.0f,-31.0f },
		{ 28.0f,0.0f,-33.0f },
		{ 28.0f,0.0f,-35.0f },
		{ 28.0f,0.0f,-37.0f },
		{ 28.0f,0.0f,-39.0f },
		{ 28.0f,0.0f,-41.0f },
		{ 28.0f,0.0f,-43.0f },
		{ 28.0f,0.0f,-45.0f },
		{ 28.0f,0.0f,-47.0f },
		{ 28.0f,0.0f,-49.0f },
		{ 28.0f,0.0f,-51.0f },
		{ 28.0f,0.0f,-53.0f },
		{ 28.0f,0.0f,-55.0f },
		{ 28.0f,0.0f,-57.0f },
		{ 28.0f,0.0f,-59.0f },
		{ 28.0f,0.0f,-61.0f },
		{ 28.0f,0.0f,-63.0f },
		{ 28.0f,0.0f,-65.0f },
		{ 28.0f,0.0f,-67.0f },
		{ 28.0f,0.0f,-69.0f },
		{ 28.0f,0.0f,-71.0f },
	};

	//! コンクリートオブジェクトスケール
	static Conv_XM::Vector3f k_ConcreteScale = Conv_XM::Vector3f(1.0f, 1.0f, 0.06f);

	//! コンクリートオブジェクト基本姿勢
	static Conv_XM::Vector4f k_ConcreteQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	//! コンクリートオブジェクト数
	static Conv_XM::Vector3f k_ConcreteTransform[k_ConcreteNum] = {
		{-12.0f,0.0f,19.0f},
		{-10.0f,0.0f,19.0f},
		{-8.0f,0.0f,19.0f},
		{-6.0f,0.0f,19.0f},
		{-4.0f,0.0f,19.0f},
		{-2.0f,0.0f,19.0f},
		{ 0.0f,0.0f,19.0f},
		{2.0f,0.0f,19.0f},
		{4.0f,0.0f,19.0f},
		{6.0f,0.0f,19.0f},
		{8.0f,0.0f,19.0f},
		{10.0f,0.0f,19.0f},
		{12.0f,0.0f,19.0f},
		{14.0f,0.0f,19.0f},
		{16.0f,0.0f,19.0f},
		{18.0f,0.0f,19.0f},
		{20.0f,0.0f,19.0f},
		{22.0f,0.0f,19.0f},
		{24.0f,0.0f,19.0f},
		{26.0f,0.0f,19.0f},
		{28.0f,0.0f,19.0f},
		{30.0f,0.0f,19.0f},
		{32.0f,0.0f,19.0f},
		{34.0f,0.0f,19.0f},
		{36.0f,0.0f,19.0f},
		{38.0f,0.0f,19.0f},
		{40.0f,0.0f,19.0f},
		{42.0f,0.0f,19.0f},
		{44.0f,0.0f,19.0f},
		{46.0f,0.0f,19.0f},
		{48.0f,0.0f,19.0f},
		{50.0f,0.0f,19.0f},
		{52.0f,0.0f,19.0f},
		{54.0f,0.0f,19.0f},
		{56.0f,0.0f,19.0f},
		{58.0f,0.0f,19.0f},
		{60.0f,0.0f,19.0f},
		{62.0f,0.0f,19.0f},
		{64.0f,0.0f,19.0f},
		{66.0f,0.0f,19.0f},
		{68.0f,0.0f,19.0f},
		{70.0f,0.0f,19.0f},
		{72.0f,0.0f,19.0f},
		{74.0f,0.0f,19.0f},
		{76.0f,0.0f,19.0f},
		{78.0f,0.0f,19.0f},
		{80.0f,0.0f,19.0f},
		{82.0f,0.0f,19.0f},
		{84.0f,0.0f,19.0f},
		{86.0f,0.0f,19.0f},
		{84.0f,0.0f,19.0f},

		{-12.0f,0.0f,15.0f},
		{-10.0f,0.0f,15.0f},
		{-8.0f,0.0f,15.0f},
		{-6.0f,0.0f,15.0f},
		{-4.0f,0.0f,15.0f},
		{-2.0f,0.0f,15.0f},
		{ 0.0f,0.0f,15.0f},
		{2.0f,0.0f,15.0f},
		{4.0f,0.0f,15.0f},
		{6.0f,0.0f,15.0f},
		{8.0f,0.0f,15.0f},
		{10.0f,0.0f,15.0f},
		{12.0f,0.0f,15.0f},
		{14.0f,0.0f,15.0f},
		{16.0f,0.0f,15.0f},
		{18.0f,0.0f,15.0f},
		{20.0f,0.0f,15.0f},
		{22.0f,0.0f,15.0f},
		{24.0f,0.0f,15.0f},
		{26.0f,0.0f,15.0f},
		{28.0f,0.0f,15.0f},
		{30.0f,0.0f,15.0f},
		{32.0f,0.0f,15.0f},
		{34.0f,0.0f,15.0f},
		{36.0f,0.0f,15.0f},
		{38.0f,0.0f,15.0f},
		{40.0f,0.0f,15.0f},
		{42.0f,0.0f,15.0f},
		{44.0f,0.0f,15.0f},
		{46.0f,0.0f,15.0f},
		{48.0f,0.0f,15.0f},
		{50.0f,0.0f,15.0f},
		{52.0f,0.0f,15.0f},
		{0.0f,0.0f,15.0f},
		{56.0f,0.0f,15.0f},
		{58.0f,0.0f,15.0f},
		{60.0f,0.0f,15.0f},
		{62.0f,0.0f,15.0f},
		{64.0f,0.0f,15.0f},
		{66.0f,0.0f,15.0f},
		{68.0f,0.0f,15.0f},
		{70.0f,0.0f,15.0f},
		{72.0f,0.0f,15.0f},
		{74.0f,0.0f,15.0f},
		{76.0f,0.0f,15.0f},
		{78.0f,0.0f,15.0f},
		{80.0f,0.0f,15.0f},
		{82.0f,0.0f,15.0f},
		{84.0f,0.0f,15.0f},
		{86.0f,0.0f,15.0f},
		{84.0f,0.0f,15.0f},

		{-12.0f,0.0f,13.0f},
		{-10.0f,0.0f,13.0f},
		{-8.0f,0.0f,13.0f},
		{-6.0f,0.0f,13.0f},
		{-4.0f,0.0f,13.0f},
		{-2.0f,0.0f,13.0f},
		{ 0.0f,0.0f,13.0f},
		{2.0f,0.0f,13.0f},
		{4.0f,0.0f,13.0f},
		{6.0f,0.0f,13.0f},
		{8.0f,0.0f,13.0f},
		{10.0f,0.0f,13.0f},
		{12.0f,0.0f,13.0f},
		{14.0f,0.0f,13.0f},
		{16.0f,0.0f,13.0f},
		{18.0f,0.0f,13.0f},
		{20.0f,0.0f,13.0f},
		{22.0f,0.0f,13.0f},
		{24.0f,0.0f,13.0f},
		{26.0f,0.0f,13.0f},
		{28.0f,0.0f,13.0f},
		{30.0f,0.0f,13.0f},
		{32.0f,0.0f,13.0f},
		{34.0f,0.0f,13.0f},
		{36.0f,0.0f,13.0f},
		{38.0f,0.0f,13.0f},
		{40.0f,0.0f,13.0f},
		{42.0f,0.0f,13.0f},
		{44.0f,0.0f,13.0f},
		{46.0f,0.0f,13.0f},
		{48.0f,0.0f,13.0f},
		{50.0f,0.0f,13.0f},
		{52.0f,0.0f,13.0f},
		{0.0f,0.0f,13.0f},
		{56.0f,0.0f,13.0f},
		{58.0f,0.0f,13.0f},
		{60.0f,0.0f,13.0f},
		{62.0f,0.0f,13.0f},
		{64.0f,0.0f,13.0f},
		{66.0f,0.0f,13.0f},
		{68.0f,0.0f,13.0f},
		{70.0f,0.0f,13.0f},
		{72.0f,0.0f,13.0f},
		{74.0f,0.0f,13.0f},
		{76.0f,0.0f,13.0f},
		{78.0f,0.0f,13.0f},
		{80.0f,0.0f,13.0f},
		{82.0f,0.0f,13.0f},
		{84.0f,0.0f,13.0f},
		{86.0f,0.0f,13.0f},
		{84.0f,0.0f,13.0f},

		{-12.0f,0.0f,9.0f},
		{-10.0f,0.0f,9.0f},
		{-8.0f,0.0f,9.0f},
		{-6.0f,0.0f,9.0f},
		{-4.0f,0.0f,9.0f},
		{-2.0f,0.0f,9.0f},
		{ 0.0f,0.0f,9.0f},
		{2.0f,0.0f,9.0f},
		{4.0f,0.0f,9.0f},
		{6.0f,0.0f,9.0f},
		{8.0f,0.0f,9.0f},
		{10.0f,0.0f,9.0f},
		{12.0f,0.0f,9.0f},
		{14.0f,0.0f,9.0f},
		{16.0f,0.0f,9.0f},
		{18.0f,0.0f,9.0f},
		{20.0f,0.0f,9.0f},
		{22.0f,0.0f,9.0f},
		{24.0f,0.0f,9.0f},
		{26.0f,0.0f,9.0f},
		{28.0f,0.0f,9.0f},
		{30.0f,0.0f,9.0f},
		{32.0f,0.0f,9.0f},
		{34.0f,0.0f,9.0f},
		{36.0f,0.0f,9.0f},
		{38.0f,0.0f,9.0f},
		{40.0f,0.0f,9.0f},
		{42.0f,0.0f,9.0f},
		{44.0f,0.0f,9.0f},
		{46.0f,0.0f,9.0f},
		{48.0f,0.0f,9.0f},
		{50.0f,0.0f,9.0f},
		{52.0f,0.0f,9.0f},
		{0.0f,0.0f,9.0f},
		{56.0f,0.0f,9.0f},
		{58.0f,0.0f,9.0f},
		{60.0f,0.0f,9.0f},
		{62.0f,0.0f,9.0f},
		{64.0f,0.0f,9.0f},
		{66.0f,0.0f,9.0f},
		{68.0f,0.0f,9.0f},
		{70.0f,0.0f,9.0f},
		{72.0f,0.0f,9.0f},
		{74.0f,0.0f,9.0f},
		{76.0f,0.0f,9.0f},
		{78.0f,0.0f,9.0f},
		{80.0f,0.0f,9.0f},
		{82.0f,0.0f,9.0f},
		{84.0f,0.0f,9.0f},
		{86.0f,0.0f,9.0f},
		{84.0f,0.0f,9.0f},


		{52.0f,0.0f, 7.0f},
		{52.0f,0.0f, 5.0f},
		{52.0f,0.0f, 3.0f},
		{52.0f,0.0f, 1.0f},
		{52.0f,0.0f, -1.0f},
		{52.0f,0.0f, -3.0f},
		{52.0f,0.0f, -5.0f},
		{52.0f,0.0f, -7.0f},
		{52.0f,0.0f, -9.0f},
		{52.0f,0.0f, -11.0f},
		{52.0f,0.0f, -13.0f},


		{56.0f,0.0f, 7.0f},
		{ 56.0f,0.0f, 5.0f },
		{ 56.0f,0.0f, 3.0f },
		{ 56.0f,0.0f, 1.0f },
		{ 56.0f,0.0f, -1.0f },
		{ 56.0f,0.0f, -3.0f },
		{ 56.0f,0.0f, -5.0f },
		{ 56.0f,0.0f, -7.0f },
		{ 56.0f,0.0f, -9.0f },
		{ 56.0f,0.0f, -11.0f },
		{ 56.0f,0.0f, -13.0f },
	};

	//! 建物オブジェクトスケール
	static Conv_XM::Vector3f k_BuildingScale = Conv_XM::Vector3f(6.0f, 4.0f, 4.0f);

	//! 建物オブジェクト基本姿勢
	static Conv_XM::Vector4f k_BuildingQuaternion[k_BuildingNum] = {
		{DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), 0.0f)},
		{DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), 0.0f)},
		{DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), 0.0f)},
		{DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), 0.0f)},
		{DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), 0.0f)},
	};

	//! 建物オブジェクト数
	static Conv_XM::Vector3f k_BuildingTransform[k_BuildingNum] = {
		{-16.0f,3.0f,14.0f},
		{70.0f,3.0f,-30.0f},
		{70.0f,3.0f,-60.0f},
		{40.0f,3.0f,-30.0f},
		{40.0f,3.0f,-60.0f},
	};
}

void SideSevenObject::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();
	modelManager.Load(SideSevenObjectData::k_RoadModelFileName);
	modelManager.Load(SideSevenObjectData::k_ConcreteModelFileName);
	modelManager.Load(SideSevenObjectData::k_BuildingModelFileName);
}

void SideSevenObject::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();
	modelManager.Unload(SideSevenObjectData::k_RoadModelFileName);
	modelManager.Unload(SideSevenObjectData::k_ConcreteModelFileName);
	modelManager.Unload(SideSevenObjectData::k_BuildingModelFileName);
}

SideSevenObject::SideSevenObject()
	: m_RoadObjectInstancingModelComponent(nullptr)
	, m_ConcreteObjectInstancingModelComponent(nullptr)
	, m_BuildingObjectInstancingModelComponent(nullptr)
{
}

SideSevenObject::~SideSevenObject()
{
}

void SideSevenObject::LoadActor()
{
	// 道オブジェクトインスタンシングモデルコンポーネント作成
	m_RoadObjectInstancingModelComponent = CreateComponent<InstancingModelComponent>();

	// コンクリートオブジェクトインスタンシングモデルコンポーネント作成
	m_ConcreteObjectInstancingModelComponent = CreateComponent<InstancingModelComponent>();

	// 建物オブジェクトインスタンシングモデルコンポーネント作成
	m_BuildingObjectInstancingModelComponent = CreateComponent<InstancingModelComponent>();

	for (int i = 0; i < SideSevenObjectData::k_BuildingNum; i++)
	{
		AABBCollisionComponent* aabbCollisionComponent = CreateComponent<AABBCollisionComponent>();
		m_BuildingObjectCollisionComponents.emplace_back(aabbCollisionComponent);
	}

	// OBBデバッグビュー作成
	CreateComponent<AABBDebugViewComponent>();
}

void SideSevenObject::InitActor()
{
	// インスタンシングマトリクス
	std::vector<Conv_XM::Matrix4x4f> roadMtx, concreteMtx, buildingMtx;

	// 木オブジェクトのインスタンシングデータ作成
	Conv_XM::Matrix4x4f mtxW, mtxS, mtxR, mtxT;

	CBufferData::Material material;
	material.Metaric = 0.0f;
	material.Smoothness = 0.0f;

	// 道オブジェクトのインスタンシングデータ作成
	mtxS = DirectX::XMMatrixScaling(SideSevenObjectData::k_RoadScale.x, SideSevenObjectData::k_RoadScale.y, SideSevenObjectData::k_RoadScale.z);

	for (int i = 0; i < SideSevenObjectData::k_RoadNum; i++)
	{
		mtxR = DirectX::XMMatrixRotationQuaternion(SideSevenObjectData::k_RoadQuaternion[i]);
		mtxT = DirectX::XMMatrixTranslation(SideSevenObjectData::k_RoadTransform[i].x, SideSevenObjectData::k_RoadTransform[i].y, SideSevenObjectData::k_RoadTransform[i].z);
		mtxW = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxR * (DirectX::XMMATRIX)mtxT;
		mtxW = DirectX::XMMatrixTranspose(mtxW);
		roadMtx.emplace_back(mtxW);
	}
	material.Metaric = 0.5f;
	material.Smoothness = 1.0f;

	m_RoadObjectInstancingModelComponent->SetInstanceMatrixData(roadMtx);
	m_RoadObjectInstancingModelComponent->SetMaterial(material);

	ModelManager& modelManager = ModelManager::GetInstance();
	Model* roadModel = modelManager.GetModel(SideSevenObjectData::k_RoadModelFileName);
	if (roadModel == nullptr)
	{
		Logger::GetInstance().SetLog("SideSevenObject::InitActor roadModelがnullptr");
		return;
	}
	m_RoadObjectInstancingModelComponent->SetModel(roadModel);

	// コンクリートオブジェクトのインスタンシングデータ作成
	mtxS = DirectX::XMMatrixScaling(SideSevenObjectData::k_ConcreteScale.x, SideSevenObjectData::k_ConcreteScale.y, SideSevenObjectData::k_ConcreteScale.z);
	mtxR = DirectX::XMMatrixRotationQuaternion(SideSevenObjectData::k_ConcreteQuaternion);

	for (int i = 0; i < SideSevenObjectData::k_ConcreteNum; i++)
	{
		mtxT = DirectX::XMMatrixTranslation(SideSevenObjectData::k_ConcreteTransform[i].x, SideSevenObjectData::k_ConcreteTransform[i].y, SideSevenObjectData::k_ConcreteTransform[i].z);
		mtxW = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxR * (DirectX::XMMATRIX)mtxT;
		mtxW = DirectX::XMMatrixTranspose(mtxW);
		concreteMtx.emplace_back(mtxW);
	}
	material.Metaric = 0.5f;
	material.Smoothness = 1.0f;
	m_ConcreteObjectInstancingModelComponent->SetInstanceMatrixData(concreteMtx);
	m_ConcreteObjectInstancingModelComponent->SetMaterial(material);

	Model* concreteModel = modelManager.GetModel(SideSevenObjectData::k_ConcreteModelFileName);
	if (roadModel == nullptr)
	{
		Logger::GetInstance().SetLog("SideSevenObject::InitActor concreteModelがnullptr");
		return;
	}
	m_ConcreteObjectInstancingModelComponent->SetModel(concreteModel);


	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	SceneCollisionManager* sceneCollisionManager = nullptr;
	if (currentScene)
	{
		sceneCollisionManager = currentScene->GetSceneCollisionManager();
	}

	// 建物オブジェクトのインスタンシングデータ作成
	mtxS = DirectX::XMMatrixScaling(SideSevenObjectData::k_BuildingScale.x, SideSevenObjectData::k_BuildingScale.y, SideSevenObjectData::k_BuildingScale.z);

	for (int i = 0; i < SideSevenObjectData::k_BuildingNum; i++)
	{
		mtxR = DirectX::XMMatrixRotationQuaternion(SideSevenObjectData::k_BuildingQuaternion[i]);
		mtxT = DirectX::XMMatrixTranslation(SideSevenObjectData::k_BuildingTransform[i].x, SideSevenObjectData::k_BuildingTransform[i].y, SideSevenObjectData::k_BuildingTransform[i].z);
		mtxW = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxR * (DirectX::XMMATRIX)mtxT;
		mtxW = DirectX::XMMatrixTranspose(mtxW);
		buildingMtx.emplace_back(mtxW);

		m_BuildingObjectCollisionComponents[i]->SetOffsetPosition(SideSevenObjectData::k_BuildingTransform[i]);
		m_BuildingObjectCollisionComponents[i]->SetOffsetScale(Conv_XM::Vector3f(8.0f, 8.0f, 12.0f));
		m_BuildingObjectCollisionComponents[i]->OnUpdateWorldTransform();

		if (sceneCollisionManager)
		{
			// シーンコリジョンマネージャーに設定する
			sceneCollisionManager->SetObstacleCollision(m_BuildingObjectCollisionComponents[i]);
		}

	}
	material.Metaric = 0.2f;
	material.Smoothness = 0.2f;
	m_BuildingObjectInstancingModelComponent->SetInstanceMatrixData(buildingMtx);
	m_BuildingObjectInstancingModelComponent->SetIsShadowInput(true);
	m_BuildingObjectInstancingModelComponent->SetMaterial(material);

	Model* buildingModel = modelManager.GetModel(SideSevenObjectData::k_BuildingModelFileName);
	if (buildingModel == nullptr)
	{
		Logger::GetInstance().SetLog("SideSevenObject::InitActor buildingModelがnullptr");
		return;
	}
	m_BuildingObjectInstancingModelComponent->SetModel(buildingModel);
}
