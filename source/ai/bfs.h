#pragma once

#include"graph.h"
#include<queue>
#include<map>

using NodeToParentMap = std::unordered_map<GraphNode*,GraphNode*>;

using BFSRouteMap = std::vector<GraphNode*>;

bool BFS(const Graph& graph, GraphNode* start, GraphNode* goal, NodeToParentMap& outMap);

void MakeBFSRoute(GraphNode* start,GraphNode* goal, NodeToParentMap& map, BFSRouteMap& outRoute);

int FindBFSNearNode(const Graph& graph, const Conv_XM::Vector3f position);