#include <iostream>
#include <vector>
#include "Node.h"
#include "AstarNode.h"
#include "BinaryHeap.h"
#include <cmath>
#include <float.h>
#include "BinaryHeap.cpp"
#include <algorithm>
using namespace std;

static int mapWidth;
static int mapHight;
static vector<AstarNode> initVec;
static BinaryHeap<AstarNode> openNodesList(initVec);
static vector<AstarNode> closeNodesList;
static vector<Node> pathNodesList;
vector<vector<Node>> nodeMap;
static shared_ptr<Node> starNode;
static shared_ptr<Node> endNode;
shared_ptr<AstarNode> curCheckNode;

extern "C" _declspec(dllexport) void CalculatePath(int** map, int width, int height, int* resultX, int* resultY);

float DisOfNode(shared_ptr<Node> node1, shared_ptr<Node> node2);
void CheckNeighborNode(shared_ptr<AstarNode> centerNode, int width, int height);
shared_ptr<AstarNode> UpdateOpenCloseNodeList();
bool IsContains(AstarNode node, vector<AstarNode>& aStarNodeList);
bool IsContains(Node node, vector<Node>& nodesList);
void OptimizedPath(vector<Node>& nodesList);
void DichotomyEliminate(vector<Node>& nodes, int minIndex, int maxIndex);
bool IsCrossObstacle(Node& node1, Node& node2);

void CalculatePath(int** map, int width, int height, int* resultX, int* resultY) {
	mapWidth = width;
	mapHight = height;
	closeNodesList.clear();
	openNodesList.Clear();
	nodeMap.clear();
	pathNodesList.clear();

	int count = 0;
	for (int i = 0; i < width; i++)
	{
		vector<Node> nodeRow;
		nodeMap.push_back(nodeRow);
		for (int j = 0; j < height; j++) {
			int type = map[i][j];
			Node curNode = *new Node(type, i, j);
			nodeMap[i].push_back(curNode);
			if (type == 1) {
				starNode = make_shared<Node>(type, i, j);
			}
			else if (type == 2) {
				endNode = make_shared<Node>(type, i, j);
			}
		}
	}

	AstarNode startAstarNode((float)0, DisOfNode(starNode, endNode), nullptr, starNode->x, starNode->y);
	AstarNode endAstarNode(DisOfNode(starNode, endNode), 0, nullptr, endNode->x, endNode->y);

	closeNodesList.push_back(startAstarNode);
	curCheckNode = make_shared<AstarNode>(startAstarNode);

	while (curCheckNode != nullptr && (curCheckNode->x != endAstarNode.x || curCheckNode->y != endAstarNode.y))
	{
		CheckNeighborNode(curCheckNode, width, height);
		curCheckNode = UpdateOpenCloseNodeList();
	}

	//结果
	int resultCount = 0;
	vector<Node> tempPathNodesList;
	if (closeNodesList.back().x == endNode->x && closeNodesList.back().y == endNode->y) {
		shared_ptr<AstarNode> curNode = closeNodesList.back().parentNode;
		while (curNode->x != starNode->x || curNode->y != starNode->y)
		{

			/*resultX[resultCount] = curNode->x;
			resultY[resultCount] = curNode->y;
			resultCount++;
			curNode = curNode->parentNode;*/
			tempPathNodesList.push_back(Node(0, curNode->x, curNode->y));
			curNode = curNode->parentNode;
		}
	}
	//平滑路径
	OptimizedPath(tempPathNodesList);

	for (size_t i = 0; i < pathNodesList.size(); i++) {
		resultX[i] = pathNodesList[i].x;
		resultY[i] = pathNodesList[i].y;
	}
}

/// <summary>
/// 优化路径
/// </summary>
/// <param name="nodesList"></param>
void OptimizedPath(vector<Node>& nodesList) {
	vector<Node> nodes;
	Node endNode(*endNode);
	nodes.push_back(endNode);
	for (size_t i = 0; i < nodesList.size(); i++) {
		nodes.push_back(nodesList[i]);
	}
	Node starNode(*starNode);
	nodes.push_back(starNode);
	DichotomyEliminate(nodes, 0, nodes.size() - 1);
}

/// <summary>
/// 二分剔除多余的点
/// </summary>
/// <param name="nodes"></param>
/// <param name="index1"></param>
/// <param name="index2"></param>
void DichotomyEliminate(vector<Node>& nodes, int minIndex, int maxIndex) {
	//两个点相邻
	if (maxIndex - minIndex <= 1) {
		if (!IsContains(nodes[minIndex], pathNodesList)) {
			pathNodesList.push_back(nodes[minIndex]);
		}
		if (!IsContains(nodes[maxIndex], pathNodesList)) {
			pathNodesList.push_back(nodes[maxIndex]);
		}

	}
	else
	{
		if (!IsCrossObstacle(nodes[minIndex], nodes[maxIndex])) {
			if (!IsContains(nodes[minIndex], pathNodesList)) {
				pathNodesList.push_back(nodes[minIndex]);
			}
			if (!IsContains(nodes[maxIndex], pathNodesList)) {
				pathNodesList.push_back(nodes[maxIndex]);
			}
		}
		else
		{
			int middleIndex = (maxIndex - minIndex + 1) / 2 + minIndex;
			DichotomyEliminate(nodes, minIndex, middleIndex);
			DichotomyEliminate(nodes, middleIndex, maxIndex);
		}
	}
}

/// <summary>
/// 是否经过障碍点
/// </summary>
/// <param name="index1"></param>
/// <param name="index2"></param>
/// <returns></returns>
bool IsCrossObstacle(Node& node1, Node& node2) {
	if (abs(node1.x - node2.x) <= 1 && abs(node1.y - node2.y) <= 1) {
		return false;
	}
	else
	{
		if (abs(node1.x - node2.x) <= 1) {
			int nodeYCheckStart = min(node1.y, node2.y);
			for (int i = 1; i < abs(node2.y - node1.y); i++) {
				int yIndex = nodeYCheckStart + i;
				if (nodeMap[node1.x][yIndex].type == 3 || nodeMap[node2.x][yIndex].type == 3) {
					return true;
				}
			}
			return false;
		}
		else if (abs(node1.y - node2.y) <= 1) {
			int nodeXCheckStart = min(node1.x, node2.x);
			for (int i = 1; i < abs(node2.x - node1.x); i++) {
				int xIndex = nodeXCheckStart + i;
				if (nodeMap[xIndex][node1.y].type == 3 || nodeMap[xIndex][node2.y].type == 3) {
					return true;
				}
			}
			return false;
		}
		else
		{
			//斜率
			float k = (node1.y - node2.y) / (node1.x - node2.x);
			for (int i = 1; i < abs(node2.x - node1.x); i++) {
				int sign = node1.x - node2.x > 0 ? 1 : -1;
				int checkX = min(node2.x + i * sign, mapWidth - 1);	
				checkX = max(0, checkX);
				int checkY = min(int(node2.y + checkX * k), mapHight - 1);
				checkY = max(0, checkY);
				if (nodeMap[checkX][checkY].type == 3) {
					return true;
				}
			}
			return false;
		}
	}
}

/// <summary>
/// 检查周围邻居节点
/// </summary>
/// <param name="centerNode"></param>
void CheckNeighborNode(shared_ptr<AstarNode> centerNode, int width, int height) {
	int centerX = (*centerNode).x;
	int centerY = (*centerNode).y;
	bool isTopObstacle = false;
	bool isBottomObstacle = false;
	bool isLeftObstacle = false;
	bool isRightObstacle = false;

	//下
	if (centerY + 1 < height) {
		auto it = nodeMap[centerX].begin() + centerY + 1;
		shared_ptr<Node> bottom = make_shared<Node>(*it);
		if (bottom->type != 3) {
			float disFromStart = centerNode->g + 1;
			float disFromEnd = DisOfNode(bottom, endNode);
			AstarNode aStarNodeBottom(disFromStart, disFromEnd, centerNode, centerX, centerY + 1);

			if (!IsContains(aStarNodeBottom, closeNodesList) && !openNodesList.IsContains(aStarNodeBottom)) {
				openNodesList.Insert(aStarNodeBottom);
			}

		}
		else
		{
			isTopObstacle = true;
		}
	}

	//上
	if (centerY - 1 >= 0) {
		auto it = nodeMap[centerX].begin() + centerY - 1;
		shared_ptr<Node> top = make_shared<Node>(*it);;
		if (top->type != 3) {
			float disFromStart = centerNode->g + 1;
			float disFromEnd = DisOfNode(top, endNode);
			AstarNode aStarNodeTop(disFromStart, disFromEnd, centerNode, centerX, centerY - 1);

			if (!IsContains(aStarNodeTop, closeNodesList) && !openNodesList.IsContains(aStarNodeTop)) {
				openNodesList.Insert(aStarNodeTop);
			}
		}
		else
		{
			isBottomObstacle = true;
		}
	}

	//左
	if (centerX - 1 >= 0) {
		auto it = nodeMap[centerX - 1].begin() + centerY;
		shared_ptr<Node> left = make_shared<Node>(*it);
		if (left->type != 3) {
			float disFromStart = centerNode->g + 1;
			float disFromEnd = DisOfNode(left, endNode);
			AstarNode aStarNodeLeft(disFromStart, disFromEnd, centerNode, centerX - 1, centerY);

			if (!IsContains(aStarNodeLeft, closeNodesList) && !openNodesList.IsContains(aStarNodeLeft)) {
				openNodesList.Insert(aStarNodeLeft);
			}
		}
		else
		{
			isLeftObstacle = true;
		}
	}

	//右
	if (centerX + 1 < width) {
		auto it = nodeMap[centerX + 1].begin() + centerY;
		shared_ptr<Node> right = make_shared<Node>(*it);
		if (right->type != 3) {
			float disFromStart = centerNode->g + 1;
			float disFromEnd = DisOfNode(right, endNode);
			AstarNode aStarNodeRight(disFromStart, disFromEnd, centerNode, centerX + 1, centerY);

			if (!IsContains(aStarNodeRight, closeNodesList) && !openNodesList.IsContains(aStarNodeRight)) {
				openNodesList.Insert(aStarNodeRight);
			}
		}
		else
		{
			isRightObstacle = true;
		}
	}

	//左下
	if (centerX - 1 >= 0 && centerY + 1 < height && !(isTopObstacle && isLeftObstacle)) {
		auto it = nodeMap[centerX - 1].begin() + centerY + 1;
		shared_ptr<Node> leftBottom = make_shared<Node>(*it);
		if (leftBottom->type != 3) {
			float disFromStart = centerNode->g + 1.4f;
			float disFromEnd = DisOfNode(leftBottom, endNode);
			AstarNode aStarNodeLeftBottom(disFromStart, disFromEnd, centerNode, centerX - 1, centerY + 1);

			if (!IsContains(aStarNodeLeftBottom, closeNodesList) && !openNodesList.IsContains(aStarNodeLeftBottom)) {
				openNodesList.Insert(aStarNodeLeftBottom);
			}
		}
	}

	//左上
	if (centerX - 1 >= 0 && centerY - 1 >= 0 && !(isBottomObstacle && isLeftObstacle)) {
		auto it = nodeMap[centerX - 1].begin() + centerY - 1;
		shared_ptr<Node> leftTop = make_shared<Node>(*it);
		if (leftTop->type != 3) {
			float disFromStart = centerNode->g + 1.4f;
			float disFromEnd = DisOfNode(leftTop, endNode);
			AstarNode aStarNodeLeftTop(disFromStart, disFromEnd, centerNode, centerX - 1, centerY - 1);

			if (!IsContains(aStarNodeLeftTop, closeNodesList) && !openNodesList.IsContains(aStarNodeLeftTop)) {
				openNodesList.Insert(aStarNodeLeftTop);
			}
		}
	}

	//右下
	if (centerX + 1 < width && centerY + 1 < height && !(isTopObstacle && isRightObstacle)) {
		auto it = nodeMap[centerX + 1].begin() + centerY + 1;
		shared_ptr<Node> rightBottom = make_shared<Node>(*it);
		if (rightBottom->type != 3) {
			float disFromStart = centerNode->g + 1.4f;
			float disFromEnd = DisOfNode(rightBottom, endNode);
			AstarNode aStarNodeRightBottom(disFromStart, disFromEnd, centerNode, centerX + 1, centerY + 1);

			if (!IsContains(aStarNodeRightBottom, closeNodesList) && !openNodesList.IsContains(aStarNodeRightBottom)) {
				openNodesList.Insert(aStarNodeRightBottom);
			}
		}
	}

	//右上
	if (centerX + 1 < width && centerY - 1 >= 0 && !(isBottomObstacle && isRightObstacle)) {
		auto it = nodeMap[centerX + 1].begin() + centerY - 1;
		shared_ptr<Node> rightTop = make_shared<Node>(*it);
		if (rightTop->type != 3) {
			float disFromStart = centerNode->g + 1.4f;
			float disFromEnd = DisOfNode(rightTop, endNode);
			AstarNode aStarNodeLeftTop(disFromStart, disFromEnd, centerNode, centerX + 1, centerY - 1);

			if (!IsContains(aStarNodeLeftTop, closeNodesList) && !openNodesList.IsContains(aStarNodeLeftTop)) {
				openNodesList.Insert(aStarNodeLeftTop);
			}
		}
	}
}

/// <summary>
/// 更新关闭列表和开启列表
/// </summary>
/// <param name="curCheckNode"></param>
shared_ptr<AstarNode> UpdateOpenCloseNodeList() {
	AstarNode min = openNodesList.DeleteMin();
	shared_ptr<AstarNode> mincostNode = make_shared<AstarNode>(min);
	closeNodesList.push_back(min);
	return mincostNode;
}

/// <summary>
/// 两个节点的距离
/// </summary>
/// <param name="node1"></param>
/// <param name="node2"></param>
/// <returns></returns>
float DisOfNode(shared_ptr<Node> node1, shared_ptr<Node> node2) {
	float result = abs(node1->x - node2->x) + abs(node1->y - node2->y);
	return result;
}

/// <summary>
/// 是否包含
/// </summary>
/// <param name="node"></param>
/// <param name="aStarNodeList"></param>
/// <returns></returns>
bool IsContains(AstarNode node, vector<AstarNode>& aStarNodeList) {
	for (size_t i = 0; i < aStarNodeList.size(); i++) {
		if (node.x == aStarNodeList[i].x && node.y == aStarNodeList[i].y) {
			return true;
		}
	}
	return false;
}

bool IsContains(Node node, vector<Node>& nodesList) {
	for (size_t i = 0; i < nodesList.size(); i++) {
		if (node.x == nodesList[i].x && node.y == nodesList[i].y) {
			return true;
		}
	}
	return false;
}

