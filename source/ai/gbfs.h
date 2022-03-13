#pragma once

#include"gbgfsscratch.h"

using GBFSMap = std::unordered_map<const WeightedGraphNode*, GBFSScratch>;

using GBFSRouteMap = std::vector<WeightedGraphNode*>;

bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start, const WeightedGraphNode* goal, GBFSMap& outMap);

void MakeGBFSRoute(WeightedGraphNode* start, WeightedGraphNode* goal, GBFSMap& map, GBFSRouteMap& outRoute);

int FindGBFSNearNode(const WeightedGraph& graph, const Conv_XM::Vector3f position);

void testHeuristic();
