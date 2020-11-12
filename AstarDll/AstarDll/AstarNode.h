#pragma once
#include <iostream>
using namespace std;

class AstarNode
{
public:
	AstarNode() = default;
	AstarNode(float disFromStart, float disFromEnd, shared_ptr<AstarNode> parentNode, int x, int y);
	~AstarNode();
	float g = 0;			//�����ľ���
	float h = 0;			//���յ�ľ���
	shared_ptr<AstarNode> parentNode;	//������
	int x;				//λ�ú�����
	int y;				//λ��������
	float GetCost() const;
	bool operator<(const AstarNode& b)
	{
		return this->GetCost() < b.GetCost();
	}
};