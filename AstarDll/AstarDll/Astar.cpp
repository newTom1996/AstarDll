#include <iostream>
#include <vector>
#include "Node.h"
#include "AstarNode.h"
#include <cmath>
#include <float.h>
using namespace std;
//
//extern "C" _declspec(dllexport) void CalculatePath(int** map,int width, int height,int* resultX, int* resultY);
//
//static vector<AstarNode> openNodesList;
//static vector<AstarNode> closeNodesList;
//static Node** nodeMap;
//static Node starNode;
//static Node endNode;
//
//float DisOfNode(const Node& node1, const Node& node2);
//void CheckNeighborNode(const AstarNode* centerNode, int width, int height);
//void UpdateOpenCloseNodeList(AstarNode* curCheckNode);
//
//void CalculatePath(int** map, int width, int height, int* resultX, int* resultY) {
//	openNodesList.clear();
//	closeNodesList.clear();
//
//
//	int count = 0;
//
//	for (int i = 0; i < width; i++)
//	{
//		for (int j = 0; j < height; j++) {
//			int type = *map[i, j];
//			nodeMap[i, j] = new Node(type,i,j);
//			if (type == 0) {
//				starNode = *nodeMap[i, j];
//			}
//			else if (type == 2) {
//				endNode = *nodeMap[i, j];
//			}
//		}
//	}
//
//	AstarNode startAstarNode((float)0,DisOfNode(starNode,endNode),nullptr, starNode.x, starNode.y);
//	AstarNode endAstarNode(DisOfNode(starNode, endNode), 0, nullptr, endNode.x, endNode.y);
//
//	closeNodesList.push_back(startAstarNode);
//	AstarNode* curCheckNode = &startAstarNode;
//
//	while (curCheckNode != nullptr && (curCheckNode->x != endAstarNode.x && curCheckNode->y != endAstarNode.y))
//	{
//		CheckNeighborNode(curCheckNode, width, height);
//		UpdateOpenCloseNodeList(curCheckNode);
//	}
//
//	//结果
//	int resultCount = 0;
//	if (closeNodesList.back().x == endNode.x && closeNodesList.back().y == endNode.y) {
//		const AstarNode* parentNode = closeNodesList.back().parentNode;
//		while (parentNode->x != starNode.x || parentNode->y != starNode.y)
//		{
//			resultX[resultCount] = parentNode->x;
//			resultY[resultCount] = parentNode->y;
//			resultCount++;
//			parentNode = (*parentNode).parentNode;
//		}
//	}
//}
//
///// <summary>
///// 检查周围邻居节点
///// </summary>
///// <param name="centerNode"></param>
//void CheckNeighborNode(const AstarNode* centerNode,int width,int height) {
//	int centerX = centerNode->x;
//	int centerY = centerNode->y;
//	bool isTopObstacle = false;
//	bool isBottomObstacle = false;
//	bool isLeftObstacle = false;
//	bool isRightObstacle = false;
//
//	//上
//	if (centerY + 1 < height) {
//		Node* top = nodeMap[centerX, centerY + 1];
//		if (top->type != 3) {
//			float disFromStart = centerNode->g + 1;
//			float disFromEnd = DisOfNode(*top, endNode);
//			AstarNode aStarNodeTop(disFromStart, disFromEnd, centerNode, centerX, centerY + 1);
//			openNodesList.push_back(aStarNodeTop);
//		}
//		else
//		{
//			isTopObstacle = true;
//		}
//	}
//
//	//下
//	if (centerY - 1 >= 0) {
//		Node* bottom = nodeMap[centerX, centerY - 1];
//		if (bottom->type != 3) {
//			float disFromStart = centerNode->g + 1;
//			float disFromEnd = DisOfNode(*bottom, endNode);
//			AstarNode aStarNodeBottom(disFromStart, disFromEnd, centerNode, centerX, centerY - 1);
//			openNodesList.push_back(aStarNodeBottom);
//		}
//		else
//		{
//			isBottomObstacle = true;
//		}
//	}
//
//	//左
//	if (centerX - 1 >= 0) {
//		Node* left = nodeMap[centerX - 1, centerY];
//		if (left->type != 3) {
//			float disFromStart = centerNode->g + 1;
//			float disFromEnd = DisOfNode(*left, endNode);
//			AstarNode aStarNodeLeft(disFromStart, disFromEnd, centerNode, centerX - 1, centerY);
//			openNodesList.push_back(aStarNodeLeft);
//		}
//		else
//		{
//			isLeftObstacle = true;
//		}
//	}
//
//	//右
//	if (centerX + 1 < width) {
//		Node* right = nodeMap[centerX + 1, centerY];
//		if (right->type != 3) {
//			float disFromStart = centerNode->g + 1;
//			float disFromEnd = DisOfNode(*right, endNode);
//			AstarNode aStarNodeRight(disFromStart, disFromEnd, centerNode, centerX + 1, centerY);
//			openNodesList.push_back(aStarNodeRight);
//		}
//		else
//		{
//			isRightObstacle = true;
//		}
//	}
//
//	//左上
//	if (centerX - 1 >= 0 && centerY + 1 < height && !(isTopObstacle && isLeftObstacle)) {
//		Node* leftTop = nodeMap[centerX - 1, centerY + 1];
//		if (leftTop->type != 3) {
//			float disFromStart = centerNode->g + 1.4f;
//			float disFromEnd = DisOfNode(*leftTop, endNode);
//			AstarNode aStarNodeLeftTop(disFromStart, disFromEnd, centerNode, centerX - 1, centerY + 1);
//			openNodesList.push_back(aStarNodeLeftTop);
//		}
//	}
//
//	//左下
//	if (centerX - 1 >= 0 && centerY - 1 >= 0 && !(isBottomObstacle && isLeftObstacle)) {
//		Node* leftBottom = nodeMap[centerX - 1, centerY - 1];
//		if (leftBottom->type != 3) {
//			float disFromStart = centerNode->g + 1.4f;
//			float disFromEnd = DisOfNode(*leftBottom, endNode);
//			AstarNode aStarNodeLeftBottom(disFromStart, disFromEnd, centerNode, centerX - 1, centerY - 1);
//			openNodesList.push_back(aStarNodeLeftBottom);
//		}
//	}
//
//	//右上
//	if (centerX + 1 < width && centerY + 1 < height && !(isTopObstacle && isRightObstacle)) {
//		Node* rightTop = nodeMap[centerX + 1, centerY + 1];
//		if (rightTop->type != 3) {
//			float disFromStart = centerNode->g + 1.4f;
//			float disFromEnd = DisOfNode(*rightTop, endNode);
//			AstarNode aStarNodeRightTop(disFromStart, disFromEnd, centerNode, centerX + 1, centerY + 1);
//			openNodesList.push_back(aStarNodeRightTop);
//		}
//	}
//
//	//右下
//	if (centerX + 1 < width && centerY - 1 >= 0 && !(isBottomObstacle && isRightObstacle)) {
//		Node* rightBottom = nodeMap[centerX + 1, centerY - 1];
//		if (rightBottom->type != 3) {
//			float disFromStart = centerNode->g + 1.4f;
//			float disFromEnd = DisOfNode(*rightBottom, endNode);
//			AstarNode aStarNodeLeftBottom(disFromStart, disFromEnd, centerNode, centerX + 1, centerY - 1);
//			openNodesList.push_back(aStarNodeLeftBottom);
//		}
//	}
//}
//
///// <summary>
///// 更新关闭列表和开启列表
///// </summary>
///// <param name="curCheckNode"></param>
//void UpdateOpenCloseNodeList(AstarNode* curCheckNode) {
//	AstarNode mincostNode;
//	float curCost = FLT_MAX;
//	//选择开启列表中消耗最小的点移动到关闭列表，要是为Null,curCheckNode = null
//	for (auto itBegin = openNodesList.begin(); itBegin != openNodesList.end(); itBegin++) {
//		if ((*itBegin).GetCost() < curCost) {
//			curCost = (*itBegin).GetCost();
//			mincostNode = (*itBegin);
//		}
//	}
//
//	for (auto it = openNodesList.begin(); it != openNodesList.end(); it++) {
//		if ((*it).x == mincostNode.x && (*it).y == mincostNode.y) {
//			it = openNodesList.erase(it);
//			break;
//		}
//	}
//	closeNodesList.push_back(mincostNode);
//	curCheckNode = &mincostNode;
//}
//
///// <summary>
///// 两个节点的距离
///// </summary>
///// <param name="node1"></param>
///// <param name="node2"></param>
///// <returns></returns>
//float DisOfNode(const Node& node1, const Node& node2) {
//	float result = abs(node1.x - node2.x) + abs(node1.y - node2.y);
//	return result;
//}
//	