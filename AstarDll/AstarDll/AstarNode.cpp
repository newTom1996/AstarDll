#include "pch.h"
#include "AstarNode.h"
#include <iostream>
using namespace std;

AstarNode::AstarNode(float disFromStart, float disFromEnd, shared_ptr<AstarNode> parentNode, int x, int y) : g(disFromStart), h(disFromEnd), parentNode(parentNode), x(x), y(y) {

}

AstarNode::~AstarNode() {

}

float AstarNode::GetCost() const {
	return h + g;
}