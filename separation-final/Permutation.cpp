#include "Permutation.h"
#include <algorithm>
#include <iostream>
using namespace std;

Permutation::Permutation(int length)
{
	this->length = length;
	arr = new int[length];
	for (int i = 0; i < length; i++)
		arr[i] = i;
}
Permutation::Permutation()
{
}
Permutation::Permutation(int* arr, int length)
{
	this->length = length;
	this->arr = arr;
}
Permutation::~Permutation()
{
	delete[]arr;
}
void Permutation::operator*(Permutation arg) {
	for (int i = 0; i < length; i++)
		this->arr[i] = arg.arr[this->arr[i]];
}
Permutation::Permutation(const Permutation &t)
{
	this->length = t.length;
	this->arr = new int[length];
	for (int i = 0; i < length; i++)
		this->arr[i] = t.arr[i];
}
bool Permutation::operator==(const Permutation &t) const {
	if (this->length != t.length)
		return false;
	for (int i = 0; i < length; i++)
		if (this->arr[i] != t.arr[i])
			return false;
	return true;
}
void Permutation::next()
{
	next_permutation(this->arr, this->arr + this->length);
}
void Permutation::print()
{
	for (int i = 0; i < length; i++)
		cout << arr[i];
	cout << endl;
}
