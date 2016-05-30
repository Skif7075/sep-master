#pragma once
#ifndef Word_Node_h
#define Word_Node_h

class WordNode
{
private:

public:
	int* curStates;
	unsigned int word;
	int length;
	int endOnesCount;
	int onesCount;

	WordNode(int);
	~WordNode();
};

#endif