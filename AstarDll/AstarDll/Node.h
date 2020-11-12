#pragma once

/*
0 = normal,					//普通
1 = start,					//起点
2 = end,					//终点
3 = obstacle,				//障碍
*/

class Node
{
public:
	Node() = default;
	Node(int type, int x, int y);
	~Node();
	int type;			//类型
	int x;				//横坐标
	int y;				//纵坐标
};

