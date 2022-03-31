/**
 * @file astar.h
 * @brief A*‚ÌŽÀ‘•
 */

#pragma once

#include"weighted_graph.h"
#include"astar_scratch.h"

using AStarMap = std::unordered_map<WeightedGraphNode*, AStarScratch>;

using AStarRouteMap = std::vector<WeightedGraphNode*>;

bool AStar(WeightedGraph& g, WeightedGraphNode* start, WeightedGraphNode* goal, AStarMap& outMap);

void MakeAStarRoute(WeightedGraphNode* start, WeightedGraphNode* goal, AStarMap& map, AStarRouteMap& outRoute);

int FindAStarNearNode(const WeightedGraph& graph, const Conv_XM::Vector3f position);