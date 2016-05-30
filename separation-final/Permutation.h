#pragma once
#ifndef Permutation_h
#define Permutation_h


class Permutation
{
public:
	int length;
	int *arr;

	Permutation();
	Permutation(int);
	Permutation(int*, int);
	~Permutation();
	void operator*(Permutation);
	Permutation(const Permutation&);
	bool operator==(const Permutation&) const;
	void next();
	void print();
};

#endif