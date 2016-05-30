#include "Permutation.h"
#include "Timer.h"
#include "WordNode.h"
#include <utility>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int k = 5;
int automataCount = 125;
int testAutomataCount = 9;
int branchIndex;
ofstream logfile;
vector<WordNode*> branch;
int calculatedCount = 0;
int* calculatedValues = new int[automataCount];


int* powValues = new int[testAutomataCount];
pair<Permutation*, Permutation*>* automata = new pair<Permutation*, Permutation*>[automataCount];;

void readAutomata()
{
	ifstream fin1, fin2;
	fin1.open("1.txt");
	fin2.open("2.txt");
	for (int i = 0; i < automataCount; i++)
	{
		int* arr1 = new int[5];
		int* arr2 = new int[5];
		for (int j = 0; j < 5; j++)
		{
			fin1 >> arr1[j];
			fin2 >> arr2[j];
		}
		automata[i].first = new Permutation(arr1, 5);
		automata[i].second = new Permutation(arr2, 5);
	}
	fin1.close();
	fin2.close();
}
void setPowValues()
{
	powValues[0] = 1;
	for (int i = 1; i < testAutomataCount; i++) {
		powValues[i] = powValues[i - 1] * k;
	}
}
void setBranchValues()
{
	for (int i = 0; i < 33; i++)
	{
		branch.push_back(new WordNode(testAutomataCount));
		branch[i]->length = i;
	}
}
void pushBit(bool bit)
{
	for (int i = 0; i < testAutomataCount; i++)
	{
		int state = branch[branchIndex]->curStates[i];
		if (bit)
			branch[branchIndex + 1]->curStates[i] = automata[i].second->arr[state];
		else
			branch[branchIndex + 1]->curStates[i] = automata[i].first->arr[state];
	}
	branch[branchIndex + 1]->word = ((branch[branchIndex]->word) << 1);
	if (bit)
	{
		branch[branchIndex + 1]->word |= 1u;
		branch[branchIndex + 1]->endOnesCount = 1 + branch[branchIndex]->endOnesCount;
		branch[branchIndex + 1]->onesCount = 1 + branch[branchIndex]->onesCount;
	}
	else
	{
		branch[branchIndex + 1]->endOnesCount = 0;
		branch[branchIndex + 1]->onesCount = branch[branchIndex]->onesCount;
	}
	branchIndex++;
}
void nextWord(int curLength)
{
	if (branch[branchIndex]->length < curLength)
		pushBit(0);
	else
	{
		int onesCount = branch[branchIndex]->endOnesCount;
		for (int i = 0; i < onesCount + 1; i++)
			branchIndex--;
		pushBit(1);
	}
}
int runAutomaton(pair<Permutation*, Permutation*> automaton, unsigned int word, int length)
{
	int curState = 0;
	for (int j = 0; j < length; j++)
	{
		if (((word&(1 << j)) == 0))
			curState = automaton.first->arr[curState];
		else
			curState = automaton.second->arr[curState];
	}
	return curState;
}
bool compareWords(unsigned int word1, unsigned int word2, int length)
{
	for (int i = 0; i < calculatedCount; i++)
	{
		if (runAutomaton(automata[i], word1, length) != calculatedValues[i])
			return false;
	}
	for (int i = calculatedCount; i < automataCount; i++)
	{
		calculatedValues[i] = runAutomaton(automata[i], word2, length);
		calculatedCount++;
		if (runAutomaton(automata[i], word1, length) != calculatedValues[i])
			return false;
	}
	return true;
}
int hashFunction(int* states, int count)
{
	int hashValue = 0;
	for (int i = 0; i < count; i++)
	{
		hashValue += (long long int)(powValues[i]) * (long long int)(states[i]);
	}
	return hashValue;
}
void printIdentity(int word,int curLength)
{
	for (int i = 0; i < curLength; i++)
	{
		cout << ((word&(1 << (curLength - i - 1))) ? 1 : 0);
		logfile << ((word&(1 << (curLength - i - 1))) ? 1 : 0);
	}
	cout << "="; 
	logfile << "=";
	for (int i = 0; i < curLength; i++)
	{
		cout << ((branch[branchIndex]->word&(1 << (curLength - i - 1))) ? 1 : 0);
		logfile << ((branch[branchIndex]->word&(1 << (curLength - i - 1))) ? 1 : 0);
	}
	cout << endl; 
	logfile << endl;
}
void main()
{
	for (int i = 0; i < automataCount; i++)
	{
		automata[i] = pair<Permutation*, Permutation*>();
	}
	readAutomata();
	setPowValues();
	setBranchValues();
	logfile.open("log.txt");
	int max = k * powValues[testAutomataCount - 1];

	vector<uint32_t>** hashTable = new vector<uint32_t>*[max];
	for (int i = 0; i < max;i++)
	{
		hashTable[i] = new vector<uint32_t>();
	}
	Timer* timer = new Timer();
	for (int curLength = 4; curLength <= 32; curLength++) 
	{
		cout << "Length: " << curLength<<endl;
		logfile << "Length: " << curLength<<endl;
		timer->start();
		unsigned int mask = (1u << (curLength - 1));
		branchIndex = 0;
		do
		{
			calculatedCount = 0;
			nextWord(curLength);
			if (branch[branchIndex]->length == curLength && branch[branchIndex]->onesCount <= ((int)curLength / 2))
			{
				int hashValue = hashFunction(branch[branchIndex]->curStates, testAutomataCount);
				if (branch[branchIndex]->word&mask)
				{
					if (!(*hashTable[hashValue]).empty())
					{
						for (vector<unsigned int>::iterator it = (*hashTable[hashValue]).begin(); it != (*hashTable[hashValue]).end(); ++it)
						{
							if ((*it & 1u) != (branch[branchIndex]->word & 1u))
								if (compareWords(*it, branch[branchIndex]->word, curLength))
								{
									printIdentity(*it, curLength);
								}
						}
					}
				}
				else
				{
					(*hashTable[hashValue]).push_back(branch[branchIndex]->word);
				}
			}
		} while (branch[branchIndex]->endOnesCount < curLength);
		cout << "Seconds passed: " << timer->getCurSeconds() << endl;
		logfile << "Seconds passed: " << timer->getCurSeconds() << endl;
		for (int i = 0; i < max; i++)
			(*hashTable[i]).clear();
	}
	system("pause");
}