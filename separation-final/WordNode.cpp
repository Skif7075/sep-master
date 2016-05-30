#include "WordNode.h"

WordNode::WordNode(int testAutomataCount)
{
	curStates = new int[testAutomataCount];
	for (int i = 0; i < testAutomataCount; i++)
		curStates[i] = 0;
	word = 0;
	length = 0;
	endOnesCount = 0;
	onesCount = 0;
}

WordNode::~WordNode()
{
	delete[]curStates;
}