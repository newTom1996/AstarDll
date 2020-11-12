#pragma once
#include <iostream>
using namespace std;

class AstarNode
{
public:
	AstarNode() = default;
	AstarNode(float disFromStart, float disFromEnd, shared_ptr<AstarNode> parentNode, int x, int y);
	~AstarNode();
	float g = 0;			//离起点的距离
	float h = 0;			//离终点的距离
	shared_ptr<AstarNode> parentNode;	//父格子
	int x;				//位置横坐标
	int y;				//位置纵坐标
	float GetCost() const;
	bool operator<(const AstarNode& b)
	{
		return this->GetCost() < b.GetCost();
	}
};