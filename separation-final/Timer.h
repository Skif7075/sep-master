#pragma once
#ifndef Timer_h
#define Timer_h

class Timer
{
private:
	int startSeconds;
public:
	void start();
	int getCurSeconds();

};

#endif