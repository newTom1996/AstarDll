#pragma once

/*
0 = normal,					//��ͨ
1 = start,					//���
2 = end,					//�յ�
3 = obstacle,				//�ϰ�
*/

class Node
{
public:
	Node() = default;
	Node(int type, int x, int y);
	~Node();
	int type;			//����
	int x;				//������
	int y;				//������
};

